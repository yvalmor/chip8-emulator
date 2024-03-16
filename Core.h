//
// Created by yvonm on 16/03/2024.
//

#ifndef CHIP_8_EMULATOR_CORE_H
#define CHIP_8_EMULATOR_CORE_H


#include "opcode.h"
class Core {
public:
    unsigned short drawFlag;

private:
    unsigned short opcode;

    unsigned char memory[4096];

    unsigned char V[16];

    unsigned short I;
    unsigned short pc;

    unsigned char gfx[64 * 32];

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[16];
    unsigned short sp;

    unsigned char key[16];

public:
    void initialize();
    void loadGame(const char *game);
    void emulateCycle();
    void drawGraphics();
    void setKeys();

private:
    void loadFontSet();

    void fetchOpcode();
    void executeOpcode();

    void updateTimers();

    void display(Opcode decodedOpcode);
};


#endif//CHIP_8_EMULATOR_CORE_H
