#include <colors.hpp>
#include <header.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <format>
#include <vector>
#include <algorithm>
#include <iomanip>

static const int DAY = 4;
static const std::string NAME = "Camp Cleanup";

void task1(std::stringstream &input) {
    std::string buf;
    int one, two, three, four;
    char x;
    int score = 0;

    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        ss >> one >> x >> two >> x >> three >> x >> four;

        if (one >= three && two <= four) score++;
        else if (three >= one && four <= two) score++;
    }

    std::cout << "Common pairs add up to: " << score << std::endl;
}

void task2(std::stringstream &input) {
    std::string buf;
    int one, two, three, four;
    char x;
    int score = 0;

    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        ss >> one >> x >> two >> x >> three >> x >> four;

        if (two >= three && one <= four) score++;
    }
    
    std::cout << "Common pairs add up to: " << score << std::endl;
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