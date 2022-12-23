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

static const int DAY = 23;
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
    N, NE, E, SE, S, SW, W, NW
};

static const Loc dirDeltas[8] {
    {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
};

static const Dir movements[4] {
    N, S, W, E
};

bool isValidMove(const bool neighbors[], Dir dir) {
    switch (dir) {
        case N: return !neighbors[NE] && !neighbors[N] && !neighbors[NW];
        case S: return !neighbors[SE] && !neighbors[S] && !neighbors[SW];
        case W: return !neighbors[NW] && !neighbors[W] && !neighbors[SW];
        case E: return !neighbors[NE] && !neighbors[E] && !neighbors[SE];
    }
    
    throw "Invalid move direction!";
}

void task1(std::stringstream &input) {
    int line = 0;
    std::string buf;
    std::unordered_set<Loc, LocHash> *elves = new std::unordered_set<Loc, LocHash>();

    while (std::getline(input, buf)) {
        for (int i = 0; i < buf.size(); i++) {
            if (buf[i] == '#') {
                elves->insert({i, line});
            }
        }

        line++;
    }

    int firstOffset = 0;

    for (int r = 0; r < 10; r++) {
        std::unordered_map<Loc, Loc, LocHash> nextMove;
        std::unordered_map<Loc, int, LocHash> numElvesMovingToLoc;

        // Propose moves
        for (const Loc &loc : *elves) {
            bool neighbors[8];
            bool empty = true;
            for (int i = 0; i < 8; i++) {
                bool nExists = elves->contains(loc + dirDeltas[i]);
                neighbors[i] = nExists;
                if (nExists) empty = false;
            }

            if (empty) continue;

            for (int i = 0; i < 4; i++) {
                Dir dir = movements[(i + firstOffset) % 4];

                if (isValidMove(neighbors, dir)) {
                    // Increment number of elves moving to location
                    numElvesMovingToLoc[loc + dirDeltas[dir]]++;
                    // Encode next move
                    nextMove[loc] = loc + dirDeltas[dir];
                    break;
                }
            }
        }


        std::unordered_set<Loc, LocHash> *newElves = new std::unordered_set<Loc, LocHash>();

        // Do movement if able
        for (const Loc &loc : *elves) {
            const Loc &nextLoc = nextMove[loc];
            if (numElvesMovingToLoc[nextLoc] == 1) {
                newElves->insert(nextLoc);
            } else {
                newElves->insert(loc);
            }
        }

        delete elves;
        elves = newElves;
        firstOffset = (firstOffset + 1) % 4;
    }

    Loc min = *elves->begin();
    Loc max = *elves->begin();

    for (const Loc &loc : *elves) {
        min.x = std::min(min.x, loc.x);
        min.y = std::min(min.y, loc.y);
        max.x = std::max(max.x, loc.x);
        max.y = std::max(max.y, loc.y);
    }

    std::cout << "Number of empty tiles is " << ((max.x - min.x + 1) * (max.y - min.y + 1)) - elves->size() << std::endl;
}

void task2(std::stringstream &input) {
    int line = 0;
    std::string buf;
    std::unordered_set<Loc, LocHash> *elves = new std::unordered_set<Loc, LocHash>();

    while (std::getline(input, buf)) {
        for (int i = 0; i < buf.size(); i++) {
            if (buf[i] == '#') {
                elves->insert({i, line});
            }
        }

        line++;
    }

    int firstOffset = 0;
    int rounds = 1;

    std::unordered_map<Loc, Loc, LocHash> nextMove;
    std::unordered_map<Loc, int, LocHash> numElvesMovingToLoc;

    while(1) {
        nextMove.clear();
        numElvesMovingToLoc.clear();

        // Propose moves
        for (const Loc &loc : *elves) {
            bool neighbors[8];
            bool empty = true;
            for (int i = 0; i < 8; i++) {
                neighbors[i] = elves->contains(loc + dirDeltas[i]);
                if (neighbors[i]) empty = false;
            }

            // Do not move if no elves nearby
            if (empty) continue;

            for (int i = 0; i < 4; i++) {
                Dir dir = movements[(i + firstOffset) % 4];

                if (isValidMove(neighbors, dir)) {
                    // Increment number of elves moving to location
                    numElvesMovingToLoc[loc + dirDeltas[dir]]++;
                    // Encode next move
                    nextMove[loc] = loc + dirDeltas[dir];
                    break;
                }
            }
        }

        if (nextMove.size() == 0) break;

        std::unordered_set<Loc, LocHash> *newElves = new std::unordered_set<Loc, LocHash>();

        // Do movement if able
        for (const Loc &loc : *elves) {
            if (!nextMove.contains(loc)) {
                newElves->insert(loc);
                continue;
            }
            
            const Loc &nextLoc = nextMove[loc];
            if (numElvesMovingToLoc[nextLoc] == 1) {
                // Only one elf moving to the new location
                newElves->insert(nextLoc);
            } else if (numElvesMovingToLoc[nextLoc] != 2) {
                int val = numElvesMovingToLoc[nextLoc];
                throw "Uhhhh!!!!!!!";
            } else {
                // Too many elves, stay at old location
                newElves->insert(loc);
            }
        }

        delete elves;
        elves = newElves;
        firstOffset = (firstOffset + 1) % 4;
        rounds++;
    }

    std::cout << "First round of no movement is " << rounds << std::endl;
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