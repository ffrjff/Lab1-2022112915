#include "graph.hpp"
#include <iostream>
#include <set>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <climits>
#include <algorithm>
#include <functional>
#include <cmath>
#include <unordered_set>
#include <utility>
#include <random>


using namespace std;

bool initWords(int &index, string &preWord, string &curWord, const string &text);
bool readWord(int &index, string &word, const string &text);
vector<string> getUniqueWords(const string &text);

void Graph::graphCreate()
{
    string preWord, curWord;
    int index = 0;
    if (!initWords(index, preWord, curWord, text))
    {
        exit(0);
    }
    while (1)
    {
        weight[vertexMap[preWord]][vertexMap[curWord]]++;
        preWord = curWord;
        if (!readWord(index, curWord, text))
        {
            cout << "success to build graph" << endl;
            return;
        }
        // cout << "New pair: " << preWord << " " << curWord << endl;
    }
}

void Graph::showDirectedGraph()
{
    cout << "Directed Graph:\n";
    cout << "text: " << text << endl;
    for (int i = 0; i < numVertex; i++)
    {
        cout << vertices[i] << " -> ";
        bool first = true;
        for (int j = 0; j < numVertex; j++)
        {
            if (weight[i][j] != 0)
            {
                if (!first)
                    cout << ", ";
                cout << vertices[j] << "(weight: " << weight[i][j] << ")";
                first = false;
            }
        }
        cout << endl;
    }
}

string Graph::queryBridgeWords(string word1, string word2)
{
    if (vertexMap.find(word1) == vertexMap.end())
    {
        return "No " + word1 + " in the graph!";
    }
    else if (vertexMap.find(word2) == vertexMap.end())
    {
        return "No " + word2 + " in the graph!";
    }
    int index1 = vertexMap[word1];
    int index2 = vertexMap[word2];
    vector<string> bridgeWords;
    for (int i = 0; i < vertices.size(); ++i)
    {
        if (weight[index1][i] > 0 && weight[i][index2] > 0)
        {
            bridgeWords.push_back(vertices[i]);
        }
    }
    if (bridgeWords.empty())
    {
        return "No bridge words from \"" + word1 + "\" to \"" + word2 + "\"!";
    }
    else
    {
        string result = "The bridge words from " + word1 + " to " + word2 + " is: \"";
        for (int i = 0; i < bridgeWords.size(); ++i)
        {
            result += bridgeWords[i];
            if (i < bridgeWords.size() - 1)
            {
                result += ", ";
            }
            else if (bridgeWords.size() > 1)
            {
                result += " and ";
            }
        }
        result += "\"";
        return result;
    }
}

vector<string> Graph::lightQueryBridgeWords(string word1, string word2)
{
    vector<string> bridgeWords;
    if (vertexMap.find(word1) == vertexMap.end())
    {
        return bridgeWords;
    }
    else if (vertexMap.find(word2) == vertexMap.end())
    {
        return bridgeWords;
    }
    int index1 = vertexMap[word1];
    int index2 = vertexMap[word2];
    for (int i = 0; i < vertices.size(); ++i)
    {
        if (weight[index1][i] > 0 && weight[i][index2] > 0)
        {
            bridgeWords.push_back(vertices[i]);
        }
    }
    if (bridgeWords.empty())
    {
        return bridgeWords;
    }
    else
    {
        string result = "The bridge words from " + word1 + " to " + word2 + " is: \"";
        for (int i = 0; i < bridgeWords.size(); ++i)
        {
            result += bridgeWords[i];
            if (i < bridgeWords.size() - 1)
            {
                result += ", ";
            }
            else if (bridgeWords.size() > 1)
            {
                result += " and ";
            }
        }
        result += "\"";
        return bridgeWords;
    }
}

string Graph::generateNewText(string inputText)
{
    int index = 0;
    string preWord, curWord;
    initWords(index, preWord, curWord, inputText);
    string newText;
    vector<string> bridgeWords;
    newText += preWord;
    while (1)
    {
        bridgeWords = lightQueryBridgeWords(preWord, curWord);
        if (bridgeWords.empty())
        {
            newText += " " + curWord;
        }
        else
        {
            srand(static_cast<unsigned>(time(nullptr)));
            int randomIndex = rand() % bridgeWords.size();
            newText += " " + bridgeWords[randomIndex] + " " + curWord;
        }
        preWord = curWord;
        if (!readWord(index, curWord, inputText))
        {
            cout << "success to create new text" << endl;
            return newText;
        }
    }
}

