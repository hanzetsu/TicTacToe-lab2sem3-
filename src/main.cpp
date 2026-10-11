#include <iostream>
#include "core/Board.hpp"

int main()
{
    Board b;
    b.print();
    while (!b.isGameOver())
    {

        int r, c;
        std::cout << "Ход " << (b.whoseMove() == 1 ? 'X' : 'O') << '\n';
        std::cout << "Введите ряд и столбец (1-3): ";
        if (!(std::cin >> r >> c))
        {
            std::cin.clear();             // сбросить fail state
            std::cin.ignore(10000, '\n'); // выбросить буфер до конца строки
            std::cout << "Некорректный ввод.\n";
            continue;
        }

        if (r < 1 || r > 3 || c < 1 || c > 3)
        {
            std::cout << "Координаты должны быть от 1 до 3.\n";
            b.print();
            continue;
        }
        --r;
        --c;
        if (!b.makeMove(r, c))
        {
            std::cout << "Клетка занята, попробуйте снова.\n";
            b.print();
            continue; // не выходим, повторим ход
        }
        b.print();
    }
    if (b.isWin(1))
        std::cout << "X победил!\n";
    else if (b.isWin(2))
        std::cout << "O победил!\n";
    else
        std::cout << "Ничья.\n";
}