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

static const int DAY = 6;
static const std::string NAME = "Tuning Trouble";

void task1(std::stringstream &input) {
    std::string buf;
    std::deque<char> chars;
    std::vector<char> numEachChar(26, 0);
    char x;
    int idx = 0;
    bool noDuplicates;

    std::getline(input, buf);
    std::stringstream ss(buf);

    while (!ss.eof()) {
        idx++;
        ss >> x;
        numEachChar[x - 'a']++;
        chars.push_back(x);
        if (idx < 4) continue;

        noDuplicates = true;

        for (int i = 0; i < 26; i++) {
            if (numEachChar[i] > 1) {
                noDuplicates = false;
            }
        }

        if (noDuplicates) {
            std::cout << "Start marker is: " << idx << std::endl;
            return;
        }

        numEachChar[chars.front() - 'a']--;
        chars.pop_front();
    }
}

void task2(std::stringstream &input) {
    std::string buf;
    std::deque<char> chars;
    std::vector<char> numEachChar(26, 0);
    char x;
    int idx = 0;
    bool noDuplicates;

    std::getline(input, buf);
    std::stringstream ss(buf);

    while (!ss.eof()) {
        idx++;
        ss >> x;
        numEachChar[x - 'a']++;
        chars.push_back(x);
        if (idx < 14) continue;

        noDuplicates = true;

        for (int i = 0; i < 26; i++) {
            if (numEachChar[i] > 1) {
                noDuplicates = false;
            }
        }

        if (noDuplicates) {
            std::cout << "Start marker is: " << idx << std::endl;
            return;
        }

        numEachChar[chars.front() - 'a']--;
        chars.pop_front();
    }
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

    auto timeDiff = std::chrono::high_resolution_clock::now() - t1;
    std::cout << "Time to complete: " << std::chrono::duration_cast<std::chrono::microseconds>(timeDiff) << std::endl;
    task2(buffer);
}