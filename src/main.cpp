#include <iostream>

#include <iostream>
#include <cstdlib>
#include "game.hpp"

using namespace std;

int main(int, char**) {
    Game game("game", "root");
    game.loadScene();
    game.bindInputs();
    game.runGameLoop();
    return EXIT_SUCCESS;
}
