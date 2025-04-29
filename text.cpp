#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;


string processText(const string& input) {
    string processed;
    for (size_t i = 0; i < input.size(); ++i) {
        unsigned char ch = input[i];
        if (ch == '\n' || ch == '\r') {
            processed += ' ';
            continue;
        }
        if (isalpha(ch)) {
            processed += tolower(ch);
            continue;
        }
        // 处理破折号 `—`
        if (ch == 0xE2 && i + 2 < input.size() && 
            static_cast<unsigned char>(input[i+1]) == 0x80 && 
            static_cast<unsigned char>(input[i+2]) == 0x94) {
            processed += ' ';
            i += 2;
            continue;
        }
        if (ispunct(ch)) {
            processed += ' ';
            continue;
        }
        if (ch == ' ') {
            processed += ' ';
        }
    }
    string final_processed;
    bool prev_space = false;
    for (char ch : processed) {
        if (ch == ' ') {
            if (!prev_space) {
                final_processed += ' ';
                prev_space = true;
            }
        } else {
            final_processed += ch;
            prev_space = false;
        }
    }
    ofstream out_file("output.txt");
    if (out_file.is_open()) {
        out_file << final_processed;
        out_file.close();
    } else {
        cerr << "无法打开 output.txt 进行写入！" << endl;
    }
    
    return final_processed;
}

string readFileToString(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << filePath << "'" << endl;
        return "";
    }
    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, ios::beg);
    string content;
    content.resize(fileSize);
    file.read(&content[0], fileSize);
    return content;
}