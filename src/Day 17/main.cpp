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
#include <unordered_set>
#include <unordered_map>

static const int DAY = 17;
static const std::string NAME = "Monkey in the Middle";

typedef std::pair<int, int> Delta;

enum Shape {MINUS, PLUS, L, PIPE, CUBE};
enum Dir {LEFT, RIGHT, DOWN};

const static Delta shapes[5][5] = {
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}},           // -
    {{0, 1}, {1, 2}, {1, 0}, {1, 1}, {2, 1}},   // +
    {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}},   // L
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},           // |
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}}            // Cube
};

struct pair_hash {
    size_t operator()(const std::pair<int, int> &x) const {
        return x.first * 31 + x.second;
    }
};

bool canMove(Shape s, const Delta &loc, const std::unordered_set<Delta, pair_hash> &rocks, Dir d) {
    if (d == LEFT && loc.first <= 0) return false;
    else if (d == RIGHT) {
        int width = 3;
        if (s == PIPE) width = 1;
        else if (s == CUBE) width = 2;
        else if (s == MINUS) width = 4;
        width--;

        if (loc.first + width >= 6) return false;
    } else if (d == DOWN) {
        if (loc.second == 1) return false;
    }

    int shapeCubes = s == PLUS || s == L ? 5 : 4;
    for (int i = 0; i < shapeCubes; i++) {
        Delta del = shapes[s][i];
        del.first += loc.first;
        del.second += loc.second;
        switch (d) {
            case LEFT: del.first--; break;
            case RIGHT: del.first++; break;
            case DOWN: del.second--; break;
        }

        if (rocks.contains(del)) return false;
    }

    return true;
}

void task1(std::stringstream &input) {
    std::string pattern;
    std::unordered_set<Delta, pair_hash> filledLocs;

    std::getline(input, pattern);

    int patternCount = 0;
    int highestY = 0;

    for (int i = 0; i < 2022; i++) {
        bool inMotion = true;
        Delta curLoc = Delta(2, highestY + 4);

        while (inMotion) {
            Shape s = static_cast<Shape>(i % 5);
            if (pattern[patternCount] == '<') {
                if (canMove(s, curLoc, filledLocs, LEFT)) {
                    curLoc.first--;
                }
            } else {
                if (canMove(s, curLoc, filledLocs, RIGHT)) {
                    curLoc.first++;
                }
            }

            if (canMove(s, curLoc, filledLocs, DOWN)) {
                curLoc.second--;
            } else {
                inMotion = false;

                int shapeCubes = s == PLUS || s == L ? 5 : 4;
                for (int i = 0; i < shapeCubes; i++) {
                    Delta del = shapes[s][i];
                    del.first += curLoc.first;
                    del.second += curLoc.second;
                    highestY = std::max(highestY, del.second);
                    
                    filledLocs.insert(del);
                }
            }

            patternCount = (patternCount + 1) % pattern.size();
        }
    }

    std::cout << "Highest Y: " << highestY << std::endl;
}

const static int64_t TASK2_MAX = 1000000000000; // :(

int calcValueFromDeltas(const int64_t (&y)[]) {
    int64_t center = y[0];
    int val = 0;
    for (int i = 1; i < 7; i++) {
        val <<= 4;
        val += y[i] - center;
    }
    return val;
}

void task2(std::stringstream &input) {
    std::string jetstream;
    std::unordered_set<Delta, pair_hash> filledLocs;

    std::getline(input, jetstream);

    int jetCount = 0;
    int64_t highestY = 0;
    int64_t patternCycles = 0;
    int64_t patternHeight = 0;

    std::unordered_map<Delta, Delta, pair_hash> map;

    bool patternFound = false;
    int64_t y[7] = {0, 0, 0, 0, 0, 0, 0};

    for (int64_t i = 0; i < TASK2_MAX; i++) {
        bool inMotion = true;
        bool finishedPattern = false;

        Delta curLoc = Delta(2, highestY + 4);
        Shape s = static_cast<Shape>(i % 5);
        
        if (patternFound && (TASK2_MAX - i) % patternCycles == 0) {
            finishedPattern = true;
            highestY += (TASK2_MAX - i) / patternCycles * patternHeight;
            break;
        }

        while (inMotion) {
            if (canMove(s, curLoc, filledLocs, jetstream[jetCount] == '<' ? LEFT : RIGHT)) {
                curLoc.first += jetstream[jetCount] == '<' ? -1 : 1;
            }

            if (canMove(s, curLoc, filledLocs, DOWN)) {
                curLoc.second--;
            } else {
                inMotion = false;

                int shapeCubes = s == PLUS || s == L ? 5 : 4;
                for (int i = 0; i < shapeCubes; i++) {
                    Delta del = shapes[s][i];
                    del.first += curLoc.first;
                    del.second += curLoc.second;
                    highestY = std::max(highestY, (int64_t) del.second);
                    y[del.first] = std::max(y[del.first], (int64_t) del.second);
                    filledLocs.insert(del);
                }

                Delta prevKey = {s + (jetCount << 4), calcValueFromDeltas(y)};

                if (!patternFound && i > 10000 && map.contains(prevKey)) {
                    const auto [patternStartI, patternStartHeight] = map[prevKey];
                    std::cout << "Found pattern at i: " << i << ' ' << highestY << std::endl;
                    std::cout << "Pattern starts at i: " << patternStartI << ' ' << patternStartHeight << std::endl;
                    patternFound = true;
                    patternCycles = i - patternStartI;
                    patternHeight = highestY - patternStartHeight;
                }

                map[prevKey] = {i, highestY};

            }

            jetCount = (jetCount + 1) % jetstream.size();
        }
    }

    std::cout << "Highest Y: " << highestY << std::endl;
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