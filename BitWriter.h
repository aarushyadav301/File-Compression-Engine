#include <fstream>

class BitWriter {
    public:
        BitWriter(std::string oF);
        void writeBit(int bit);
        void writeByte(unsigned char byte);
        void writeShort(unsigned short byte);
        void writeInt(unsigned int byte);
        void flush();
        void close();
    private:
        std::ofstream outputFile_;
        const int bufferSize = 8;
        unsigned char buffer;
        int bitCount = 0;
};