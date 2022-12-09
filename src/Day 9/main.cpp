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
#include <unordered_set>

static const int DAY = 9;
static const std::string NAME = "Rope Bridge";

struct pair_hash {
    size_t operator()(const std::pair<int, int> &x) const {
        return x.first * 31 + x.second;
    }
};

int signum(int x) {
    if (x > 0) return 1;
    else if ( x < 0) return -1;
    else return 0;
}

void task1(std::stringstream &input) {
    std::unordered_set<std::pair<int, int>, pair_hash> visited;
    std::string buf;
    char dir;
    int num;

    std::pair<int, int> head;
    std::pair<int, int> tail;

    // Get Map size
    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        ss >> dir;
        ss >> num;

        for (int i = 0; i < num; i++) {
            // Move head
            switch (dir) {
                case 'U': head.second++; break;
                case 'D': head.second--; break;
                case 'L': head.first--; break;
                case 'R': head.first++; break;
            }
            
            // Move tail
            int yDis = head.second - tail.second;
            int xDis = head.first - tail.first;

            if (std::abs(yDis) >= 2 || std::abs(xDis) >= 2) {
                tail.second += signum(yDis);
                tail.first += signum(xDis);
            }

            visited.insert(tail);
        }
    }

    std::cout << "Visited tiles: " << visited.size() << std::endl;
}

void move_tail(std::pair<int, int> &head, std::pair<int, int> &tail) {
    int yDis = head.second - tail.second;
    int xDis = head.first - tail.first;

    if (std::abs(yDis) >= 2 || std::abs(xDis) >= 2) {
        tail.second += signum(yDis);
        tail.first += signum(xDis);
    }
}

void task2(std::stringstream &input) {
    std::unordered_set<std::pair<int, int>, pair_hash> visited;
    std::string buf;
    char dir;
    int num;

    std::vector<std::pair<int, int>> tails(10);
    std::pair<int, int> &head = tails[0];

    for (int i = 0; i < 10; i++) { tails[i].first = 0; tails[i].second = 0; }

    // Get Map size
    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        ss >> dir;
        ss >> num;

        for (int i = 0; i < num; i++) {
            // Move head
            switch (dir) {
                case 'U': head.second++; break;
                case 'D': head.second--; break;
                case 'L': head.first--; break;
                case 'R': head.first++; break;
            }

            for (int i = 0; i < 9; i++) {
                move_tail(tails[i], tails[i + 1]);
            }

            // Only keep track of the last tail
            visited.insert(tails[9]);
        }
    }

    std::cout << "Visited tiles: " << visited.size() << std::endl;
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