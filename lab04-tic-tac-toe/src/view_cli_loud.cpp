#include <iostream>
#include <sstream>
#include <string>
#include "view.h"

namespace tictactoe {
View::View(Game &game) : game_(game) {
}

void View::print_field() {
    std::cout << std::endl;
    for (int i = 0; i < game_.get_Height(); i++) {
        for (int j = 0; j < game_.get_Width(); j++) {
            std::cout << game_.show_field()[i][j];
        }
        std::cout << std::endl;
    }
}

void View::play() {
    print_field();
    std::cout << game_.get_turn() << " "
              << "move"
              << ":"
              << " ";
    while (!game_.check_winner() && !game_.is_draw()) {
        std::string str;
        getline(std::cin, str);
        std::stringstream s;
        std::stringstream v;
        std::string word_1;
        std::string word_2;
        s << str;
        v << str;
        int num_1 = -2;
        int num_2 = -2;
        s >> num_1;
        s >> num_2;
        v >> word_1;
        v >> word_2;
        if (num_2 == 0) {
            if (!((word_2.length() != 0 && word_2[0] == '0') ||
                  ((word_2.length() > 1 &&
                    (word_2[0] == '+' || word_2[0] == '-')) &&
                   word_2[1] == '0'))) {
                num_2 = -2;
            }
        }
        if (num_1 == -1 && num_2 == -1) {
            break;
        } else {
            if (num_1 < 0 || num_2 < 0 || num_1 >= game_.get_Height() ||
                num_2 >= game_.get_Width()) {
                bad_move();
            } else {
                if (!game_.is_occupied(num_1, num_2)) {
                    game_.put_move(num_1, num_2);
                    current_move();
                } else {
                    bad_move();
                }
            }
        }
    }
}

void View::current_move() {
    if (!game_.is_draw() && !game_.check_winner()) {
        print_field();
        std::cout << game_.get_turn() << " "
                  << "move"
                  << ":"
                  << " ";
    } else if (game_.check_winner()) {
        print_field();
        std::cout << game_.get_turn() << " "
                  << "wins!" << std::endl;
    } else if (game_.is_draw()) {
        print_field();
        std::cout << std::endl;
        std::cout << "Draw." << std::endl;
    }
}

void View::bad_move() {
    std::cout << "Bad move!" << std::endl;
    std::cout << game_.get_turn() << " "
              << "move"
              << ":"
              << " ";
}
}  // namespace tictactoe
