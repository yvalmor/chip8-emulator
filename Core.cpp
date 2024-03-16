//
// Created by yvonm on 16/03/2024.
//

#include "Core.h"
#include <fstream>
#include <iostream>
#include <random>

void Core::initialize() {
    drawFlag = false;

    opcode = 0;

    for (unsigned char &i: memory)
        i = 0;

    for (unsigned char &i: V)
        i = 0;

    I = 0;
    pc = 0x200;

    for (unsigned char &i: gfx)
        i = false;

    delay_timer = 0;
    sound_timer = 0;

    for (unsigned short &i: stack)
        i = 0;

    sp = 0;

    for (unsigned char &i: key)
        i = false;

    loadFontSet();
}

void Core::loadFontSet() {
    unsigned char fontSet[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0,// 0
            0x20, 0x60, 0x20, 0x20, 0x70,// 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0,// 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0,// 3
            0x90, 0x90, 0xF0, 0x10, 0x10,// 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0,// 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0,// 6
            0xF0, 0x10, 0x20, 0x40, 0x40,// 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0,// 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0,// 9
            0xF0, 0x90, 0xF0, 0x90, 0x90,// A
            0xE0, 0x90, 0xE0, 0x90, 0xE0,// B
            0xF0, 0x80, 0x80, 0x80, 0xF0,// C
            0xE0, 0x90, 0x90, 0x90, 0xE0,// D
            0xF0, 0x80, 0xF0, 0x80, 0xF0,// E
            0xF0, 0x80, 0xF0, 0x80, 0x80 // F
    };

    for (int i = 0; i < 80; i++)
        memory[i] = fontSet[i];
}

void Core::loadGame(const char *game) {
    std::ifstream file;
    file.open(game, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << game << std::endl;
        exit(1);
    }

    file.seekg(0, std::ios::end);
    long size = file.tellg();

    if (size > 4096 - 0x200) {
        std::cerr << "Error: ROM too large" << std::endl;
        exit(1);
    }

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(memory + 0x200), size);

    file.close();

    std::cout << "Loaded " << game << " into memory" << std::endl;

    // TODO: remove this
    for (int i = 0; i < size; i++)
        std::cout << std::hex << memory[i + 0x200] << " ";
}

void Core::emulateCycle() {
    fetchOpcode();
    executeOpcode();

    updateTimers();
}

void Core::fetchOpcode() {
    opcode = memory[pc] << 8 | memory[pc + 1];
}

void Core::updateTimers() {
    if (delay_timer > 0)
        delay_timer--;

    if (sound_timer > 0) {
        if (sound_timer == 1)
            // TODO: implement sound
            std::cout << "BEEP!" << std::endl;
        sound_timer--;
    }
}
