#ifndef VIEW_H_
#define VIEW_H_

#include "game.h"

namespace tictactoe {
struct View {
private:
    Game &game_;

public:
    explicit View(Game &game);
    void print_field();
    void play();
    void current_move();
    void bad_move();
};
}  // namespace tictactoe

#endif  // VIEW_H_