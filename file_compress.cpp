#include <set>
#include <map>
#include <algorithm>

#include <fstream>
#include <iostream>
#include <filesystem>

#include "BitWriter.h"

struct Node {
    Node *left_ = nullptr;
    Node *right_ = nullptr;
    char c_ = '\0';
    int freq_;

    Node () {}
    Node(Node *left, Node *right, int freq) : left_(left), right_(right), freq_(freq) {}
    Node(Node *left, Node *right, char c, int freq) : left_(left), right_(right), c_(c), freq_(freq) {}
    Node(char c, int freq) : c_(c), freq_(freq) {}
    bool operator< (const Node &other) const {
        return (freq_ < other.freq_);
    }
};

std::string fileConv (const char *fileName) {
    std::string file;
    int length = 0;
    while (fileName[length] != '\0') {
        file += fileName[length];
        length++;
    }

    return (file);
}

void pathCalc (const char c, Node *root, std::string current_path, std::string &final_path) {
    if (root == nullptr) {
        return;
    }

    if (root->left_ == nullptr && root->right_ == nullptr && root->c_ == c) {
        final_path = current_path;
        return;
    }

    pathCalc(c, root->left_, current_path + "0", final_path);
    pathCalc(c, root->right_, current_path + "1", final_path);
}

void deleteMem (Node *root) {
    if (root == nullptr) {
        return;
    }

    deleteMem(root->left_);
    deleteMem(root->right_);
    delete root;
}

int main (int argc, char* argv[]) {
    if (argc > 3) {
        std::cout << "Improper Executable Formatting" << '\n';
        return (1);
    }

    std::string oF;
    std::string iF = fileConv(argv[1]);
    (argc == 3) ? (oF = fileConv(argv[2])) : (oF = oF);
    std::ifstream inputFile(iF, std::ios::binary);

    // Print input file size
    int input_size = static_cast<int>(std::filesystem::file_size(iF));
    std::cout << "Input file size is " << input_size << " bytes\n";
    std::cout << "Processing input file...\n";


    std::vector <int> frequencies(256);

    char c;
    int count = 0;
    while (inputFile.get(c)) {
        frequencies[static_cast<unsigned char>(c)]++;
        (frequencies[static_cast<unsigned char>(c)] == 1) ? (count++) : (count+=0);
    }

    std::multiset <Node> sortedNodes;

    for (int i = 0; i < 256; i++) {
        if (!frequencies[i]) {
            continue;
        }

        Node adding((char)(i), frequencies[i]);
        sortedNodes.insert(adding);
    }

    while (sortedNodes.size() >= 2) {
        auto it = sortedNodes.begin();
        Node *l = new Node(it->left_, it->right_, it->c_, it->freq_); 
        it = sortedNodes.erase(it);
        Node *r = new Node(it->left_, it->right_, it->c_, it->freq_);
        it = sortedNodes.erase(it);

        Node parent(l, r, '\0', l->freq_ + r->freq_);
        sortedNodes.insert(parent);
    }

    auto it = sortedNodes.begin();
    Node *huffman_root = new Node (it->left_, it->right_, it->c_, it->freq_);

    std::unordered_map <char, std::string> charPath;

    for (int i = 0; i < 256; i++) {
        if (frequencies[i]) {
            std::string path_str;
            pathCalc((char)(i), huffman_root, "", path_str);
            path_str = path_str.substr(0, path_str.length());
            charPath[(char)(i)] = path_str;
        }
    }

    BitWriter bw(oF);
    bw.writeInt(static_cast<unsigned int>(input_size));
    bw.writeByte(static_cast<unsigned char>(count));
    std::cout << "Unique characters " << count << '\n';
    int maxLen = 0;
    
    for (auto it{charPath.begin()}; it != charPath.end(); ++it) {
        char cur = it->first;
        std::string path = it->second;
        int len = path.length();
        maxLen = std::max(maxLen, len);

        bw.writeByte(static_cast<unsigned char>(cur));
        bw.writeByte(static_cast<unsigned char>(len));

        if (len <= 8) {
            bw.writeByte(static_cast<unsigned char>(std::stoi(path, nullptr, 2)));
        }
        else if (len <= 16) {
            bw.writeShort(static_cast<unsigned short>(std::stoi(path, nullptr, 2)));
        }
        else {
            bw.writeInt(static_cast<unsigned int>(std::stoi(path, nullptr, 2)));
        }
    }

    std::cout << "Maximum path length " << maxLen << '\n';

    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);
    
    while (inputFile.get(c)) {
        std::string path = charPath[c];
        int len = path.length();

        for (int i{0}; i < len; i++) {
            bw.writeBit(std::stoi(path.substr(i, 1)));
        }
    }

    bw.flush();
    bw.close();

    // Print output file size
    auto output_size = std::filesystem::file_size(oF);
    std::cout << "Output file size is " << output_size << " bytes\n";
    double compression = (double)(input_size - output_size) / (double)(input_size);
    std::cout << "Compression of " << 100 * compression << "%\n";

    deleteMem(huffman_root);

    return (0);
}
