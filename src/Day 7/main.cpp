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
#include <chrono>
#include <map>

static const int DAY = 7;
static const std::string NAME = "Tuning Trouble";


struct Directory {
    Directory *parent;
    std::map<std::string, Directory *> dirs;
    int size = 0;
};

void calcSizes(Directory *root);
int getMinSizeAboveMin(Directory *root, int min);
int countSmallDirs(Directory *root);

void parseTree(std::stringstream &input) {
    std::string buf;

    Directory root { .parent = &root };
    Directory *curDir = &root;

    while (std::getline(input, buf)) {
        std::string x;
        std::stringstream ss(buf);
        ss >> x;
        if (x == "$") {
            std::string dir;
            ss >> dir;
            if (dir != "cd") continue;
            ss >> dir;

            if (dir == "/") {
                curDir = &root;
            } else if (dir == "..") {
                curDir = curDir->parent;
            } else {
                if (!curDir->dirs.contains(dir)) {
                    std::cerr << "Could not find directory: " << dir << std::endl;
                    exit(-1);
                }
                curDir = curDir->dirs[dir];
            }
        } else if (x == "dir") {
            std::string dirname;
            ss >> dirname;
            if (!curDir->dirs.contains(x)) {
                curDir->dirs[dirname] = new Directory();
                curDir->dirs[dirname]->parent = curDir;
            }
        } else {
            curDir->size += std::stoi(x);
        }
    }

    calcSizes(&root);

    std::cout << "Task 1" << std::endl;
    int size = countSmallDirs(&root);
    std::cout << "Size of all small dirs: " << size << std::endl;
    
    std::cout << "Task 2" << std::endl;
    int unused = 70000000 - root.size;
    std::cout << "Unused disk size: " << unused << std::endl;
    int res = getMinSizeAboveMin(&root, 30000000 - unused);
    std::cout << "Smallest deletable dir: " << res << std::endl;
}

void calcSizes(Directory *root) {
    for (const auto& [key, dir] : root->dirs) {
        calcSizes(dir);
        root->size += dir->size;
    }
}

int getMinSizeAboveMin(Directory *root, int min) {
    int res = INT32_MAX;

    for (const auto& [key, dir] : root->dirs) {
        res = std::min(res, getMinSizeAboveMin(dir, min));
    }

    if (root->size > min) res = std::min(res, root->size);
    return res;
}

int countSmallDirs(Directory *root) {
    int res = 0;

    for (const auto& [key, dir] : root->dirs) {
        res += countSmallDirs(dir);
    }

    if (root->size < 100000) res += root->size;
    return res;
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

    parseTree(buffer);

    auto timeDiff = std::chrono::high_resolution_clock::now() - t1;
    std::cout << "Time to complete: " << std::chrono::duration_cast<std::chrono::microseconds>(timeDiff) << std::endl;
}