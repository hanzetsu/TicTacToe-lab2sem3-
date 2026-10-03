#include <iostream>
#include "core/Board.hpp"

int main() {
    Board b;
    while(!b.isGameOver()) {
        int move_r, move_c;
        std::cout<<"Введите сначала ряд и потом столбец хода"<<std::endl;
        std::cin>>move_r;
        std::cin>>move_c;
        if (!b.makeMove(move_r,move_c)) std::cerr<<"Error"<<std::endl;
        b.print();
    }
}