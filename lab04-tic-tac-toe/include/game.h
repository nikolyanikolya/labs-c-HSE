#ifndef GAME_H_
#define GAME_H_

#include <string>
#include <vector>

namespace tictactoe {

struct Game {
private:
    char turn = 'O';
    std::vector<std::vector<char>> my_field;
    const int HEIGHT = 10;
    const int WIDTH = 10;
    const int ROW_TO_WIN = 5;

public:
    Game();

    [[nodiscard]] bool is_occupied(const int &a, const int &b) const;

    void put_move(const int &a, const int &b);

    [[nodiscard]] int get_Height() const;

    [[nodiscard]] int get_Width() const;

    [[nodiscard]] std::vector<std::vector<char>> show_field() const;

    [[nodiscard]] bool check_winner_vertical() const;

    [[nodiscard]] bool check_winner_horizontal() const;

    [[nodiscard]] bool check_winner_diagonal_1() const;

    [[nodiscard]] bool check_winner_diagonal_2() const;

    [[nodiscard]] bool check_winner() const;

    [[nodiscard]] char get_turn() const;

    [[nodiscard]] bool is_draw() const;
};
}  // namespace tictactoe

#endif  // GAME_H_
