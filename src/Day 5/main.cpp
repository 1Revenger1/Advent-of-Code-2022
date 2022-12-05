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

static const int DAY = 5;
static const std::string NAME = "Camp Cleanup";

void task1(std::stringstream &input) {
    std::vector<std::string> lines;
    std::string buf;

    while (std::getline(input, buf)) {
        if (buf.starts_with(" 1")) break;
        lines.push_back(buf);
    }
    
    std::cout << buf << std::endl;
    int columns = buf[buf.length() - 2] - '0';

    // Assume at most 10 (I hope)
    std::stack<char> stacks[10];
    std::cout << columns << " in input" << std::endl;

    for (int i = 0; i < columns; i++) {
        std::cout << i << std::endl;
        for (int j = lines.size() - 1; j >= 0; j--) {
            char c = lines[j][(i * 4) + 1];
            if (c != ' ')
                stacks[i].push(lines[j][(i * 4) + 1]);
        }
    }

    // trash new line
    std::getline(input, buf);
    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        int num, from, to;
        std::string a;
        ss >> a >> num >> a >> from >> a >> to;

        for (int i = 0; i < num; i++) {
            stacks[to - 1].push(stacks[from - 1].top());
            stacks[from - 1].pop();
        }
    }

    std::string result = "";
    for (int i = 0; i < columns; i++) {
        result += stacks[i].top();
    }

    std::cout << "Common pairs add up to: " << result << std::endl;
}

void task2(std::stringstream &input) {
    std::vector<std::string> lines;
    std::string buf;

    while (std::getline(input, buf)) {
        if (buf.starts_with(" 1")) break;
        lines.push_back(buf);
    }
    
    std::cout << buf << std::endl;
    int columns = buf[buf.length() - 2] - '0';

    // Assume at most 10 (I hope)
    std::stack<char> stacks[10];
    std::cout << columns << " in input" << std::endl;

    for (int i = 0; i < columns; i++) {
        std::cout << i << std::endl;
        for (int j = lines.size() - 1; j >= 0; j--) {
            char c = lines[j][(i * 4) + 1];
            if (c != ' ')
                stacks[i].push(lines[j][(i * 4) + 1]);
        }
    }

    // trash new line
    std::getline(input, buf);
    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        int num, from, to;
        std::string a;
        ss >> a >> num >> a >> from >> a >> to;

        std::stack<char> ugh;

        for (int i = 0; i < num; i++) {
            ugh.push(stacks[from - 1].top());
            stacks[from - 1].pop();
        }

        for (int i = 0; i < num; i++) {
            stacks[to - 1].push(ugh.top());
            ugh.pop();
        }
    }

    std::string result = "";
    for (int i = 0; i < columns; i++) {
        result += stacks[i].top();
    }

    std::cout << "Common pairs add up to: " << result << std::endl;
}

int main(int, char**) {
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
}