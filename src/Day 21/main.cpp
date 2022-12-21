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

static const int DAY = 21;
static const std::string NAME = "Monkey in the Middle";

enum MonkeyType {
    CONST, ADD, MIN, MULT, DIV
};

struct Monkey {
    std::string left;
    std::string right;
    MonkeyType type;
    bool isHumn;
    int num;
};

int64_t solveRoot(std::unordered_map<std::string, Monkey> &monkeys, Monkey &root) {
    if (root.type == CONST) return root.num;

    int64_t left = solveRoot(monkeys, monkeys[root.left]);
    int64_t right = solveRoot(monkeys, monkeys[root.right]);

    switch (root.type) {
        case MULT: return left * right;
        case DIV: return left / right;
        case ADD: return left + right;
        case MIN: return left - right;
    }

    throw "Invalid operation type";
}

void task1(std::stringstream &input) {
    std::unordered_map<std::string, Monkey> monkeys;
    std::string buf;
    
    while (std::getline(input, buf)) {
        Monkey monk;
        std::string name;

        name = buf.substr(0, 4);
        monk.isHumn = name == "humn";
        if (std::isdigit(buf[6])) {
            monk.type = CONST;
            monk.num = std::stoi(buf.substr(6));
        } else {
            monk.left = buf.substr(6, 4);
            monk.right = buf.substr(13, 4);

            switch (buf[11]) {
                case '+': monk.type = ADD; break;
                case '-': monk.type = MIN; break;
                case '*': monk.type = MULT; break;
                case '/': monk.type = DIV; break;
            }
        }

        monkeys[name] = monk;
    }

    std::cout << "Root val is: " << solveRoot(monkeys, monkeys["root"]) << std::endl;
}

bool findHumnPath(std::unordered_map<std::string, Monkey> &monkeys, std::deque<std::string> &path, const std::string &name) {
    Monkey &monk = monkeys[name];
    path.push_back(name);

    if (monk.type != CONST) {
        if (findHumnPath(monkeys, path, monk.left)) return true;
        if (findHumnPath(monkeys, path, monk.right)) return true;
    } else if (name == "humn") return true;

    path.pop_back();
    return false;
}

int64_t findHumnSol(std::unordered_map<std::string, Monkey> &monkeys, std::deque<std::string> &path, const std::string &name, int64_t equalToVal) {
    Monkey &curMonk = monkeys[name];
    std::string nextName = path.front();
    path.pop_front();

    bool left = curMonk.left == nextName;
    std::string solveName = left ? curMonk.right : curMonk.left;
    int64_t solveBranch = solveRoot(monkeys, monkeys[solveName]);

    int64_t nextStepVal = 0;
    switch (curMonk.type) {
        case ADD: nextStepVal = equalToVal - solveBranch; break;
        case MIN: nextStepVal = (left ? equalToVal : -equalToVal) + solveBranch; break;
        case MULT: nextStepVal = equalToVal / solveBranch; break;
        case DIV: nextStepVal = (left ? equalToVal * solveBranch : solveBranch / equalToVal); break;
    }

    if (nextName == "humn") return nextStepVal;
    return findHumnSol(monkeys, path, nextName, nextStepVal);
}

void task2(std::stringstream &input) {
    std::unordered_map<std::string, Monkey> monkeys;
    std::string buf;
    
    while (std::getline(input, buf)) {
        Monkey monk;
        std::string name;

        name = buf.substr(0, 4);
        if (std::isdigit(buf[6])) {
            monk.type = CONST;
            monk.num = std::stoi(buf.substr(6));
        } else {
            monk.left = buf.substr(6, 4);
            monk.right = buf.substr(13, 4);

            switch (buf[11]) {
                case '+': monk.type = ADD; break;
                case '-': monk.type = MIN; break;
                case '*': monk.type = MULT; break;
                case '/': monk.type = DIV; break;
            }
        }

        monkeys[name] = monk;
    }

    Monkey &root = monkeys["root"];

    // Find path to humn node
    std::deque<std::string> path;
    findHumnPath(monkeys, path, "root");
    path.pop_front(); // Don't need root node
    
    std::cout << "Returned " << path.size() << " steps to humn node" << std::endl;

    // Figure out what root value should equal
    int64_t equal = solveRoot(monkeys, monkeys[path.front() == root.left ? root.right : root.left]);
    std::string nextNode = path.front();
    path.pop_front();

    std::cout << "Human number is " << findHumnSol(monkeys, path, nextNode, equal) << std::endl;
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