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

static const int DAY = 14;
static const std::string NAME = "Monkey in the Middle";

void task1(std::stringstream &input) {
    std::vector<std::vector<bool>> map(500, std::vector<bool>(1000, false));
    std::string buf;
    std::string sep;
    char x;

    while (std::getline(input, buf)) {
        std::stringstream ss(buf);

        std::pair<int, int> first;
        std::pair<int, int> second;

        ss >> second.first;
        ss >> x;
        ss >> second.second;

        while (ss >> sep && sep == "->") {
            first = second;
            ss >> second.first;
            ss >> x;
            ss >> second.second;

            // vertical
            if (first.first == second.first) {
                for (int i = std::min(first.second, second.second); i <= std::max(first.second, second.second); i++) {
                    map[i][first.first] = true;
                }
            // horizontal
            } else {
                for (int i = std::min(first.first, second.first); i <= std::max(first.first, second.first); i++) {
                    map[first.second][i] = true;
                }
            }
        }
    }

    bool abyssalSnad = false;

    int snad = 0;

    while (!abyssalSnad) {
        int x = 500, y = 0;

        while (1) {
            if (!map[y + 1][x]) y++;
            else if (!map[y + 1][x - 1]) {
                y++;
                x--;
            } else if (!map[y + 1][x + 1]) {
                y++;
                x++;
            } else {
                map[y][x] = true;
                snad++;
                break;
            }

            if (y >= 499) {
                abyssalSnad = true;
                break;
            }
        }
    }

    std::cout << "Snad come to rest: " << snad << std::endl;
}

void task2(std::stringstream &input) {
    std::vector<std::vector<bool>> map(500, std::vector<bool>(1000, false));
    std::string buf;
    std::string sep;
    char x;

    int maxY;

    while (std::getline(input, buf)) {
        std::stringstream ss(buf);

        std::pair<int, int> first;
        std::pair<int, int> second;

        ss >> second.first;
        ss >> x;
        ss >> second.second;
        maxY = std::max(maxY, second.second);

        while (ss >> sep && sep == "->") {
            first = second;
            ss >> second.first;
            ss >> x;
            ss >> second.second;
            maxY = std::max(maxY, second.second);

            // vertical
            if (first.first == second.first) {
                for (int i = std::min(first.second, second.second); i <= std::max(first.second, second.second); i++) {
                    map[i][first.first] = true;
                }
            // horizontal
            } else {
                for (int i = std::min(first.first, second.first); i <= std::max(first.first, second.first); i++) {
                    map[first.second][i] = true;
                }
            }
        }
    }

    maxY += 2;
    for (int i = 0; i < map[maxY].size(); i++) {
        map[maxY][i] = true;
    }

    bool abyssalSnad = false;

    int snad = 0;

    while (!abyssalSnad) {
        int x = 500, y = 0;

        while (1) {
            if (!map[y + 1][x]) y++;
            else if (!map[y + 1][x - 1]) {
                y++;
                x--;
            } else if (!map[y + 1][x + 1]) {
                y++;
                x++;
            } else {
                map[y][x] = true;
                snad++;
                if (x == 500 && y == 0) {
                    abyssalSnad = true;
                }
                break;
            }
        }
    }

    std::cout << "Snad come to rest: " << snad << std::endl;
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