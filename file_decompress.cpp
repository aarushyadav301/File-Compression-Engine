#include <fstream>
#include <iostream>
#include <bitset>
#include <string>
#include <map>

#include "BitWriter.h"

int main (int argc, char* argv[]) { 
    if (!(argc == 3)) {
        std::cout << "Improper File Input Format";
        return (1);
    }

    std::string iF = argv[1];
    std::string oF = argv[2];

    std::ifstream inputFile(iF, std::ios::binary);
    std::ofstream outputFile(oF);

    unsigned int input_size;
    inputFile.read(reinterpret_cast<char*>(&input_size), sizeof(input_size));

    char c;
    int unique_characters;
    inputFile.get(c);
    unique_characters = static_cast<int>(static_cast<unsigned char>(c));
    int type = 0;
    int length;

    std::unordered_map <std::string, char> pathChars;
    char currentChar;

    while (unique_characters > 0) {
        if (type == 0) {
            inputFile.get(c);
            currentChar = c;
        }
        else if (type == 1) {
            inputFile.get(c);
            length = static_cast<int>(static_cast<unsigned char>(c));
        }
        else {
            if (length <= 8) {
                inputFile.get(c);
                std::string path = std::bitset<8>(static_cast<unsigned char>(c)).to_string();
                path = path.substr(8 - length);
                pathChars[path] = currentChar;
            }
            else if (length <= 16) {
                unsigned short c_s;
                inputFile.read(reinterpret_cast<char*>(&c_s), sizeof(c_s));
                std::string path = std::bitset<16>(c_s).to_string();
                path = path.substr(16 - length);
                pathChars[path] = currentChar;
            }   
            else {
                unsigned int c_i;
                inputFile.read(reinterpret_cast<char*>(&c_i), sizeof(c_i));
                std::string path = std::bitset<32>(c_i).to_string();
                path = path.substr(32 - length);
                pathChars[path] = currentChar;
            }
        }

        type = (++type) % 3;
        (type == 0) ? (unique_characters--) : (unique_characters += 0);
    }

    BitWriter bw(oF);
    std::string building = "";
    int count = 0;

    std::cout << "BEFORE SECOND WHILE LOOP\n";

    while (inputFile.get(c)) {
        std::string cur = std::bitset<8>(c).to_string();
        for (int i = 0; i < cur.length(); i++) {
            building += cur.substr(i, 1);
            if (pathChars.count(building)) {
                bw.writeByte(static_cast<unsigned char>(pathChars[building]));
                count++;
                if (count == input_size) {
                    break;
                }
                building = "";
            }
        }

        if (count == input_size) {
            break;
        }
    }
}