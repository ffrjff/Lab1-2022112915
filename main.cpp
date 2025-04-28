#include "graph.hpp"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <windows.h>

using namespace std;

int count = 0;
void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void menu()
{
    cout << "-------------Lab1-------------" << endl;
    cout << "1. Show graph and save graph in png" << endl;
    cout << "2. Query bridge words" << endl;
    cout << "3. Generate new text" << endl;
    cout << "4. Calculate all shortest path" << endl;
    cout << "5. Calculate single source shortest paths" << endl;
    cout << "6. Calculate PR value" << endl;
    cout << "7. Random walk" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter your choice: ";
}

void lightMenu()
{
    cout << "1. Again" << endl;
    cout << "2. Return" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        system("cls");
        return 1;
    }
    string filename = argv[1];
    string fileContent = readFileToString(filename);
    if (fileContent.empty())
    {
        cerr << "Error: File is empty or cannot be read." << endl;
        system("cls");
        return 1;
    }

    string processedText = processText(fileContent);
    cout << "Processed text: " << endl
         << processedText << endl;

    Graph graph(processedText);
    graph.graphCreate();

    system("cls");
    int choice;

    while (true)
    {
        menu();
        cin >> choice;
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            while (true)
            {
                system("cls");
                graph.showDirectedGraph();
                cout << "Save graph to file? (y/n): ";
                char saveChoice;
                cin >> saveChoice;
                if(saveChoice == 'y' || saveChoice == 'Y')
                {
                    system("cls");
                    graph.saveGraphToFile("out_graph.png");
                    system("cls");
                    cout << "Success to save graph to out_graph.png" << endl;
                }
                lightMenu();
                int c;
                cin >> c;
                clearInputBuffer();
                if (c == 2)
                {
                    system("cls");
                    break;
                }
                else if (c == 3)
                {
                    system("cls");
                    exit(0);
                }
            }
            break;

        case 2:
            while (true)
            {
                system("cls");
                string word1, word2;
                cout << "Enter two words to query bridge words: ";
                cin >> word1 >> word2;
                clearInputBuffer();
                cout << graph.queryBridgeWords(word1, word2) << endl;
                lightMenu();
                int c;
                cin >> c;
                clearInputBuffer();
                if (c == 2)
                {
                    system("cls");
                    break;
                }
                else if (c == 3)
                {
                    system("cls");
                    exit(0);
                }
            }
            break;

        case 3:
            while (true)
            {
                system("cls");
                string inputText;
                cout << "Enter text to generate new text: ";
                getline(cin, inputText);
                cout << graph.generateNewText(inputText) << endl;
                lightMenu();
                int c;
                cin >> c;
                clearInputBuffer();
                if (c == 2)
                {
                    system("cls");
                    break;
                }
                else if (c == 3)
                {
                    system("cls");
                    exit(0);
                }
            }
            break;

        case 4:
            while (true)
            {
                system("cls");
                string startWord, endWord;
                cout << "Enter two words to calculate shortest path: ";
                cin >> startWord >> endWord;
                clearInputBuffer();
                graph.calcShortestPath(startWord, endWord);
                lightMenu();
                int c;
                cin >> c;
                clearInputBuffer();
                if (c == 2)
                {
                    system("cls");
                    break;
                }
                else if (c == 3)
                {
                    system("cls");
                    exit(0);
                }
            }
            break;

        case 5:
            while (true)
            {
                system("cls");
                string singleSourceWord;
                cout << "Enter a word to calculate single source shortest paths: ";
                cin >> singleSourceWord;
                clearInputBuffer();
                graph.singleSourceShortestPaths(singleSourceWord);
                lightMenu();
                int c;
                cin >> c;
                clearInputBuffer();
                if (c == 2)
                {
                    system("cls");
                    break;
                }
                else if (c == 3)
                {
                    system("cls");
                    exit(0);
                }
            }
            break;

        case 6:
            while (true)
            {
                system("cls");
                string prWord;
                cout << "Enter a word to calculate PR value: ";
                cin >> prWord;
                clearInputBuffer();
                cout << "PR(" << prWord << "): " << graph.calPageRank(prWord) << endl;
                lightMenu();
                int c;
                cin >> c;
                clearInputBuffer();
                if (c == 2)
                {
                    system("cls");
                    break;
                }
                else if (c == 3)
                {
                    system("cls");
                    exit(0);
                }
            }
            break;

        case 7:
            while (true)
            {
                system("cls");
                cout << graph.randomWalk() << endl;
                lightMenu();
                int c;
                cin >> c;
                clearInputBuffer();
                if (c == 2)
                {
                    system("cls");
                    break;
                }
                else if (c == 3)
                {
                    system("cls");
                    exit(0);
                }
            }
            break;
        case 8:
            exit(0);
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
