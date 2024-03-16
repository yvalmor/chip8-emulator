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

void Core::executeOpcode() {
    auto decodedOpcode = Opcode(opcode);

    switch (decodedOpcode.getInstruction()) {
        case 0x0000:
            // TODO: implement
            break;

        case 0x1000:
            pc = decodedOpcode.getNnn();
            break;

        case 0x2000:
            stack[sp] = pc;
            sp++;
            pc = decodedOpcode.getNnn();
            break;

        case 0x3000:
            if (V[decodedOpcode.getX()] == decodedOpcode.getKk())
                pc += 2;
            break;

        case 0x4000:
            if (V[decodedOpcode.getX()] != decodedOpcode.getKk())
                pc += 2;
            break;

        case 0x5000:
            if (V[decodedOpcode.getX()] == V[decodedOpcode.getY()])
                pc += 2;
            break;

        case 0x6000:
            V[decodedOpcode.getX()] = decodedOpcode.getKk();
            break;

        case 0x7000:
            V[decodedOpcode.getX()] += decodedOpcode.getKk();
            break;

        case 0x8000:
            // TODO: implement
            break;

        case 0x9000:
            if (V[decodedOpcode.getX()] != V[decodedOpcode.getY()])
                pc += 2;
            break;

        case 0xA000:
            I = decodedOpcode.getNnn();
            break;

        case 0xB000:
            pc = V[0] + decodedOpcode.getNnn();
            break;

        case 0xC000:
            V[decodedOpcode.getX()] = (random() % 255) & decodedOpcode.getKk();
            break;

        case 0xD000:
            display(decodedOpcode);
            break;

        case 0xE000:
            // TODO: implement (kbd conditions)
            break;

        case 0xF000:
            // TODO: implement (kbd actions)
            break;

        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            exit(1);
    }
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
