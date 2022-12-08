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

static const int DAY = 8;
static const std::string NAME = "Tuning Trouble";

bool isVisible(std::vector<std::vector<int>> &map, std::pair<int, int> coord) {
    auto [x, y] = coord;
    int height = map[y][x];

    auto visible = [height](int val) { return val >= height; };
    auto colVisible = [height, x](std::vector<int> &vec) { return vec[x] >= height; };

    auto topEnd = map.begin() + y;
    auto botBegin = map.begin() + y + 1;
    auto leftEnd = map[y].begin() + x;
    auto rightBegin = map[y].begin() + x + 1;

    bool leftVisible = std::ranges::find_if(map[y].begin(), leftEnd, visible) == leftEnd;
    bool rightVisible = std::ranges::find_if(rightBegin, map[y].end(), visible) == map[y].end();
    bool topVisible = std::ranges::find_if(map.begin(), topEnd, colVisible) == topEnd;
    bool botVisible = std::ranges::find_if(botBegin, map.end(), colVisible) == map.end();
    
    return leftVisible || rightVisible || topVisible || botVisible;
}

int scenicScore(std::vector<std::vector<int>> &map, std::pair<int, int> coord) {
    auto [x, y] = coord;
    int height = map[y][x];

    auto visible = [height](int val) { return val >= height; };
    auto colVisible = [height, x](std::vector<int> &vec) { return vec[x] >= height; };

    auto leftBegin = map[y].rbegin() + (map[y].size() - x);
    auto topBegin = map.rbegin() + (map.size() - y);

    auto botBegin = map.begin() + y + 1;
    auto rightBegin = map[y].begin() + x + 1;

    auto leftFirst = std::ranges::find_if(leftBegin, map[y].rend(), visible);
    auto topFirst = std::ranges::find_if(topBegin, map.rend(), colVisible);

    auto rightFirst = std::ranges::find_if(rightBegin, map[y].end(), visible);
    auto botFirst = std::ranges::find_if(botBegin, map.end(), colVisible);

    int leftScore = std::distance(leftBegin, leftFirst) + (leftFirst == map[y].rend() ? 0 : 1);
    int topScore = std::distance(topBegin, topFirst) + (topFirst == map.rend() ? 0 : 1);
    int rightScore = std::distance(rightBegin, rightFirst) + (rightFirst == map[y].end() ? 0 : 1);
    int botScore = std::distance(botBegin, botFirst) + (botFirst == map.end() ? 0 : 1);

    return leftScore * topScore * rightScore * botScore;
}

void task1(std::stringstream &input) {
    std::string buf;
    std::vector<std::string> lines;
    char x;

    // Get Map size
    while (std::getline(input, buf)) {
        lines.push_back(buf);
    }

    std::vector<std::vector<int>> map(lines.size());

    // Create map
    for (int i = 0; i < lines.size(); i++) {
        std::stringstream ss(lines[i]);
        for (int j = 0; j < lines[i].size(); j++) {
            ss >> x;
            map[i].push_back(x - '0');
        }
    }

    int visibleTrees = (lines[0].length() * 2) + ((lines.size() * 2) - 4);

    // Inner tiles only
    for (int i = 1; i < lines[0].length() - 1; i++) {
        for (int j = 1; j < lines.size() - 1; j++) {
            if (isVisible(map, std::pair<int, int>(j, i)))
                visibleTrees++;
        }
    }

    std::cout << "Visible trees: " << visibleTrees << std::endl;
}

void task2(std::stringstream &input) {
    std::string buf;
    std::vector<std::string> lines;
    char x;

    // Get Map size
    while (std::getline(input, buf)) {
        lines.push_back(buf);
    }

    std::vector<std::vector<int>> map(lines.size());

    // Create map
    for (int i = 0; i < lines.size(); i++) {
        std::stringstream ss(lines[i]);
        for (int j = 0; j < lines[i].size(); j++) {
            ss >> x;
            map[i].push_back(x - '0');
        }
    }

    int maxVis = 0;

    // Inner tiles only
    for (int i = 1; i < lines[0].length() - 1; i++) {
        for (int j = 1; j < lines.size() - 1; j++) {
            maxVis = std::max(maxVis, scenicScore(map, std::pair<int, int>(j, i)));
        }
    }

    std::cout << "Max vis: " << maxVis << std::endl;
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