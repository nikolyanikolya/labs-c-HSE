#include "view.h"
int main() {
    tictactoe::Game your_game;
    tictactoe::View your_view(your_game);
    your_view.play();
    return 0;
}