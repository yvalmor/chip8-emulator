#include "Core.h"
#include <iostream>

#include "SDL2/SDL.h"

int main(int argc, char **argv) {
    Core core = Core();

    if (argc != 2) {
        std::cout << "Usage: chip8 <ROM>" << std::endl;
        return 1;
    }

    // TODO: setup graphics and input
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    } else {
        std::cout << "SDL_Init Success" << std::endl;
    }

    return 0;

    core.initialize();
    core.loadGame(argv[1]);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        core.emulateCycle();

        if (core.drawFlag)
            core.drawGraphics();

        core.setKeys();
    }
#pragma clang diagnostic pop

    return 0;
}