vector<vector<string>> Graph::calcShortestPath(string word1, string word2)
{
    if (vertexMap.find(word1) == vertexMap.end())
    {
        cout << "Word \"" << word1 << "\" is not in the graph!" << endl;
        return {};
    }
    if (vertexMap.find(word2) == vertexMap.end())
    {
        cout << "Word \"" << word2 << "\" is not in the graph!" << endl;
        return {};
    }
    int startIndex = vertexMap[word1];
    int endIndex = vertexMap[word2];
    int n = vertices.size();
    vector<int> dist(n, INT_MAX);
    vector<vector<int>> predecessors(n);
    vector<bool> visited(n, false);
    dist[startIndex] = 0;
    using P = pair<int, int>;
    priority_queue<P, vector<P>, greater<>> pq;
    pq.push({0, startIndex});
    while (!pq.empty())
    {
        int currDist = pq.top().first;
        int currIndex = pq.top().second;
        pq.pop();
        if (visited[currIndex])
            continue;
        visited[currIndex] = true;
        for (int i = 0; i < n; ++i)
        {
            if (weight[currIndex][i] > 0 && !visited[i])
            {
                int newDist = currDist + weight[currIndex][i];
                if (newDist < dist[i])
                {
                    dist[i] = newDist;
                    predecessors[i].clear();
                    predecessors[i].push_back(currIndex);
                    pq.push({newDist, i});
                }
                else if (newDist == dist[i])
                {
                    predecessors[i].push_back(currIndex);
                }
            }
        }
    }
    if (dist[endIndex] == INT_MAX)
    {
        cout << "No path from \"" << word1 << "\" to \"" << word2 << "\"!" << endl;
        return {};
    }
    vector<vector<string>> allPaths;
    vector<int> currentPath;
    function<void(int)> backtrack = [&](int node)
    {
        currentPath.push_back(node);
        if (node == startIndex)
        {
            vector<string> path;
            for (int i = currentPath.size() - 1; i >= 0; --i)
            {
                path.push_back(vertices[currentPath[i]]);
            }
            allPaths.push_back(path);
        }
        else
        {
            for (int prev : predecessors[node])
            {
                backtrack(prev);
            }
        }
        currentPath.pop_back();
    };
    backtrack(endIndex);
    cout << "All shortest paths from \"" << word1 << "\" to \"" << word2 << "\":\n";
    for (const auto &path : allPaths)
    {
        for (size_t i = 0; i < path.size(); ++i)
        {
            cout << path[i];
            if (i < path.size() - 1)
            {
                cout << " -> ";
            }
        }
        cout << " (Length: " << dist[endIndex] << ")\n";
    }
    return allPaths;
}

void Graph::singleSourceShortestPaths(const string &start)
{
    if (vertexMap.find(start) == vertexMap.end())
    {
        cout << "Word \"" << start << "\" is not in the graph!" << endl;
        return;
    }
    int n = vertices.size();
    int startIndex = vertexMap[start];
    for (int i = 0; i < n; ++i)
    {
        if (i == startIndex)
            continue;
        calcShortestPath(start, vertices[i]);
    }
}

// double Graph::calPageRank(string word)
// {
//     if (vertexMap.find(word) == vertexMap.end())
//     {
//         cout << "Word \"" << word << "\" is not in the graph!" << endl;
//         return -1;
//     }

//     int N = numVertex;             // 图的节点总数
//     double d = 0.85;               // 阻尼因子
//     double epsilon = 1e-6;         // 收敛精度
//     vector<double> PR(N, 1.0 / N); // 初始PR值均为1/N
//     vector<double> PR_new(N, 0.0); // 存储更新后的PR值

//     // 获取目标单词的索引
//     int wordIndex = vertexMap[word];

//     // 迭代计算PageRank值，直到收敛
//     bool converged = false;
//     while (!converged)
//     {
//         converged = true;

//         // 计算每个节点的新PR值
//         for (int u = 0; u < N; ++u)
//         {
//             PR_new[u] = (1 - d) / N; // PR(u) = (1 - d) / N
//             for (int v = 0; v < N; ++v)
//             {
//                 if (weight[v][u] > 0)
//                 { // 如果v指向u
//                     int L_v = 0;
//                     for (int k = 0; k < N; ++k)
//                     {
//                         if (weight[v][k] > 0)
//                         {
//                             L_v++; // 计算v的出度
//                         }
//                     }
//                     PR_new[u] += d * PR[v] / L_v; // PR(u) += d * PR(v) / L(v)
//                 }
//             }
//         }
//         converged = true;
//         for (int i = 0; i < N; ++i)
//         {
//             if (fabs(PR_new[i] - PR[i]) > epsilon)
//             { // 使用fabs计算浮点数的绝对值
//                 converged = false;
//                 break;
//             }
//         }
//         PR = PR_new;
//     }
//     return PR[wordIndex];
// }

double Graph::calPageRank(std::string word) {
    // 检查顶点是否存在
    if (vertexMap.find(word) == vertexMap.end()) {
        return -1.0;  // 顶点不存在返回-1
    }

    // PageRank算法参数
    const double dampingFactor = 0.85;  // 阻尼系数
    const double tolerance = 1e-6;      // 收敛阈值
    const int maxIterations = 100;      // 最大迭代次数

    // 初始化PR值
    std::vector<double> prValues(numVertex, 1.0 / numVertex);
    std::vector<double> tempPrValues(numVertex);

    // 迭代计算
    for (int iter = 0; iter < maxIterations; ++iter) {
        double diff = 0.0;
        tempPrValues = prValues;

        // 计算每个顶点的PR值
        for (int i = 0; i < numVertex; ++i) {
            double sum = 0.0;

            // 计算所有入链贡献
            for (int j = 0; j < numVertex; ++j) {
                if (weight[j][i] > 0) {  // 存在从j到i的边
                    int outDegree = 0;
                    for (int k = 0; k < numVertex; ++k) {
                        if (weight[j][k] > 0) outDegree++;
                    }
                    if (outDegree > 0) {
                        sum += tempPrValues[j] / outDegree;
                    }
                }
            }

            // 更新PR值
            prValues[i] = (1 - dampingFactor) / numVertex + dampingFactor * sum;
            diff += std::abs(prValues[i] - tempPrValues[i]);
        }

        // 检查收敛
        if (diff < tolerance) {
            break;
        }
    }

    // 返回指定顶点的PR值
    return prValues[vertexMap[word]];
}

