#include "game.h"
#include "mytest.h"

namespace tictactoe {
void do_checks(const Game &my_game) {
    CHECK(!my_game.is_draw());
    CHECK(!my_game.check_winner());
    CHECK(!my_game.check_winner_diagonal_1());
    CHECK(!my_game.check_winner_diagonal_2());
    CHECK(!my_game.check_winner_horizontal());
    CHECK(!my_game.check_winner_vertical());
}

TEST_CASE("without moves") {
    Game my_game;
    do_checks(my_game);
    for (int i = 0; i < my_game.get_Height(); i++) {
        for (int j = 0; j < my_game.get_Width(); j++) {
            CHECK(!my_game.is_occupied(i, j));
        }
    }
}

TEST_CASE("get_Height") {
    Game my_game;
    CHECK(my_game.get_Height() == 10);
}

TEST_CASE("get_Width") {
    Game my_game;
    CHECK(my_game.get_Width() == 10);
}

TEST_CASE("show_field") {
    Game my_game;
    bool flag = true;
    for (int i = 0; i < my_game.get_Height(); i++) {
        for (int j = 0; j < my_game.get_Width(); j++) {
            if (my_game.show_field()[i][j] != '.') {
                flag = false;
            }
        }
    }
    CHECK(flag);
}

TEST_CASE("put_move, is_occupied") {
    Game my_game;
    CHECK(!my_game.is_occupied(1, 2));
    my_game.put_move(1, 2);
    CHECK(my_game.is_occupied(1, 2));
    CHECK(!my_game.is_occupied(0, 0));
    my_game.put_move(0, 0);
    CHECK(my_game.is_occupied(0, 0));
    my_game.put_move(0, 0);
    CHECK(my_game.is_occupied(0, 0));
    CHECK(!my_game.is_occupied(0, 1));
    my_game.put_move(0, 1);
    CHECK(my_game.is_occupied(0, 1));
    CHECK(my_game.get_Width() == 10);
    CHECK(my_game.get_Height() == 10);
}

TEST_CASE("put_move, show_field") {
    Game my_game;
    my_game.put_move(1, 2);
    bool flag = my_game.show_field()[1][2] == 'O';
    for (int i = 0; i < my_game.get_Height(); i++) {
        for (int j = 0; j < my_game.get_Width(); j++) {
            if (my_game.show_field()[i][j] != '.') {
                if (i != 1 || j != 2) {
                    flag = false;
                }
            }
        }
    }
    CHECK(flag);
    my_game.put_move(0, 0);
    flag = my_game.show_field()[0][0] == 'X';
    for (int i = 0; i < my_game.get_Height(); i++) {
        for (int j = 0; j < my_game.get_Width(); j++) {
            if (my_game.show_field()[i][j] != '.') {
                if ((i != 0 || j != 0) && (i != 1 || j != 2)) {
                    flag = false;
                }
            }
        }
    }
    CHECK(flag);
}

TEST_CASE("winner is on the diagonal_2") {
    Game my_game;
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(1, 1);
    CHECK(my_game.get_turn() == 'X');
    do_checks(my_game);
    my_game.put_move(1, 1);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(0, 9);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(9, 0);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(1, 8);
    CHECK(my_game.get_turn() == 'O');
    do_checks(my_game);
    my_game.put_move(8, 0);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(2, 7);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(7, 0);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(3, 6);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(6, 0);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(4, 5);
    CHECK(!my_game.is_draw());
    CHECK(!my_game.check_winner_vertical());
    CHECK(!my_game.check_winner_horizontal());
    CHECK(!my_game.check_winner_diagonal_1());
    CHECK(my_game.check_winner());
    CHECK(my_game.get_turn() == 'X');
    CHECK(my_game.check_winner_diagonal_2());
}

TEST_CASE("winner on the diagonal_1") {
    Game my_game;
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(9, 9);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(9, 9);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(0, 0);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(8, 8);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(0, 1);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(7, 7);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(0, 2);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(6, 6);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(0, 3);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(5, 5);
    CHECK(!my_game.is_draw());
    CHECK(my_game.check_winner_diagonal_1());
    CHECK(!my_game.check_winner_diagonal_2());
    CHECK(!my_game.check_winner_vertical());
    CHECK(!my_game.check_winner_horizontal());
    CHECK(my_game.check_winner());
    CHECK(my_game.get_turn() == 'O');
}

TEST_CASE("winner is on the vertical") {
    Game my_game;
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(0, 5);
    CHECK(my_game.get_turn() == 'X');
    do_checks(my_game);
    my_game.put_move(0, 5);
    CHECK(my_game.get_turn() == 'X');
    do_checks(my_game);
    my_game.put_move(9, 9);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(1, 5);
    CHECK(my_game.get_turn() == 'X');
    do_checks(my_game);
    my_game.put_move(8, 8);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(2, 5);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(7, 7);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(3, 5);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'X');
    my_game.put_move(6, 6);
    do_checks(my_game);
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(4, 5);
    CHECK(!my_game.is_draw());
    CHECK(my_game.check_winner_vertical());
    CHECK(my_game.check_winner());
    CHECK(my_game.get_turn() == 'O');
}

