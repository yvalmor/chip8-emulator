//
// Created by yvonm on 16/03/2024.
//

#ifndef CHIP_8_EMULATOR_OPCODE_H
#define CHIP_8_EMULATOR_OPCODE_H


class Opcode {
private:
    unsigned short opcode;
    unsigned short nnn;
    unsigned char n;
    unsigned char x;
    unsigned char y;
    unsigned char kk;

public:
    explicit Opcode(unsigned short opcode);

    [[nodiscard]] unsigned short getOpcode() const;
    [[nodiscard]] unsigned char getInstruction() const;
    [[nodiscard]] unsigned short getNnn() const;
    [[nodiscard]] unsigned char getN() const;
    [[nodiscard]] unsigned char getX() const;
    [[nodiscard]] unsigned char getY() const;
    [[nodiscard]] unsigned char getKk() const;
};

#endif//CHIP_8_EMULATOR_OPCODE_H