string Graph::randomWalk()
{
    if (vertices.empty())
    {
        return "Graph is empty.";
    }
    std::random_device rd;
    std::mt19937 gen(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<> distrib(0, numVertex * 100);
    int startIndex = distrib(gen) % numVertex;
    cout << "start index: " << startIndex << endl;
    // srand(time(nullptr));
    // int startIndex = rand() % numVertex;
    string startNode = vertices[startIndex];
    vector<vector<bool>> visitedEdges(numVertex, vector<bool>(numVertex, false));
    vector<string> path;
    path.push_back(startNode);
    int currentIndex = startIndex;
    
    while (true)
    {
        vector<int> neighbors;
        for (int i = 0; i < numVertex; ++i)
        {
            if (weight[currentIndex][i] > 0)
            {
                neighbors.push_back(i);
            }
        }
        if (neighbors.empty())
        {
            break;
        }
        int nextIndex = neighbors[rand() % neighbors.size()];
        if (visitedEdges[currentIndex][nextIndex])
        {
            path.push_back(vertices[nextIndex]);
            break;
        }
        visitedEdges[currentIndex][nextIndex] = true;
        path.push_back(vertices[nextIndex]);
        currentIndex = nextIndex;
    }
    
    string result;
    for (const auto &node : path)
    {
        result += node + " ";
    }
    if (!result.empty())
    {
        result.pop_back();
    }
    
    ofstream outFile("random_walk.txt", ios::app);
    if (outFile.is_open())
    {
        outFile << result << endl;
        outFile.close();
        cout << "Random walk path appended to 'random_walk.txt'." << endl;
    }
    else
    {
        cerr << "Error: Unable to save to file." << endl;
    }
    
    return result;
}

Graph::Graph(const string &inputText)
{
    text = inputText;
    vector<string> words = getUniqueWords(inputText);
    numVertex = words.size();
    vertices.resize(numVertex);
    for (int i = 0; i < numVertex; ++i)
    {
        vertexMap[words[i]] = i;
        vertices[i] = words[i];
    }
    weight.resize(numVertex, vector<int>(numVertex, 0));
}

void Graph::saveGraphToFile(const std::string& filename) {
    std::ofstream dotFile("graph.dot");
    dotFile << "digraph G {\n";
    dotFile << "  rankdir=LR;\n"; // 从左到右布局
    
    // 添加所有节点
    for (const auto& vertex : vertices) {
        dotFile << "  \"" << vertex << "\";\n";
    }
    
    // 添加所有边
    for (int i = 0; i < numVertex; ++i) {
        for (int j = 0; j < numVertex; ++j) {
            if (weight[i][j] > 0) {
                dotFile << "  \"" << vertices[i] << "\" -> \"" << vertices[j] 
                       << "\" [label=\"" << weight[i][j] << "\"];\n";
            }
        }
    }
    
    dotFile << "}\n";
    dotFile.close();
    
    // 调用Graphviz生成图片
    std::string cmd = "dot -Tpng graph.dot -o " + filename;
    system(cmd.c_str());
    
    // 可选：删除临时dot文件
    remove("graph.dot");
    
    std::cout << "Graph saved to " << filename << std::endl;
}

bool readWord(int &index, string &word, const string &text)
{
    word.clear();
    if (index >= text.size())
    {
        cout << "no more word" << endl;
        return false;
    }
    while (index < text.size() && text[index] != ' ')
    {
        word += text[index];
        index++;
    }
    index++;
    return true;
}

bool initWords(int &index, string &preWord, string &curWord, const string &text)
{
    if (readWord(index, preWord, text))
    {
        // cout << "first word: " << preWord << endl;
    }
    else
    {
        cout << "text is empty" << endl;
        return false;
    }
    if (readWord(index, curWord, text))
    {
        // cout << "second word: " << curWord << endl;
    }
    else
    {
        cout << "only one word in text" << endl;
        return false;
    }
    return true;
}

vector<string> getUniqueWords(const string &text)
{
    istringstream stream(text);
    string word;
    set<string> uniqueWords;
    while (stream >> word)
    {
        uniqueWords.insert(word);
    }
    return vector<string>(uniqueWords.begin(), uniqueWords.end());
}

// int main() {
//     string inputText = "to explore strange new worlds to seek out new life and new civilizations";
//     Graph graph(inputText);
//     graph.graphCreate();
//     graph.showDirectedGraph();
//     return 0;
// }