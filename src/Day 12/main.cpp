#include <colors.hpp>
#include <header.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <format>
#include <vector>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <iterator>
#include <queue>

static const int DAY = 12;
static const std::string NAME = "Monkey in the Middle";

struct GraphNode {
    unsigned int dist = UINT32_MAX;
    GraphNode *prev = nullptr;
    std::pair<int, int> pos;
    bool visited = false;
    int height;
};

struct GraphSort {
    inline bool operator() (const GraphNode *struct1, const GraphNode *struct2)
    {
        return struct1->dist > struct2->dist;
    }
};

static std::vector<std::pair<int, int>> dirs = {
    std::pair<int, int>(1, 0),
    std::pair<int, int>(0, 1),
    std::pair<int, int>(-1, 0),
    std::pair<int, int>(0, -1)
};

void task1(std::stringstream &input) {
    std::string buf;
    std::vector<std::string> lines;

    while (std::getline(input, buf)) lines.push_back(buf);
    std::priority_queue<std::pair<int, GraphNode *>, std::vector<std::pair<int, GraphNode *>>, std::greater<std::pair<int, GraphNode *>>> queue;
    std::vector<std::vector<GraphNode *>> map(lines.size());

    // std::pair<int, int> start;
    std::pair<int, int> dest;

    for (int y = 0; y < lines.size(); y++) {
        for (char c : lines[y]) {
            GraphNode *node = new GraphNode;
            if (c == 'S') {
                node->dist = 0;
                node->height = 0;
                queue.push(std::pair<int, GraphNode *> (0, node));
            } else if (c == 'E') {
                dest.second = y;
                dest.first = map[y].size();
                node->height = 'z' - 'a';
            } else {
                node->height = c - 'a';
            }

            node->pos.first = map[y].size();
            node->pos.second = y;
            map[y].push_back(node);
        }
    }

    while (!queue.empty()) {
        GraphNode *node = queue.top().second;
        queue.pop();
        node->visited = true;

        if (node->height == -1) continue;

        if (node->pos == dest) {
            break;
        }

        for (const auto &dir : dirs) {
            auto [dx, dy] = dir;
            int x = node->pos.first + dx;
            int y = node->pos.second + dy;

            if (x < 0 || y < 0 || x >= map[0].size() || y >= map.size()) {
                continue;
            }

            auto neighbor = map[y][x];
            if (neighbor->visited || neighbor->height - node->height > 1) continue;

            int newDist = node->dist + 1;
            if (newDist < neighbor->dist) {
                neighbor->dist = newDist;
                neighbor->prev = node;
                queue.push(std::pair<int, GraphNode *>(newDist, neighbor));
            }
        }
    }

    std::cout << "Distance travelled is: " << map[dest.second][dest.first]->dist << std::endl;
}

void task2(std::stringstream &input) {std::string buf;
    std::vector<std::string> lines;

    while (std::getline(input, buf)) lines.push_back(buf);
    std::priority_queue<std::pair<int, GraphNode *>, std::vector<std::pair<int, GraphNode *>>, std::greater<std::pair<int, GraphNode *>>> queue;
    std::vector<std::vector<GraphNode *>> map(lines.size());

    std::vector<GraphNode *> lowElev;

    for (int y = 0; y < lines.size(); y++) {
        for (char c : lines[y]) {
            GraphNode *node = new GraphNode;
            if (c == 'S') {
                node->height = 0;
                lowElev.push_back(node);
            } else if (c == 'E') {
                node->dist = 0;
                queue.push(std::pair<int, GraphNode *> (0, node));
                node->height = 'z' - 'a';
            } else {
                if (c == 'a') lowElev.push_back(node);
                node->height = c - 'a';
            }

            node->pos.first = map[y].size();
            node->pos.second = y;
            map[y].push_back(node);
        }
    }

    while (!queue.empty()) {
        GraphNode *node = queue.top().second;
        queue.pop();
        node->visited = true;

        if (node->height == -1) continue;

        for (const auto &dir : dirs) {
            auto [dx, dy] = dir;
            int x = node->pos.first + dx;
            int y = node->pos.second + dy;

            if (x < 0 || y < 0 || x >= map[0].size() || y >= map.size()) {
                continue;
            }

            auto neighbor = map[y][x];
            if (neighbor->visited || node->height - neighbor->height > 1) continue;

            int newDist = node->dist + 1;
            if (newDist < neighbor->dist) {
                neighbor->dist = newDist;
                neighbor->prev = node;
                queue.push(std::pair<int, GraphNode *>(newDist, neighbor));
            }
        }
    }

    unsigned int lowDist = INT32_MAX;
    for (const GraphNode *node : lowElev) {
        lowDist = std::min(node->dist, lowDist);
    }

    std::cout << "Distance travelled is: " << lowDist << std::endl;
}

int main(int, char**) {
    auto t1 = std::chrono::high_resolution_clock::now();
    printHeader(DAY, NAME);

    std::ifstream inputFile("../../src/Day " + std::to_string(DAY) + "/input.txt");
    std::stringstream buffer;

    if (inputFile.fail()) {
        std::cout << "No input file!" << std::endl;
        exit(-1);
    }

    buffer << inputFile.rdbuf();

    std::cout << "Task 1" << std::endl;
    task1(buffer);

    buffer.clear();
    buffer.seekg(0);

    std::cout << std::endl << "Task 2" << std::endl;

    task2(buffer);
    auto timeDiff = std::chrono::high_resolution_clock::now() - t1;
    std::cout << "Time to complete: " << std::chrono::duration_cast<std::chrono::microseconds>(timeDiff) << std::endl;
}