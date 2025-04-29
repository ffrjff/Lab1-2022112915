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
    transform(word1.begin(), word1.end(), word1.begin(), ::tolower);
    transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
    if (vertexMap.find(word1) == vertexMap.end() || vertexMap.find(word2) == vertexMap.end())
    {
        return "No " + word1 + " or " + word2 + " in the graph!";
    }
    int index1 = vertexMap[word1];
    int index2 = vertexMap[word2];
    vector<string> bridgeWords;
    for (int i = 0; i < numVertex; ++i)
    {
        if (weight[index1][i] > 0 && weight[i][index2] > 0)
        {
            bridgeWords.push_back(vertices[i]);
        }
    }
    if (bridgeWords.empty())
    {
        return "No bridge words from " + word1 + " to " + word2 + "!";
    }
    else if (bridgeWords.size() == 1)
    {
        return "The bridge word from " + word1 + " to " + word2 + " is: " + bridgeWords[0] + ".";
    }
    else
    {
        string result = "The bridge words from " + word1 + " to " + word2 + " are: ";
        for (size_t i = 0; i < bridgeWords.size(); ++i)
        {
            if (i == bridgeWords.size() - 1)
            {
                result += "and " + bridgeWords[i] + ".";
            }
            else if (i == bridgeWords.size() - 2)
            {
                result += bridgeWords[i] + " ";
            }
            else
            {
                result += bridgeWords[i] + ", ";
            }
        }
        return result;
    }
}

vector<string> Graph::lightQueryBridgeWords(string word1, string word2)
{
    vector<string> bridgeWords;
    transform(word1.begin(), word1.end(), word1.begin(), ::tolower);
    transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
    if (vertexMap.find(word1) == vertexMap.end() || vertexMap.find(word2) == vertexMap.end())
    {
        return bridgeWords;
    }
    int index1 = vertexMap[word1];
    int index2 = vertexMap[word2];
    for (int i = 0; i < numVertex; ++i)
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
        string result = "The bridge words from " + word1 + " to " + word2 + " are: ";
        for (size_t i = 0; i < bridgeWords.size(); ++i)
        {
            if (i == bridgeWords.size() - 1)
            {
                result += "and " + bridgeWords[i] + ".";
            }
            else if (i == bridgeWords.size() - 2)
            {
                result += bridgeWords[i] + " ";
            }
            else
            {
                result += bridgeWords[i] + ", ";
            }
        }
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
    transform(word1.begin(), word1.end(), word1.begin(), ::tolower);
    transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
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

double Graph::calPageRank(string word) {
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    if (vertexMap.find(word) == vertexMap.end()) {
        cerr << "Word not found in the graph.\n";
        return -1.0;
    }
    const double dampingFactor = 0.85;
    const double tolerance = 1e-6;
    int iterations = 100;
    int wordIndex = vertexMap[word];
    int n = numVertex;
    // TF-IDF
    vector<double> pr(n, 0.0);
    double totalImportance = 0.0;
    for (int i = 0; i < n; ++i) {
        double tf = accumulate(weight[i].begin(), weight[i].end(), 0.0);
        double idf = log((double)n / (1 + weight[i][i]));
        pr[i] = tf * idf;  // TF-IDF
        totalImportance += pr[i];
    }
    for (int i = 0; i < n; ++i) {
        pr[i] /= totalImportance;
    }
    vector<double> newPr(n, 0.0);
    vector<int> outDegrees(n, 0);
    for (int i = 0; i < n; ++i) {
        outDegrees[i] = accumulate(weight[i].begin(), weight[i].end(), 0);
    }
    for (int iter = 0; iter < iterations; ++iter) {
        fill(newPr.begin(), newPr.end(), (1.0 - dampingFactor) / n);
        for (int i = 0; i < n; ++i) {
            if (outDegrees[i] == 0) {
                for (int j = 0; j < n; ++j) {
                    newPr[j] += dampingFactor * pr[i] / n;
                }
            } else {
                for (int j = 0; j < n; ++j) {
                    if (weight[i][j] > 0) {
                        newPr[j] += dampingFactor * pr[i] * weight[i][j] / outDegrees[i];
                    }
                }
            }
        }
        double diff = 0.0;
        for (int i = 0; i < n; ++i) {
            diff += fabs(newPr[i] - pr[i]);
        }
        pr.swap(newPr);
        if (diff < tolerance) break;
    }
    return pr[wordIndex];
}

string Graph::randomWalk()
{
    if (vertices.empty())
    {
        return "Graph is empty.";
    }
    random_device rd;
    mt19937 gen(static_cast<unsigned>(time(nullptr)));
    uniform_int_distribution<> distrib(0, numVertex * 100);
    int startIndex = distrib(gen) % numVertex;
    cout << "start index: " << startIndex << endl;
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

void Graph::saveGraphToFile(const string &filename)
{
    ofstream dotFile("graph.dot");
    dotFile << "digraph G {\n";
    dotFile << "  rankdir=LR;\n";
    for (const auto &vertex : vertices)
    {
        dotFile << "  \"" << vertex << "\";\n";
    }
    for (int i = 0; i < numVertex; ++i)
    {
        for (int j = 0; j < numVertex; ++j)
        {
            if (weight[i][j] > 0)
            {
                dotFile << "  \"" << vertices[i] << "\" -> \"" << vertices[j]
                        << "\" [label=\"" << weight[i][j] << "\"];\n";
            }
        }
    }
    dotFile << "}\n";
    dotFile.close();
    string cmd = "dot -Tpng graph.dot -o " + filename;
    system(cmd.c_str());
    remove("graph.dot");
    cout << "Graph saved to " << filename << endl;
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