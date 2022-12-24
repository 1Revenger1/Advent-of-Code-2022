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

static const int DAY = 24;
static const std::string NAME = "Monkey in the Middle";

struct Loc {
    int x = 0; int y = 0;

    Loc operator + (const Loc &b) const {
        return {x + b.x, y + b.y};
    }

    Loc &operator += (const Loc &b) {
        x += b.x;
        y += b.y;
        return *this;
    }

    bool operator == (const Loc &b) const {
        return x == b.x && y == b.y;
    }
};

struct LocHash {
    size_t operator() (const Loc &a) const {
        return a.x * 31 + a.y;
    }
};

enum Dir {
    N, E, S, W
};

static const Loc dirDeltas[4] {
    {0, -1}, {1, 0}, {0, 1}, {-1, 0}
};

struct Blizzard {
    Loc loc;
    Dir dir;
};

int getTime(std::vector<Blizzard> &blizzards, const Loc start, const Loc dest, const Loc max) {
    std::unordered_set<Loc, LocHash> *actions = new std::unordered_set<Loc, LocHash>();
    const auto [maxX, maxY] = max;
    int minutes = 0;
    
    actions->insert(start);
    
    while (1) {
        minutes++;
        std::unordered_map<Loc, bool, LocHash> takenSpaces;

        for (auto &[blizLoc, dir] : blizzards) {
            blizLoc += dirDeltas[dir];
            if (blizLoc.x == 0) blizLoc.x = maxX - 1;
            if (blizLoc.x == maxX) blizLoc.x = 1;
            if (blizLoc.y == 0) blizLoc.y = maxY - 1;
            if (blizLoc.y == maxY) blizLoc.y = 1;

            takenSpaces[blizLoc] = true;
        }

        std::unordered_set<Loc, LocHash> *newActions = new std::unordered_set<Loc, LocHash>();
        
        for (const Loc &curLoc : *actions) {
            if (!takenSpaces.contains(curLoc)) newActions->insert(curLoc);

            for (const Loc &delta : dirDeltas) {
                Loc newLoc = curLoc + delta;
                
                if (newLoc == dest) {
                    return minutes;
                }

                if (newLoc.x > 0 && newLoc.x < max.x &&
                    newLoc.y > 0 && newLoc.y < max.y &&
                    !takenSpaces.contains(newLoc)) {
                    
                    newActions->insert(newLoc);
                }
            }
        }

        delete actions;
        actions = newActions;
    }

    return -1;
}

void task1(std::stringstream &input) {
    std::vector<Blizzard> blizzards;
    std::string buf;

    int line = 0;
    while (std::getline(input, buf)) {
        for (int i = 1; i < buf.size() - 1; i++) {
            Blizzard bliz = {{i, line}, N};

            switch (buf[i]) {
                case '^': bliz.dir = N; break;
                case '>': bliz.dir = E; break;
                case 'v': bliz.dir = S; break;
                case '<': bliz.dir = W; break;
                default: continue;
            }

            blizzards.push_back(bliz);
        }

        line++;
    }

    int maxX = buf.size() - 1;
    int maxY = line - 1;

    const Loc dest = {maxX - 1, maxY};
    const Loc start = {1, 0};

    std::cout << "Took " << getTime(blizzards, start, dest, {maxX, maxY}) << " minutes to reach the extraction point!" << std::endl;
}

void task2(std::stringstream &input) {
    std::vector<Blizzard> blizzards;
    std::string buf;

    int line = 0;
    while (std::getline(input, buf)) {
        for (int i = 1; i < buf.size() - 1; i++) {
            Blizzard bliz = {{i, line}, N};

            switch (buf[i]) {
                case '^': bliz.dir = N; break;
                case '>': bliz.dir = E; break;
                case 'v': bliz.dir = S; break;
                case '<': bliz.dir = W; break;
                default: continue;
            }

            blizzards.push_back(bliz);
        }

        line++;
    }

    int maxX = buf.size() - 1;
    int maxY = line - 1;

    const Loc dest = {maxX - 1, maxY};
    const Loc start = {1, 0};

    int minutesOne = getTime(blizzards, start, dest, {maxX, maxY});
    int minutesTwo = getTime(blizzards, dest, start, {maxX, maxY});
    int minutesThree = getTime(blizzards, start, dest, {maxX, maxY});

    std::cout << "Took " << minutesOne + minutesTwo + minutesThree << " minutes to reach the extraction point!" << std::endl;
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