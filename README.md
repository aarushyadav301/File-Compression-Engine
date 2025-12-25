# Huffman File Compression Engine

A Huffman file compression engine that takes in files, compresses them into a binary format, and decompresses the binary files back to their original format **without any loss or corruption** of the original data.

## How it works

The compression process relies on a Huffman binary tree built from character frequencies:

- Characters that occur **more frequently** are placed closer to the root.
- Characters that occur **less frequently** are placed farther from the root.
- Instead of storing full bytes for each character, the compressor stores the *path* to a character’s leaf node (a bitstring of `0`s and `1`s), which reduces space usage.

## Project structure

- `file_compress.cpp`: Main Huffman compression logic
- `file_decompress.cpp`: Main Huffman decompression logic
- `BitWriter.h` / `BitWriter.cpp`: Helper class for bit writing to files

## Requirements

- A C++17+ compiler (e.g., `g++`)
- `std::filesystem` support

## Build

### Compile the compressor
