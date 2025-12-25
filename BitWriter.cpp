#include "BitWriter.h"

BitWriter::BitWriter(std::string oF) : outputFile_(oF, std::ios::binary) {}

void BitWriter::writeBit(int bit) {
    buffer = buffer << 1;

    if (bit) {
        buffer = buffer | 1;
    }

    bitCount++;
    if (bitCount == 8) {
        outputFile_.put(buffer);
        buffer = 0;
        bitCount = 0;
    }
}

void BitWriter::writeByte(unsigned char byte) {
    outputFile_.write(reinterpret_cast<char*>(&byte), sizeof(byte));
}

void BitWriter::writeShort(unsigned short byte) {
    outputFile_.write(reinterpret_cast<char*>(&byte), sizeof(byte));
}

void BitWriter::writeInt(unsigned int byte) {
    outputFile_.write(reinterpret_cast<char*>(&byte), sizeof(byte));
}

void BitWriter::flush() {
    buffer = buffer << (8 - bitCount);
    outputFile_.put(buffer);
    buffer = 0;
    bitCount = 0;
}

void BitWriter::close() {
    outputFile_.close();
}