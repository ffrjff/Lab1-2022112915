#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;


string processText(const string& input) {
    string processed;
    processed.reserve(input.size());
    for (char c : input) {
        if (isalpha(c)) {
            processed += std::tolower(c);
        } else if (c == '\n' || c == '\r' || ispunct(c) || isspace(c)) {
            if (!processed.empty() && processed.back() != ' ') {
                processed += ' ';
            }
        }
    }
    if (!processed.empty() && processed.back() == ' ') {
        processed.pop_back();
    }
    return processed;
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

// int main(int argc, char* argv[]) {
//     if (argc != 2) {
//         cerr << "Usage: " << argv[0] << " <filename>" << endl;
//         return 1;
//     }
//     string filename = argv[1];
//     string fileContent = readFileToString(filename);
//     if (!fileContent.empty()) {
//         string processedText = processText(fileContent);
//         cout << "Processed text:" << endl;
//         cout << processedText << endl;
//     }
//     return 0;
// }