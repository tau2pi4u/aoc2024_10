// aoc2024_10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

#include <chrono>
struct Timer
{
    using time_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

    void Begin()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    void End(const char* s, int itrs = 1)
    {
        time_t stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        total += duration;
        printf("%s: %llu us\n", s, duration / itrs);
    }

    void PrintTotal()
    {
        printf("Total: %llu us\n", total);
    }

    time_t start;
    size_t total = 0;
};


struct Node
{
    Node(int height) : height(height) {}

    void Connect(Node* other)
    {
        if (!other) return;
        if (other->height != height + 1) return;
        next.push_back(other);
    }

    int tag;
    int height;
    std::vector<Node*> next;
};

struct Graph
{
    size_t xLim;
    size_t yLim;

    Node* GetNode(size_t y, size_t x)
    {
        if (y < 0 || x < 0 || y >= yLim || x >= xLim)
        {
            return nullptr;
        }
        return nodes[y][x].get();
    }

    std::vector<Node*> trailHeads;
    std::vector<std::vector<std::unique_ptr<Node>>> nodes;
};

Graph ReadInput(std::ifstream& ifstream, Timer & timer)
{
    Graph g;
    std::string string;

    timer.Begin();
    while (std::getline(ifstream, string))
    {
        g.nodes.push_back(std::vector<std::unique_ptr<Node>>());
        for (auto& c : string)
        {
            g.nodes.back().push_back(std::make_unique<Node>(c - '0' ));
        }
        g.xLim = g.nodes.back().size();
    }
    g.yLim = g.nodes.size();
    timer.End("Node creation");

    timer.Begin();
    for (int y = 0; y < g.nodes.size(); y++)
    {
        for (int x = 0; x < g.nodes.size(); x++)
        {
            auto baseNode = g.GetNode(y, x);
            baseNode->Connect(g.GetNode(y, x - 1));
            baseNode->Connect(g.GetNode(y, x + 1));
            baseNode->Connect(g.GetNode(y - 1, x));
            baseNode->Connect(g.GetNode(y + 1, x));
            if (baseNode->height == 0)
            {
                g.trailHeads.push_back(baseNode);
            }
        }
    }
    timer.End("Linking");
    return g;
}

int GetNextTag()
{
    static int tag = 0;
    return ++tag;
}

int CountTrails(Node* node, int tag)
{
    if (tag && node->tag == tag) return 0;
    node->tag = tag;
    if (node->height == 9) return 1;
    int count = 0;
    for (auto& next : node->next)
    {
        count += CountTrails(next, tag);
    }
    return count;
}

int FindPathEnds(Graph const& g)
{
    int count = 0;
    for (auto head : g.trailHeads)
    {
        int tag = GetNextTag();
        count += CountTrails(head, tag);
    }
    return count;
}


int FindPathEndsP2(Graph const& g)
{
    int count = 0;
    for (auto head : g.trailHeads)
    {
        int tag = 0;
        count += CountTrails(head, tag);
    }
    return count;
}


int main()
{
    Timer timer;
    std::ifstream ifstream;
    ifstream.open("input.txt");
    Graph g = ReadInput(ifstream, timer);

    timer.Begin();
    int p1 = FindPathEnds(g);
    timer.End("P1");

    printf("p1: %d\n", p1);

    timer.Begin();
    int p2 = FindPathEndsP2(g);
    timer.End("P2");

    printf("p1: %d\n", p2);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
