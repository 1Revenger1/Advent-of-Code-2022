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
#include <queue>
#include <unordered_set>
#include <unordered_map>

/**
 *  Based off of the solution below since I had no clue how to solve this
 * without brute forcing.
 * 
 * https://gist.github.com/bluepichu/59c815b132c0e9ad29e4df32c5cddfbd 
 */

static const int DAY = 16;
static const std::string NAME = "Monkey in the Middle";

typedef std::pair<int, int> range;

struct Vent {
    std::unordered_set<std::string> to;
    int flow;
};

static std::unordered_map<std::string, int> distMem;

int distToPnt(std::unordered_map<std::string, Vent> &map, std::string start, std::string end) {
    std::unordered_map<std::string, int> distMap;
    // Why is this a max heap by default :(
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> queue;

    if (distMem.contains(start + end)) return distMem[start + end];

    distMap[start] = 0;
    queue.push(std::pair<int, std::string>(0, start));

    while (!queue.empty()) {
        const auto [distTot, curLoc] = queue.top();
        queue.pop();

        if (curLoc == end) {
            distMem[start + end] = distTot;
            return distTot;
        }

        const auto &v = map[curLoc];

        for (const auto &n : v.to) {
            if (!distMap.contains(n)) distMap[n] = INT32_MAX;
            if (distTot + 1 > distMap[n]) continue;
            distMap[n] = distTot + 1;
            queue.push(std::pair<int, std::string>(distTot + 1, n));
        }
    }

    distMem[start + end] = -1;
    return -1;
}

int calcFlow(std::unordered_map<std::string, Vent> &map, std::vector<std::string> &hasFlow, int bitmask) {
    int flow = 0;
    for (int i = 0; i < hasFlow.size(); i++) {
        int mask = 1 << i;
        if (bitmask & mask) flow += map[hasFlow[i]].flow;
    }
    return flow;
}

void task1(std::stringstream &input) {
    std::string buf;
    std::unordered_map<std::string, Vent> map;
    std::vector<std::string> hasFlow;
    hasFlow.clear();

    while(std::getline(input, buf)) {
        std::string name;
        std::stringstream ss(buf);
        Vent v;

        ss.seekg(6);
        ss >> name;
        ss.seekg(15, std::ios_base::cur);
        ss >> v.flow;
        ss.seekg(24, std::ios_base::cur);
        if (ss.peek() == ' ') ss.seekg(1, std::ios_base::cur);
        std::string child;
        while (std::getline(ss, child, ',')) {
            v.to.insert(child);
            ss.seekg(1, std::ios_base::cur);
        }

        map[name] = v;
        if (v.flow > 0) hasFlow.push_back(name);
    }

    int64_t maxVal = 0;

    std::vector<std::vector<std::vector<int64_t>>> a(31, std::vector<std::vector<int64_t>>(hasFlow.size(), std::vector<int64_t>(1 << hasFlow.size(), INT64_MIN)));

    // Starting conditions, start at time valve opens if we go to each starting location
    for (size_t i = 0; i < hasFlow.size(); i++) {
        int64_t dist = distToPnt(map, "AA", hasFlow[i]);
        a[dist + 1][i][1 << i] = 0;
    }

    // minutes
    for (size_t min = 1; min <= 30; min++) {
        // cur Location
        for (size_t loc = 0; loc < hasFlow.size(); loc++) {
            // Released valves
            for (size_t rel = 0; rel < (1 << hasFlow.size()); rel++) {
                if (a[min - 1][loc][rel] < 0 && a[min][loc][rel] < 0) continue;

                int64_t flow = calcFlow(map, hasFlow, rel);

                a[min][loc][rel] = std::max(a[min][loc][rel], a[min - 1][loc][rel] + flow);

                maxVal = std::max(maxVal, a[min][loc][rel]);

                // This valve isn't open, so we can't travel from here
                if (((1 << loc) & rel) == 0) continue;

                // neighbors
                for (int dest = 0; dest < hasFlow.size(); dest++) {
                    if ((1 << dest) & rel) continue; // Already opened

                    int time = distToPnt(map, hasFlow[loc], hasFlow[dest]) + 1;

                    // Impossible to reach in time
                    if (min + time > 30) continue;

                    int64_t neighborTotFlow = a[min][loc][rel] + flow * time;
                    if (neighborTotFlow > a[min + time][dest][rel | (1 << dest)]) {
                        a[min + time][dest][rel | (1 << dest)] = neighborTotFlow;
                    }
                }
            }
        }
    }

    std::cout << "Solo max pressure: " << maxVal << std::endl;
    maxVal = 0;

    // Calculate max pressure at minute 26 with elephant friend helpin
    for (int eleR = 0; eleR < 1 << hasFlow.size(); eleR++) {
        for (int youR = 0; youR < 1 << hasFlow.size(); youR++) {
            // Elephant must visit completely different locations
            if ((eleR & youR) != 0) continue;

            int64_t youPres = INT64_MIN;
            int64_t elePres = INT64_MIN;

            for (int l = 0; l < hasFlow.size(); l++) {
                youPres = std::max(youPres, a[26][l][youR]);
            }

            for (int l = 0; l < hasFlow.size(); l++) {
                elePres = std::max(elePres, a[26][l][eleR]);
            }

            maxVal = std::max(maxVal, youPres + elePres);
        }
    }

    std::cout << "Elephant friend pressure: " << maxVal << std::endl;
}

void task2(std::stringstream &input) {
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