TEST_CASE("winner is on the horizontal") {
    Game my_game;
    CHECK(my_game.get_turn() == 'O');
    my_game.put_move(0, 0);
    CHECK(my_game.get_turn() == 'X');
    do_checks(my_game);
    my_game.put_move(0, 0);
    CHECK(my_game.get_turn() == 'X');
    do_checks(my_game);
    my_game.put_move(1, 0);
    CHECK(my_game.get_turn() == 'O');
    do_checks(my_game);
    my_game.put_move(9, 9);
    CHECK(my_game.get_turn() == 'X');
    do_checks(my_game);
    my_game.put_move(1, 1);
    CHECK(my_game.get_turn() == 'O');
    do_checks(my_game);
    my_game.put_move(8, 8);
    CHECK(my_game.get_turn() == 'X');
    do_checks(my_game);
    my_game.put_move(1, 2);
    CHECK(my_game.get_turn() == 'O');
    do_checks(my_game);
    my_game.put_move(7, 7);
    CHECK(my_game.get_turn() == 'X');
    do_checks(my_game);
    my_game.put_move(1, 3);
    CHECK(my_game.get_turn() == 'O');
    do_checks(my_game);
    my_game.put_move(6, 6);
    CHECK(my_game.get_turn() == 'X');
    do_checks(my_game);
    my_game.put_move(1, 4);
    CHECK(!my_game.is_draw());
    CHECK(!my_game.check_winner_diagonal_2());
    CHECK(!my_game.check_winner_diagonal_1());
    CHECK(my_game.check_winner_horizontal());
    CHECK(my_game.check_winner());
    CHECK(my_game.get_turn() == 'X');
}

TEST_CASE("is_draw, show_field") {
    Game my_game;
    bool flag = true;
    for (int i = 0; i < 10; i += 2) {
        for (int j = 0; j < 10; j += 2) {
            if (flag) {
                my_game.put_move(i, j);
                my_game.put_move(i + 1, j);
                my_game.put_move(i, j + 1);
                my_game.put_move(i + 1, j + 1);
                flag = false;
            } else {
                my_game.put_move(i + 1, j);
                my_game.put_move(i, j);
                my_game.put_move(i + 1, j + 1);
                my_game.put_move(i, j + 1);
                flag = true;
            }
        }
    }
    bool flag_1 = true;
    for (int i = 0; i < my_game.get_Height(); i++) {
        for (int j = 0; j < my_game.get_Width(); j++) {
            if (my_game.show_field()[i][j] == '.') {
                flag_1 = false;
            }
        }
    }

    CHECK(flag_1);
    CHECK(my_game.is_draw());
}

TEST_CASE("is_occupied") {
    Game my_game;
    bool flag = true;
    for (int i = 0; i < 10; i += 2) {
        for (int j = 0; j < 10; j += 2) {
            if (flag) {
                my_game.put_move(i, j);
                my_game.put_move(i + 1, j);
                my_game.put_move(i, j + 1);
                my_game.put_move(i + 1, j + 1);
                flag = false;
            } else {
                my_game.put_move(i + 1, j);
                my_game.put_move(i, j);
                my_game.put_move(i + 1, j + 1);
                my_game.put_move(i, j + 1);
                flag = true;
            }
        }
    }
    for (int i = 0; i < my_game.get_Height(); i++) {
        for (int j = 0; j < my_game.get_Width(); j++) {
            CHECK(my_game.is_occupied(i, j));
        }
    }
}
}  // namespace tictactoe
