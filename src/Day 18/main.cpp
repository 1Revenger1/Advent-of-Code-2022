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

static const int DAY = 18;
static const std::string NAME = "Monkey in the Middle";

struct Xyz {
    int x;
    int y;
    int z;

    Xyz operator + (const Xyz &a) const {
        Xyz res;
        res.x = x + a.x;
        res.y = y + a.y;
        res.z = z + a.z;
        return res;
    }

    bool operator == (const Xyz &a) const {
        return a.x == x && a.y == y && a.z == z;
    }
};

struct XyzHash {
    size_t operator()(const Xyz &a) const {
        return a.x * 31 + a.y * 31 + a.z;
    }
};

const static Xyz dirs[6] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {-1, 0, 0},
    {0, -1, 0},
    {0, 0, -1}
};

void task1(std::stringstream &input) {
    std::unordered_set<Xyz, XyzHash> cubes;
    std::string buf;
    
    while (std::getline(input, buf)) {
        Xyz cube;
        char discard;
        std::stringstream ss(buf);
        ss >> cube.x;
        ss >> discard;
        ss >> cube.y;
        ss >> discard;
        ss >> cube.z;

        cubes.insert(cube);
    }

    int sides = 0;

    for (const Xyz &cube : cubes) {
        for (const Xyz &dir : dirs) {
            Xyz neighbor = cube + dir;
            if (!cubes.contains(neighbor)) sides++;
        }
    }

    std::cout << "Exposed sides: " << sides << std::endl;
}

bool checkInterior(Xyz maxes, Xyz cube, std::unordered_set<Xyz, XyzHash> &interior, std::unordered_set<Xyz, XyzHash> cubes) {
    std::unordered_set<Xyz, XyzHash> visited;
    std::queue<Xyz> queue;

    if (interior.contains(cube)) return true;
    queue.push(cube);

    while (!queue.empty()) {
        Xyz curCube = queue.front();
        queue.pop();
        
        for (const Xyz &dir : dirs) {
            Xyz newLoc = curCube + dir;
            if (newLoc.x > maxes.x || newLoc.y > maxes.y || newLoc.z > maxes.z ||
                newLoc.x < 0 || newLoc.y < 0 || newLoc.z < 0) {
                // reached exterior
                
                return false;
            }

            if (!visited.contains(newLoc) && !cubes.contains(newLoc)) {
                visited.insert(newLoc);
                queue.push(newLoc);
            }
        }
    }

    // interior
    for (const Xyz &visit : visited) {
        interior.insert(visit);
    }

    return true;
}

void task2(std::stringstream &input) {
    std::unordered_set<Xyz, XyzHash> cubes;
    std::string buf;

    Xyz max;

    std::unordered_set<Xyz, XyzHash> interior;

    while (std::getline(input, buf)) {
        Xyz cube;
        char discard;
        std::stringstream ss(buf);
        ss >> cube.x;
        ss >> discard;
        ss >> cube.y;
        ss >> discard;
        ss >> cube.z;

        max.y = std::max(max.y, cube.y);
        max.x = std::max(max.x, cube.x);
        max.z = std::max(max.z, cube.z);

        cubes.insert(cube);
    }

    int sides = 0;

    for (const Xyz &cube : cubes) {
        for (const Xyz &dir : dirs) {
            Xyz neighbor = cube + dir;

            if (!cubes.contains(neighbor) && !checkInterior(max, neighbor, interior, cubes)) sides++;
        }
    }

    std::cout << "Exposed sides: " << sides << std::endl;
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