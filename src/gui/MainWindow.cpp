#include "gui/MainWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFont>
#include <QPushButton>
#include <algorithm>
#include <chrono>
#include "agents/MinimaxAgent.hpp"
#include "agents/AlphaBetaAgent.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Крестики-нолики");

    auto* central = new QWidget(this);
    auto* vbox = new QVBoxLayout(central);

    auto* controls = new QHBoxLayout();

    auto* newGameBtn = new QPushButton("Новая игра", central);
    connect(newGameBtn, &QPushButton::clicked, this, &MainWindow::onNewGame);
    controls->addWidget(newGameBtn);

    sizeCombo = new QComboBox(central);
    sizeCombo->addItem("3×3");
    sizeCombo->addItem("4×4");
    sizeCombo->addItem("5×5");
    sizeCombo->setCurrentIndex(1);
    connect(sizeCombo, &QComboBox::currentIndexChanged,
            this, &MainWindow::onSizeChanged);
    controls->addWidget(new QLabel("Поле:", central));
    controls->addWidget(sizeCombo);

    algorithmCombo = new QComboBox(central);
    algorithmCombo->addItem("Minimax");
    algorithmCombo->addItem("Alpha-Beta");
    algorithmCombo->setCurrentIndex(1);
    connect(algorithmCombo, &QComboBox::currentIndexChanged,
            this, &MainWindow::onAlgorithmChanged);
    controls->addWidget(new QLabel("Алгоритм:", central));
    controls->addWidget(algorithmCombo);

    depthSpin = new QSpinBox(central);
    depthSpin->setRange(1, 12);
    depthSpin->setValue(6);
    connect(depthSpin, &QSpinBox::valueChanged,
            this, &MainWindow::onDepthChanged);
    controls->addWidget(new QLabel("Глубина:", central));
    controls->addWidget(depthSpin);

    playAsOCheck = new QCheckBox("Играть за O", central);
    playAsOCheck->setChecked(false);
    connect(playAsOCheck, &QCheckBox::checkStateChanged,
            this, &MainWindow::onSideChanged);
    controls->addWidget(playAsOCheck);

    vbox->addLayout(controls);

    statusLabel = new QLabel("Ход: X", central);
    statusLabel->setAlignment(Qt::AlignCenter);
    QFont sf = statusLabel->font();
    sf.setPointSize(14);
    sf.setBold(true);
    statusLabel->setFont(sf);
    vbox->addWidget(statusLabel);

    grid = new QGridLayout();
    vbox->addLayout(grid);

    timeLabel = new QLabel("", central);
    timeLabel->setAlignment(Qt::AlignCenter);
    vbox->addWidget(timeLabel);

    setCentralWidget(central);

    N = 4; K = 3;
    board = Board(N, K);
    rebuildGrid();
    applyDepthLimit();
    recreateAgent();
    updateStatus();

    resize(700, 750);
}

MainWindow::~MainWindow() {
    delete agent;
}

void MainWindow::applyDepthLimit() {
    int maxDepth;
    if (N == 3)      maxDepth = 9;
    else if (N == 4) maxDepth = 7;
    else             maxDepth = 5;

    depthSpin->setMaximum(maxDepth);
    if (depthSpin->value() > maxDepth) {
        depthSpin->setValue(maxDepth);
    }
}

void MainWindow::recreateAgent() {
    delete agent;
    agent = nullptr;

    int depth = depthSpin ? depthSpin->value() : 6;
    int choice = algorithmCombo ? algorithmCombo->currentIndex() : 1;

    switch (choice) {
        case 0:  agent = new MinimaxAgent(depth);    break;
        case 1:  agent = new AlphaBetaAgent(depth);  break;
        default: agent = new AlphaBetaAgent(depth);  break;
    }
}

void MainWindow::rebuildGrid() {
    for (QPushButton* b : buttons) {
        grid->removeWidget(b);
        delete b;
    }
    buttons.clear();

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int index = r * N + c;
            auto* btn = new QPushButton("", this);
            btn->setMinimumSize(40, 40);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            QFont f = btn->font();
            f.setPointSize(N == 3 ? 28 : (N == 4 ? 22 : 18));
            f.setBold(true);
            btn->setFont(f);

            connect(btn, &QPushButton::clicked, this, [this, index]() {
                onCellClicked(index);
            });

            grid->addWidget(btn, r, c);
            buttons.append(btn);
        }
    }
}

void MainWindow::refreshCell(int index) {
    int r = index / N;
    int c = index % N;
    int cell = board.getCell(r, c);

    if (cell == 1) buttons[index]->setText("X");
    else if (cell == 2) buttons[index]->setText("O");
    else buttons[index]->setText("");

    bool isOngoing = (board.getGameState() == GameState::Ongoing);
    buttons[index]->setEnabled(cell == 0 && isOngoing && isHumanTurn());
}

void MainWindow::refreshAll() {
    for (int i = 0; i < N * N; ++i) {
        refreshCell(i);
    }
}

void MainWindow::updateStatus() {
    GameState state = board.getGameState();
    
    if (state == GameState::WinX)        statusLabel->setText("X победил!");
    else if (state == GameState::WinO)   statusLabel->setText("O победил!");
    else if (state == GameState::Draw)   statusLabel->setText("Ничья");
    else                                 statusLabel->setText(QString("Ход: %1").arg(board.whoseMove() == 1 ? "X" : "O"));

    for (int i = 0; i < N * N; ++i) {
        refreshCell(i);
    }
}

bool MainWindow::isHumanTurn() const {
    int p = board.whoseMove();
    return (humanIsX && p == 1) || (!humanIsX && p == 2);
}

void MainWindow::onCellClicked(int index) {
    if (board.getGameState() != GameState::Ongoing) return;
    if (!isHumanTurn()) return;

    int r = index / N;
    int c = index % N;

    if (!board.makeMove(r, c)) return;

    updateStatus();
    if (board.getGameState() != GameState::Ongoing) return;

    makeAgentMove();
}

void MainWindow::makeAgentMove() {
    if (board.getGameState() != GameState::Ongoing) return;
    if (!agent) return;

    auto t0 = std::chrono::high_resolution_clock::now();
    int move = agent->chooseMove(board);
    auto t1 = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    if (move < 0 || move >= N * N) return;

    int r = move / N;
    int c = move % N;
    board.makeMove(r, c);

    updateStatus();
    timeLabel->setText(QString("Ход агента: %1 мс").arg(ms, 0, 'f', 1));
}

void MainWindow::onNewGame() {
    board = Board(N, K);
    timeLabel->setText("");
    updateStatus();

    if (!humanIsX) {
        makeAgentMove();
    }
}

void MainWindow::onSizeChanged(int index) {
    N = 3 + index;
    K = 3;

    board = Board(N, K);
    rebuildGrid();
    applyDepthLimit();
    recreateAgent();

    timeLabel->setText("");
    updateStatus();

    if (!humanIsX) {
        makeAgentMove();
    }
}

void MainWindow::onAlgorithmChanged(int /*index*/) {
    applyDepthLimit();
    recreateAgent();
    onNewGame();
}

void MainWindow::onDepthChanged(int /*depth*/) {
    recreateAgent();
    onNewGame();
}

void MainWindow::onSideChanged(Qt::CheckState state) {
    humanIsX = (state == Qt::Unchecked);
    onNewGame();
}