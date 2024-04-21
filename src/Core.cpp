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

    clearDisplay();

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
}

void Core::emulateCycle() {
    fetchOpcode();
    executeOpcode();

    updateTimers();
}

void Core::drawGraphics() {
    // TODO: link to graphics
}

void Core::setKeys() {
    // TODO: link to input
}

void Core::fetchOpcode() {
    opcode = memory[pc] << 8 | memory[pc + 1];
}

void Core::executeOpcode() {
    auto decodedOpcode = Opcode(opcode);

    switch (decodedOpcode.getOpcode() & 0xF000) {
        case 0x0000:
            executeOpcodeType0(decodedOpcode);
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
                pc++;
            break;

        case 0x4000:
            if (V[decodedOpcode.getX()] != decodedOpcode.getKk())
                pc++;
            break;

        case 0x5000:
            if (V[decodedOpcode.getX()] == V[decodedOpcode.getY()])
                pc++;
            break;

        case 0x6000:
            V[decodedOpcode.getX()] = decodedOpcode.getKk();
            break;

        case 0x7000:
            V[decodedOpcode.getX()] += decodedOpcode.getKk();
            break;

        case 0x8000:
            executeOpcodeType8(decodedOpcode);
            break;

        case 0x9000:
            if (V[decodedOpcode.getX()] != V[decodedOpcode.getY()])
                pc++;
            break;

        case 0xA000:
            I = decodedOpcode.getNnn();
            break;

        case 0xB000:
            pc = V[0] + decodedOpcode.getNnn();
            break;

        case 0xC000:
            V[decodedOpcode.getX()] = (rand() % 255) & decodedOpcode.getKk();
            break;

        case 0xD000:
            display(decodedOpcode);
            break;

        case 0xE000:
            executeOpcodeTypeE(decodedOpcode);
            break;

        case 0xF000:
            executeOpcodeTypeF(decodedOpcode);
            break;

        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            exit(1);
    }

    pc++;
}

void Core::executeOpcodeType0(Opcode decodedOpcode) {
    switch (decodedOpcode.getOpcode() & 0x00FF) {
        case 0x00E0:
            clearDisplay();
            break;

        case 0x00EE:
            pc = stack[sp];
            sp--;
            break;

        default:
            pc = decodedOpcode.getNnn();
            break;
    }
}

void Core::executeOpcodeType8(Opcode decodedOpcode) {
    switch (decodedOpcode.getOpcode() & 0xF00F) {
        case 0x8000:
            V[decodedOpcode.getX()] = V[decodedOpcode.getY()];
            break;

        case 0x8001:
            V[decodedOpcode.getX()] |= V[decodedOpcode.getY()];
            break;

        case 0x8002:
            V[decodedOpcode.getX()] &= V[decodedOpcode.getY()];
            break;

        case 0x8003:
            V[decodedOpcode.getX()] ^= V[decodedOpcode.getY()];
            break;

        case 0x8004:
            V[0xF] = ((unsigned int) V[decodedOpcode.getX()] + V[decodedOpcode.getY()] > 255);
            V[decodedOpcode.getX()] += V[decodedOpcode.getY()];
            break;

        case 0x8005:
            V[0xF] = (V[decodedOpcode.getX()] > V[decodedOpcode.getY()]);
            V[decodedOpcode.getX()] -= V[decodedOpcode.getY()];
            break;

        case 0x8006:
            V[0xF] = V[decodedOpcode.getX()] & 0x1;
            V[decodedOpcode.getX()] >>= 1;
            break;

        case 0x8007:
            V[0xF] = (V[decodedOpcode.getY()] > V[decodedOpcode.getX()]);
            V[decodedOpcode.getX()] = V[decodedOpcode.getY()] - V[decodedOpcode.getX()];
            break;

        case 0x800E:
            V[0xF] = V[decodedOpcode.getX()] >> 7;
            V[decodedOpcode.getX()] <<= 1;
            break;

        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            exit(1);
    }
}

void Core::executeOpcodeTypeE(Opcode decodedOpcode) {
    switch (decodedOpcode.getOpcode() & 0xF0FF) {
        case 0XE09E:
            if (key[V[decodedOpcode.getX()]])
                pc++;
            break;

        case 0XE0A1:
            if (!key[V[decodedOpcode.getX()]])
                pc++;
            break;

        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            exit(1);
    }
}

void Core::executeOpcodeTypeF(Opcode decodedOpcode) {
    auto opcodeX = decodedOpcode.getX();

    switch (decodedOpcode.getOpcode() & 0xF0FF) {
        case 0xF007:
            V[opcodeX] = delay_timer;
            break;

        case 0xF00A:
            // TODO: implement wait until key press
            break;

        case 0xF015:
            delay_timer = V[opcodeX];
            break;

        case 0xF018:
            sound_timer = V[opcodeX];
            break;

        case 0xF01E:
            I += V[opcodeX];
            break;

        case 0xF029:
            I = V[opcodeX] * 5;
            break;

        case 0xF033:
            memory[I] = V[opcodeX] / 100;
            memory[I + 1] = (V[opcodeX] / 10) % 10;
            memory[I + 2] = V[opcodeX] % 10;
            break;

        case 0xF055:
            for (short index = 0; index < opcodeX; index++)
                memory[I + index] = V[index];
            break;

        case 0xF065:
            for (short index = 0; index < opcodeX; index++)
                V[I + index] = memory[index];
            break;
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

void Core::display(Opcode decodedOpcode) {
    unsigned char x = V[decodedOpcode.getX()];
    unsigned char y = V[decodedOpcode.getY()];
    unsigned char height = decodedOpcode.getN();
    unsigned char pixel;

    V[0xF] = 0;

    for (unsigned char yline = 0; yline < height; yline++) {
        pixel = memory[I + yline];
        for (unsigned char xline = 0; xline < 8; xline++) {
            if ((pixel & (0x80 >> xline)) != 0) {
                if (gfx[(x + xline + ((y + yline) * 64))] == 1)
                    V[0xF] = 1;
                gfx[x + xline + ((y + yline) * 64)] ^= 1;
            }
        }
    }

    drawFlag = true;
}

void Core::clearDisplay() {
    for (unsigned char &i: gfx)
        i = false;
}
