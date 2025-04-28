#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>

class Graph {
private:
    std::string text;
    int numVertex;
    std::vector<std::vector<int>> weight; // 记录权重
    std::map<std::string, int> vertexMap; // 顶点到索引的映射
    std::vector<std::string> vertices;
public:
    Graph(const std::string& inputText);
    void graphCreate();
    void showDirectedGraph();
    std::string queryBridgeWords(std::string word1, std::string word2);
    std::vector<std::string> lightQueryBridgeWords(std::string word1, std::string word2);
    std::string generateNewText(std::string inputText);
    std::vector<std::vector<std::string>> calcShortestPath(std::string word1, std::string word2);
    void singleSourceShortestPaths(const std::string &start);
    double calPageRank(std::string word);
    std::string randomWalk();
    void saveGraphToFile(const std::string& filename);
};


std::string processText(const std::string& input);
std::string readFileToString(const std::string& filePath);


#endif