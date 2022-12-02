#include <colors.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <format>
#include <vector>
#include <algorithm>
#include <iomanip>

static const int DAY = 2;
static const std::string NAME = "Calorie Counting";

void printHeader(void) {
    const std::string nameStr = std::to_string(DAY) + " - " + NAME;
    const std::string adventStr = "Advent of Code 2022";
    size_t width = std::max(nameStr.length(), adventStr.length()) + 2;

    const std::string topBotStr = "+" + std::string(width, '-') + "+";
    const std::string fmt = "{:^" + std::to_string(width) + "}";
    const std::string adventLine = std::vformat(fmt, std::make_format_args(adventStr));
    const std::string nameLine = std::vformat(fmt, std::make_format_args(nameStr));

    std::cout << Colors::color(topBotStr + "\n|", Colors::GREEN);
    std::cout << Colors::color(adventLine, Colors::BOLD + Colors::BLUE);
    std::cout << Colors::color("|\n|", Colors::GREEN);
    std::cout << Colors::color(nameLine, Colors::BLUE);
    std::cout << Colors::color("|\n" + topBotStr + "\n\n", Colors::GREEN);
}

void task1(std::stringstream &input) {
    std::string buf;
    int score = 0;

    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        std::string oppo;
        std::string play;

        ss >> std::quoted(oppo);
        ss >> std::quoted(play);

        // A = ROCK
        // B = PAPER
        // Z = Scissors
        // X = ROCK
        // Y = PAPER
        // Z = Scissors

        if (play == "X") score += 1;
        if (play == "Y") score += 2;
        if (play == "Z") score += 3;

        if (oppo == "A") {
            if (play == "X") score += 3;
            else if (play == "Y") score += 6;
            else score += 0;
        } else if (oppo == "B") {
            if (play == "X") score += 0;
            else if (play == "Y") score += 3;
            else score += 6;
        } else {
            if (play == "X") score += 6;
            else if (play == "Y") score += 0;
            else score += 3;
        }
    }

    std::cout << "Max score is: " << score << std::endl;
}

void task2(std::stringstream &input) {
    std::string buf;
    int score = 0;

    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        std::string oppo;
        std::string play;

        ss >> std::quoted(oppo);
        ss >> std::quoted(play);

        // A = ROCK +1
        // B = PAPER +2
        // Z = Scissors +3
        // X = lose +0
        // Y = draw +3
        // Z = win +6

        if (oppo == "A") {
            if (play == "X") score += 3;
            else if (play == "Y") score += 4;
            else score += 8;
        } else if (oppo == "B") {
            if (play == "X") score += 1;
            else if (play == "Y") score += 5;
            else score += 9;
        } else { // scissors
            if (play == "X") score += 2;
            else if (play == "Y") score += 6;
            else score += 7;
        }
    }

    std::cout << "Max score is: " << score << std::endl;
}

int main(int, char**) {
    printHeader();

    std::ifstream inputFile("./src/Day " + std::to_string(DAY) + "/input.txt");
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