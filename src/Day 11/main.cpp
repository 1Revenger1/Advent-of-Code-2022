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
#include <iterator>
#include <unordered_set>

static const int DAY = 11;
static const std::string NAME = "Monkey in the Middle";

enum OpType {
    ADD_CONST,
    MULT_CONST,
    MULT_OLD
};

struct Monkey {
    std::deque<uint64_t> items;
    OpType op;
    int cont;
    int testDivisor;
    int trueMonkey;
    int falseMonkey;

    long long inspectedItems;
};

struct MonkeySort {
    inline bool operator() (const Monkey& struct1, const Monkey& struct2)
    {
        return struct1.inspectedItems > struct2.inspectedItems;
    }
};

void decodeItems(std::string items, Monkey &monkey) {
    std::string x;

    std::stringstream ss(items);
    ss >> x; ss >> x;

    int item;
    char xChar;
    while (ss >> item) {
        monkey.items.push_back(item);
        ss >> xChar;
    }
}

void decodeOp(std::string op, Monkey &monkey) {
    std::stringstream ss(op);
    ss.seekg(23);

    char opChar;
    std::string val;
    ss >> opChar;
    ss >> val;
    switch (opChar) {
        case '*':
            if (val == "old") monkey.op = MULT_OLD;
            else {
                monkey.op = MULT_CONST;
                monkey.cont = std::stoi(val);
            }
            break;
        case '+':
            monkey.op = monkey.op = ADD_CONST;
            monkey.cont = std::stoi(val);
            break;
    }
}

void decodeDivisor(std::string op, Monkey &monkey) {
    std::stringstream ss(op);
    ss.seekg(21);
    ss >> monkey.testDivisor;
}

void decodeTrueRes(std::string res, Monkey &monkey) {
    std::stringstream ss(res);
    ss.seekg(28);
    ss >> monkey.trueMonkey;
}

void decodeFalseRes(std::string res, Monkey &monkey) {
    std::stringstream ss(res);
    ss.seekg(29);
    ss >> monkey.falseMonkey;
}

void task1(std::stringstream &input) {
    std::string buf;
    std::vector<Monkey> monkeys;

    while (std::getline(input, buf)) {
        Monkey monkey {};

        std::getline(input, buf);
        decodeItems(buf, monkey);

        std::getline(input, buf);
        decodeOp(buf, monkey);

        std::getline(input, buf);
        decodeDivisor(buf, monkey);

        std::getline(input, buf);
        decodeTrueRes(buf, monkey);
        std::getline(input, buf);
        decodeFalseRes(buf, monkey);
        // Trash line
        std::getline(input, buf);

        monkeys.push_back(monkey);
    }

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < monkeys.size(); j++) {
            Monkey &monkey = monkeys[j];

            while (!monkey.items.empty()) {
                uint64_t worry = monkey.items.front();
                monkey.items.pop_front();
                monkey.inspectedItems++;

                switch (monkey.op) {
                    case ADD_CONST: worry += monkey.cont; break;
                    case MULT_CONST: worry *= monkey.cont; break;
                    case MULT_OLD: worry *= worry; break;
                }

                worry /= 3; // Phew

                if (worry % monkey.testDivisor == 0) {
                    monkeys[monkey.trueMonkey].items.push_back(worry);
                } else {
                    monkeys[monkey.falseMonkey].items.push_back(worry);
                }
            }
        }
    }

    std::sort(monkeys.begin(), monkeys.end(), MonkeySort());

    std::cout << "Top two monkeys inspected items multiplied: " << monkeys[0].inspectedItems * monkeys[1].inspectedItems << std::endl;
}

void task2(std::stringstream &input) {
    std::string buf;
    std::vector<Monkey> monkeys;

    uint64_t worryRelief = 1;

    while (std::getline(input, buf)) {
        Monkey monkey {};

        std::getline(input, buf);
        decodeItems(buf, monkey);

        std::getline(input, buf);
        decodeOp(buf, monkey);

        std::getline(input, buf);
        decodeDivisor(buf, monkey);
        worryRelief *= monkey.testDivisor;

        std::getline(input, buf);
        decodeTrueRes(buf, monkey);
        std::getline(input, buf);
        decodeFalseRes(buf, monkey);
        // Trash line
        std::getline(input, buf);

        monkeys.push_back(monkey);
    }

    for (int i = 0; i < 10000; i++) {
        for (int j = 0; j < monkeys.size(); j++) {
            Monkey &monkey = monkeys[j];

            while (!monkey.items.empty()) {
                uint64_t worry = monkey.items.front();
                monkey.items.pop_front();
                monkey.inspectedItems++;

                switch (monkey.op) {
                    case ADD_CONST: worry += monkey.cont; break;
                    case MULT_CONST: worry *= monkey.cont; break;
                    case MULT_OLD: worry *= worry; break;
                }

                // Keep worry below uint64 max
                worry %= worryRelief;

                if (worry % monkey.testDivisor == 0) {
                    monkeys[monkey.trueMonkey].items.push_back(worry);
                } else {
                    monkeys[monkey.falseMonkey].items.push_back(worry);
                }
            }
        }
    }

    std::sort(monkeys.begin(), monkeys.end(), MonkeySort());

    std::cout << "Top two monkeys inspected items multiplied: " << monkeys[0].inspectedItems * monkeys[1].inspectedItems << std::endl;
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