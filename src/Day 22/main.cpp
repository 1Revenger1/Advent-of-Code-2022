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

static const int DAY = 22;
static const std::string NAME = "Monkey in the Middle";

struct Loc {
    int x = 0; int y = 0;

    Loc operator + (const Loc &b) {
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
    RIGHT, DOWN, LEFT, UP
};

static const Loc dirDeltas[4] {
    {1, 0}, {0, 1}, {-1, 0}, {0, -1}
};

Loc findFirst(const Loc &start, Dir dir, const std::unordered_map<Loc, bool, LocHash> &map, const Loc &max) {
    Loc loc = start;
    switch (dir) {
        case RIGHT: loc.x = 0; break;
        case DOWN: loc.y = 0; break;
        case LEFT: loc.x = max.x; break;
        case UP: loc.y = max.y; break;
    }

    while (!map.contains(loc)) loc += dirDeltas[(int) dir];
    return loc;
}

void task1(std::stringstream &input) {
    std::string buf;
    std::unordered_map<Loc, bool, LocHash> map;
    Loc max;

    int line = 0;
    while (std::getline(input, buf)) {
        if (buf.size() == 0) break;
        for (int i = 0; i < buf.size(); i++) {
            if (buf[i] != ' ') {
                map[{i, line}] = buf[i] == '#';
            }
        }
        
        max.x = std::max(max.x, (int) buf.size() + 1);
        line++;
    }

    max.y = line;
    std::getline(input, buf);
    std::stringstream ss(buf);

    int val;
    char turn;
    int dir = RIGHT;
    Loc pos = findFirst({0, 0}, RIGHT, map, max);

    while (ss >> val) {
        for (int i = 0; i < val; i++) {
            Loc newLoc;
            if (map.contains(pos + dirDeltas[dir])) {
                newLoc = pos + dirDeltas[dir];
            } else {
                // Wrap
                newLoc = findFirst(pos, (Dir) dir, map, max);
            }

            if (map[newLoc]) break;
            pos = newLoc;
        }

        if (ss >> turn) {
            dir += turn == 'R' ? 1 : -1;
            if (dir > 3) dir -= 4;
            else if (dir < 0) dir += 4;
        } else {
            break;
        }
    }

    std::cout << "Password is " << (pos.y + 1) * 1000 + (pos.x + 1) * 4 + dir << std::endl;
}

// Note that this only works on the main input, and not the example input
const int CUBE_SIDE_SIZE = 50;
const std::pair<int, Dir> CUBE_MAP[6][4] = {
    //Right      Down        Left        Up
    { {},        {},         {3, RIGHT}, {5, RIGHT}  },
    { {4, LEFT}, {2, LEFT},  {},         {5, UP}     },
    { {1, UP},   {},         {3, DOWN},  {}          },
    { {},        {},         {0, RIGHT}, {2, RIGHT}  },
    { {1, LEFT}, {5, LEFT},  {},         {}          },
    { {4, UP},   {1, DOWN},  {0, DOWN},  {}          }
};

std::pair<Loc, Dir> findFirstCube(const Loc &start, Dir dir, const std::unordered_map<Loc, bool, LocHash> &map, const Loc &max, const std::vector<Loc> &cubeSides) {
    // Find current cube location
    int side = 0;
    while (side < 6) {
        const Loc &sideTopLeft = cubeSides[side];
        int xDiff = start.x - sideTopLeft.x;
        int yDiff = start.y - sideTopLeft.y;
        if (xDiff >= 0 && xDiff < CUBE_SIDE_SIZE &&
            yDiff >= 0 && yDiff < CUBE_SIDE_SIZE)
            break;
        side++;
    }

    const auto &[newCube, newDir] = CUBE_MAP[side][(int) dir];

    Loc loc;
    switch (newDir) {
        case RIGHT: loc.x = 0; break;
        case DOWN: loc.y = 0; break;
        case LEFT: loc.x = max.x; break;
        case UP: loc.y = max.y; break;
    }

    int xMod = start.x % CUBE_SIDE_SIZE;
    int yMod = start.y % CUBE_SIDE_SIZE;

    // Map new location on the new cube wall based off of old and new direction
    switch (dir) {
        case RIGHT:
            switch (newDir) {
                case LEFT: loc.y = cubeSides[newCube].y + (CUBE_SIDE_SIZE - yMod - 1); break;
                case UP: loc.x = cubeSides[newCube].x + yMod; break;
            }
            break;
        case DOWN:
            switch (newDir) {
                case DOWN: loc.x = cubeSides[newCube].x + xMod; break;
                case LEFT: loc.y = cubeSides[newCube].y + xMod; break;
            }
            break;
        case LEFT:
            switch (newDir) {
                case DOWN: loc.x = cubeSides[newCube].x + yMod; break;
                case RIGHT: loc.y = cubeSides[newCube].y + (CUBE_SIDE_SIZE - yMod - 1); break;
            }
            break;
        case UP:
            switch (newDir) {
                case RIGHT: loc.y = cubeSides[newCube].y + xMod; break;
                case UP: loc.x = cubeSides[newCube].x + xMod; break;
            }
            break;
    }

    while (!map.contains(loc)) loc += dirDeltas[(int) newDir];
    return {loc, newDir};
}

void task2(std::stringstream &input) {
    std::string buf;
    std::unordered_map<Loc, bool, LocHash> map;
    Loc max;

    std::vector<Loc> cubeSides;

    int line = 0;
    while (std::getline(input, buf)) {
        if (buf.size() == 0) break;
        for (int i = 0; i < buf.size(); i++) {
            if (buf[i] != ' ') {
                if (line % CUBE_SIDE_SIZE == 0 && i % CUBE_SIDE_SIZE == 0) {
                    cubeSides.push_back({i, line});
                }
                map[{i, line}] = buf[i] == '#';
            }
        }
        
        max.x = std::max(max.x, (int) buf.size() + 1);
        line++;
    }

    max.y = line;
    std::getline(input, buf);
    std::stringstream ss(buf);

    int val;
    char turn;
    int dir = RIGHT;
    Loc pos = findFirst({0, 0}, RIGHT, map, max);

    while (ss >> val) {
        for (int i = 0; i < val; i++) {
            Loc newLoc;
            int newDir = dir;
            if (map.contains(pos + dirDeltas[dir])) {
                newLoc = pos + dirDeltas[dir];
            } else {
                // Wrap
                auto res = findFirstCube(pos, (Dir) dir, map, max, cubeSides);
                newLoc = res.first;
                newDir = res.second;

                // Calculate inverse to help detect bugs
                auto rev = findFirstCube(res.first, (Dir) (((int) newDir + 2) % 4), map, max, cubeSides);

                if (!(pos == rev.first))
                    throw "Inverse of wrap is not equivalent to original position!";
            }

            if (map[newLoc]) break;
            pos = newLoc;
            dir = newDir;
        }

        if (ss >> turn) {
            dir += turn == 'R' ? 1 : -1;
            if (dir > 3) dir -= 4;
            else if (dir < 0) dir += 4;
        } else {
            break;
        }
    }

    // 158036
    // 145126 -- Too high
    // 136126 -- Too low

    // 48503 (Not right)
    // 43213

    // 21235 (Not Right)

    std::cout << "Password is " << (pos.y + 1) * 1000 + (pos.x + 1) * 4 + dir << std::endl;
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