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

static const int DAY = 10;
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
    std::string buf;
    std::string op;
    int x = 1;

    int cycles = 0;
    int signal = 0;

    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        ss >> op;

        if (op == "addx") {
            int val;
            ss >> val;

            cycles += 2;
            if ((cycles - 20) % 40 <= 1 && cycles >= 20 && cycles < 240)
                signal += ( (cycles / 20 * 20) * x);
            x += val;
        } else if (op == "noop") {
            cycles++;
            if ((cycles - 20) % 40 == 0 && cycles >= 20 && cycles < 240)
                signal += (cycles * x);
        }
    }

    std::cout << "Visited tiles: " << signal << std::endl;
}

void writePixel(int &cycles, int x, std::vector<std::vector<char>> &map) {
    int screenX = cycles % 40;
    if (std::abs(x - screenX) < 2) map[cycles / 40][screenX] = '#';
    cycles++;
}

void printPixel(char x) {
    if (x == '#') std::cout << Colors::BOLD << Colors::BLUE << x << Colors::RESET;
    else std::cout << Colors::BLACK << x;
}

void task2(std::stringstream &input) {
    std::string buf;
    std::string op;
    int x = 1;

    int cycles = 0;
    int signal = 0;

    std::vector<std::vector<char>> map(6, std::vector<char>(40, '.'));

    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        ss >> op;

        if (op == "addx") {
            int val;
            ss >> val;

            writePixel(cycles, x, map);
            writePixel(cycles, x, map);
            x += val;
        } else if (op == "noop") {
            writePixel(cycles, x, map);
        }
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 40; j++) {
            printPixel(map[i][j]);
        }
        std::cout << std::endl;
    }

    std::cout << Colors::RESET;
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