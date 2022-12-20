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

static const int DAY = 20;
static const std::string NAME = "Monkey in the Middle";

void task1(std::stringstream &input) {
    std::vector<std::pair<size_t, int>> idxArr;
    std::vector<size_t> movArr;
    std::string buf;

    size_t zeroIdxArrIdx;

    while (std::getline(input, buf)) {
        int num = std::stoi(buf);

        size_t valIdx = idxArr.size();
        // Store pointer to moving array and number of times to move
        std::pair<size_t, int> val(valIdx, num);
        idxArr.push_back(val);
        // In moving array, point back to values array which contain how much to move
        movArr.push_back(valIdx);

        if (num == 0) zeroIdxArrIdx = valIdx;
    }

    for (int i = 0; i < idxArr.size(); i++) {
        auto &[startPtr, numToMove] = idxArr[i];
        movArr.erase(movArr.begin() + startPtr);

        int64_t dest = startPtr + numToMove;
        while (dest < 0) {
            dest += movArr.size();
        }

        dest %= movArr.size();

        // Update index array to account for removed item
        for (size_t j = startPtr; j < movArr.size(); j++) {
            idxArr[movArr[j]].first--;
        }

        // Update index array for inserted item
        for (size_t j = dest; j < movArr.size(); j++) {
            idxArr[movArr[j]].first++;
        }

        // Reinsert value
        movArr.insert(movArr.begin() + dest, i);
        // Update idxArray
        startPtr = dest;
    }

    size_t startIdx = idxArr[zeroIdxArrIdx].first;
    int one = idxArr[movArr[(startIdx + 1000) % movArr.size()]].second;
    int two = idxArr[movArr[(startIdx + 2000) % movArr.size()]].second;
    int three = idxArr[movArr[(startIdx + 3000) % movArr.size()]].second;
    std::cout << one + two + three << std::endl;
}

static const int64_t DECRYPT_KEY = 811589153;

void task2(std::stringstream &input) {
    std::vector<std::pair<int64_t, int64_t>> idxArr;
    std::vector<int64_t> movArr;
    std::string buf;

    size_t zeroIdxArrIdx;

    while (std::getline(input, buf)) {
        int64_t num = std::stoi(buf) * DECRYPT_KEY;

        size_t valIdx = idxArr.size();
        // Store pointer to moving array and number of times to move
        std::pair<size_t, int64_t> val(valIdx, num);
        idxArr.push_back(val);
        // In moving array, point back to values array which contain how much to move
        movArr.push_back(valIdx);

        if (num == 0) zeroIdxArrIdx = valIdx;
    }

    for (int r = 0; r < 10; r++) {
        std::cout << "Round " << r << std::endl;
        for (int i = 0; i < idxArr.size(); i++) {
            auto &[startPtr, numToMove] = idxArr[i];
            movArr.erase(movArr.begin() + startPtr);

            int64_t dest = startPtr + numToMove;
            
            if (dest < 0) {
                dest += movArr.size() * ((-dest - 1) / movArr.size() + 1);
            }

            dest %= movArr.size();

            // Update index array to account for removed item
            for (size_t j = startPtr; j < movArr.size(); j++) {
                idxArr[movArr[j]].first--;
            }

            // Update index array for inserted item
            for (size_t j = dest; j < movArr.size(); j++) {
                idxArr[movArr[j]].first++;
            }

            // Reinsert value
            movArr.insert(movArr.begin() + dest, i);
            // Update idxArray
            startPtr = dest;
        }
    }

    size_t startIdx = idxArr[zeroIdxArrIdx].first;
    int64_t one = idxArr[movArr[(startIdx + 1000) % movArr.size()]].second;
    int64_t two = idxArr[movArr[(startIdx + 2000) % movArr.size()]].second;
    int64_t three = idxArr[movArr[(startIdx + 3000) % movArr.size()]].second;
    std::cout << one + two + three << std::endl;
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