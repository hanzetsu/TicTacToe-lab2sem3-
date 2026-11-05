#pragma once
#include <QMainWindow>
#include <QGridLayout>
#include <QVector>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include "core/Board.hpp"
#include "agents/IAgent.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onCellClicked(int index);
    void onNewGame();
    void onAlgorithmChanged(int index);
    void onDepthChanged(int depth);
    void onSideChanged(Qt::CheckState state);
    void onSizeChanged(int index);

private:
    int N = 4;
    int K = 3;

    Board board{3, 3};
    IAgent* agent = nullptr;
    bool humanIsX = true;

    QGridLayout* grid = nullptr;
    QVector<QPushButton*> buttons;
    QLabel* statusLabel = nullptr;
    QLabel* timeLabel = nullptr;
    QComboBox* algorithmCombo = nullptr;
    QComboBox* sizeCombo = nullptr;
    QSpinBox* depthSpin = nullptr;
    QCheckBox* playAsOCheck = nullptr;

    void rebuildGrid();
    void refreshCell(int index);
    void refreshAll();
    void updateStatus();
    void makeAgentMove();
    void recreateAgent();
    void applyDepthLimit();
    bool isHumanTurn() const;
};