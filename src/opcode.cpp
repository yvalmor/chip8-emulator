//
// Created by yvonm on 16/03/2024.
//

#include "opcode.h"

Opcode::Opcode(unsigned short opcode) {
    this->opcode = opcode;
    nnn = opcode & 0x0FFF;
    n = opcode & 0x000F;
    x = (opcode & 0x0F00) >> 8;
    y = (opcode & 0x00F0) >> 4;
    kk = opcode & 0x00FF;
}

unsigned short Opcode::getOpcode() const {
    return opcode;
}

unsigned char Opcode::getInstruction() const {
    return opcode & 0xF000;
}

unsigned short Opcode::getNnn() const {
    return nnn;
}

unsigned char Opcode::getN() const {
    return n;
}

unsigned char Opcode::getX() const {
    return x;
}

unsigned char Opcode::getY() const {
    return y;
}

unsigned char Opcode::getKk() const {
    return kk;
}
