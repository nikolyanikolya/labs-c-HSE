#include "game.h"

namespace tictactoe {
Game::Game() {
    my_field.assign(HEIGHT, std::vector<char>(WIDTH, '.'));
}

bool Game::is_occupied(const int &a, const int &b) const {
    return my_field[a][b] == 'O' || my_field[a][b] == 'X';
}

void Game::put_move(const int &a, const int &b) {
    if (!is_occupied(a, b)) {
        my_field[a][b] = turn;
        if (!check_winner()) {
            if (turn == 'O') {
                turn = 'X';
            } else if (turn == 'X') {
                turn = 'O';
            }
        }
    }
}

int Game::get_Height() const {
    return HEIGHT;
}

int Game::get_Width() const {
    return WIDTH;
}

std::vector<std::vector<char>> Game::show_field() const {
    return my_field;
}

bool Game::check_winner_horizontal() const {
    for (int i = 0; i < HEIGHT; i++) {
        int counter = 0;
        for (int j = 0; j < WIDTH; j++) {
            if (my_field[i][j] == 'O' || my_field[i][j] == 'X') {
                if (counter == 0 || my_field[i][j] == my_field[i][j - 1]) {
                    counter++;
                } else if (my_field[i][j] != my_field[i][j - 1]) {
                    counter = 1;
                }
            } else {
                counter = 0;
            }
            if (counter == ROW_TO_WIN) {
                return true;
            }
        }
    }
    return false;
}

bool Game::check_winner_vertical() const {
    for (int j = 0; j < WIDTH; j++) {
        int counter = 0;
        for (int i = 0; i < HEIGHT; i++) {
            if (my_field[i][j] == 'O' || my_field[i][j] == 'X') {
                if (counter == 0 || my_field[i][j] == my_field[i - 1][j]) {
                    counter++;
                } else if (my_field[i][j] != my_field[i - 1][j]) {
                    counter = 1;
                }
            } else {
                counter = 0;
            }
            if (counter == ROW_TO_WIN) {
                return true;
            }
        }
    }
    return false;
}

bool Game::check_winner_diagonal_1() const {
    for (int h = 0; h < WIDTH + HEIGHT; h++) {
        int j = std::max(WIDTH - 1 - h, 0);
        int counter = 0;
        int i = std::max(h - WIDTH + 1, 0);
        for (; j < WIDTH && i < HEIGHT; i++, j++) {
            if (my_field[i][j] == 'O' || my_field[i][j] == 'X') {
                if (counter == 0 || my_field[i][j] == my_field[i - 1][j - 1]) {
                    counter++;
                } else if (my_field[i][j] != my_field[i - 1][j - 1]) {
                    counter = 1;
                }
            } else {
                counter = 0;
            }
            if (counter == ROW_TO_WIN) {
                return true;
            }
        }
    }
    return false;
}

bool Game::check_winner_diagonal_2() const {
    for (int h = 0; h < HEIGHT + WIDTH; h++) {
        int j = std::min(h, WIDTH - 1);
        int i = h - j;
        int counter = 0;
        for (; j >= 0 && i < HEIGHT; i++, j--) {
            if (my_field[i][j] == 'O' || my_field[i][j] == 'X') {
                if (counter == 0 || my_field[i][j] == my_field[i - 1][j + 1]) {
                    counter++;
                } else if (my_field[i][j] != my_field[i - 1][j + 1]) {
                    counter = 1;
                }
            } else {
                counter = 0;
            }
            if (counter == ROW_TO_WIN) {
                return true;
            }
        }
    }
    return false;
}

bool Game::check_winner() const {
    return check_winner_vertical() || check_winner_horizontal() ||
           check_winner_diagonal_1() || check_winner_diagonal_2();
}

char Game::get_turn() const {
    return turn;
}

bool Game::is_draw() const {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (my_field[i][j] != 'O' && my_field[i][j] != 'X') {
                return false;
            }
        }
    }
    return true;
}

namespace {

// File-local declarations and definitions.
}  // namespace

// Externally visible declarations and definitions.
}  // namespace tictactoe
