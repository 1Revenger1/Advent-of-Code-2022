#include <colors.hpp>
#include <header.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <queue>
#include <unordered_set>
#include <unordered_map>

static const int DAY = 25;
static const std::string NAME = "Monkey in the Middle";

void task1(std::stringstream &input) {
    std::string buf;
    int64_t sum = 0;

    while (std::getline(input, buf)) {
        int64_t numSum = 0;
        for(auto it = buf.begin(); it != buf.end(); it++) {
            numSum *= 5;
            switch (*it) {
                case '1': numSum += 1; break;
                case '2': numSum += 2; break;
                case '=': numSum += -2; break;
                case '-': numSum += -1; break;
            }
        }

        sum += numSum;
    }
    
    std::string out;
    int64_t divSum = sum;

    while (divSum != 0) {
        int mod = divSum % 5;
        divSum /= 5;
        
        if (mod < 3) {
            out += (mod + '0');
        } else { 
            out += (mod == 4 ? '-' : '=');
            divSum++;
        }
    }

    std::reverse(out.begin(), out.end());
    std::cout << "Result is " << sum << " = " << out << std::endl;
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

    auto timeDiff = std::chrono::high_resolution_clock::now() - t1;
    std::cout << "Time to complete: " << std::chrono::duration_cast<std::chrono::microseconds>(timeDiff) << std::endl;
}