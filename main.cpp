#include "Core.h"
#include <iostream>

int main(int argc, char **argv) {
    Core core = Core();

    if (argc != 2) {
        std::cout << "Usage: chip8 <ROM>" << std::endl;
        return 1;
    }

    // TODO: setup graphics and input

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
