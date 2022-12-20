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

static const int DAY = 19;
static const std::string NAME = "Monkey in the Middle";

struct State {
    int minute;
    int ore, clay, obs, geode; // Resources
    int oreR, clayR, obsR;    // Robots
};

State fastForward(const State &sta, int minutes) {
    State newState = sta;
    newState.minute += minutes;
    newState.ore += sta.oreR * minutes;
    newState.clay += sta.clayR * minutes;
    newState.obs += sta.obsR * minutes;

    return newState;
}

#define ROUND_UP_DIV(num, div) (num <= 0 ? 0 : (((num) - 1) / (div) + 1))

void task1(std::stringstream &input) {
    std::string buf;

    int totQLvl = 0;

    while (std::getline(input, buf)) {
        int blueprintNum;
        int oreRCost;
        int clayRCost;
        struct {int ore; int clay;} obsRCost;
        struct {int ore; int obs;} geoRCost;

        std::queue<State> queue;

        std::stringstream ss(buf);
        ss.seekg(10);
        ss >> blueprintNum;
        ss.seekg(23, std::ios_base::cur);
        ss >> oreRCost;
        ss.seekg(28, std::ios_base::cur);
        ss >> clayRCost;
        ss.seekg(32, std::ios_base::cur);
        ss >> obsRCost.ore;
        ss.seekg(9, std::ios_base::cur);
        ss >> obsRCost.clay;
        ss.seekg(30, std::ios_base::cur);
        ss >> geoRCost.ore;
        ss.seekg(9, std::ios_base::cur);
        ss >> geoRCost.obs;

        int maxOreCost = std::max({oreRCost, clayRCost, obsRCost.ore, geoRCost.ore});
        int maxClayCost = obsRCost.clay;
        int maxGeodes = 0;

        queue.push({/* Minute */ 0, /* Resources */ 0, 0, 0, 0, /* Bots */ 1, 0, 0});

        while (!queue.empty()) {
            State sta = queue.front();
            queue.pop();

            if (sta.oreR < maxOreCost) {
                int forwardMinutes = ROUND_UP_DIV(oreRCost - sta.ore, sta.oreR) + 1;
                if (sta.minute + forwardMinutes < 24) {
                    State newState = fastForward(sta, forwardMinutes);
                    newState.oreR++;
                    newState.ore -= oreRCost;

                    queue.push(newState);
                }
            }

            if (sta.clayR < maxClayCost) {
                int forwardMinutes = ROUND_UP_DIV(clayRCost - sta.ore, sta.oreR) + 1;
                if (sta.minute + forwardMinutes < 24) {
                    State newState = fastForward(sta, forwardMinutes);
                    newState.clayR++;
                    newState.ore -= clayRCost;

                    queue.push(newState);
                }
            }

            // Obsidian
            if (sta.clayR > 0) {
                int forwardMinutes = std::max(ROUND_UP_DIV(obsRCost.clay - sta.clay, sta.clayR), ROUND_UP_DIV(obsRCost.ore - sta.ore, sta.oreR)) + 1;
                if (sta.minute + forwardMinutes < 24) {
                    State newState = fastForward(sta, forwardMinutes);
                    newState.obsR++;
                    newState.clay -= obsRCost.clay;
                    newState.ore -= obsRCost.ore;
                    queue.push(newState);
                }
            }

            // Geode
            if (sta.obsR > 0) {
                int forwardMinutes = std::max(ROUND_UP_DIV(geoRCost.ore - sta.ore, sta.oreR), ROUND_UP_DIV(geoRCost.obs - sta.obs, sta.obsR)) + 1;
                if (sta.minute + forwardMinutes < 24) {
                    State newState = fastForward(sta, forwardMinutes);
                    newState.geode += 24 - newState.minute;

                    if (maxGeodes < newState.geode) {
                        maxGeodes = newState.geode;
                    }
                    maxGeodes = std::max(maxGeodes, newState.geode);
                    newState.ore -= geoRCost.ore;
                    newState.obs -= geoRCost.obs;
                    queue.push(newState);
                }
            }
        }

        totQLvl += (blueprintNum * maxGeodes);
        std::cout << "Blueprint " << blueprintNum << " can create " << maxGeodes << " geodes." << std::endl;
    }

    std::cout << "Total quality is " << totQLvl << std::endl;
}

void task2(std::stringstream &input) {
    std::string buf;

    int totalGeodes = 1;

    while (std::getline(input, buf)) {
        int blueprintNum;
        int oreRCost;
        int clayRCost;
        struct {int ore; int clay;} obsRCost;
        struct {int ore; int obs;} geoRCost;

        std::queue<State> queue;

        std::stringstream ss(buf);
        ss.seekg(10);
        ss >> blueprintNum;
        ss.seekg(23, std::ios_base::cur);
        ss >> oreRCost;
        ss.seekg(28, std::ios_base::cur);
        ss >> clayRCost;
        ss.seekg(32, std::ios_base::cur);
        ss >> obsRCost.ore;
        ss.seekg(9, std::ios_base::cur);
        ss >> obsRCost.clay;
        ss.seekg(30, std::ios_base::cur);
        ss >> geoRCost.ore;
        ss.seekg(9, std::ios_base::cur);
        ss >> geoRCost.obs;

        if (blueprintNum > 3) break;

        int maxOreCost = std::max({oreRCost, clayRCost, obsRCost.ore, geoRCost.ore});
        int maxClayCost = obsRCost.clay;
        int maxGeodes = 0;

        queue.push({/* Minute */ 0, /* Resources */ 0, 0, 0, 0, /* Bots */ 1, 0, 0});

        while (!queue.empty()) {
            State sta = queue.front();
            queue.pop();

            if (sta.oreR < maxOreCost) {
                int forwardMinutes = ROUND_UP_DIV(oreRCost - sta.ore, sta.oreR) + 1;
                if (sta.minute + forwardMinutes < 32) {
                    State newState = fastForward(sta, forwardMinutes);
                    newState.oreR++;
                    newState.ore -= oreRCost;

                    queue.push(newState);
                }
            }

            if (sta.clayR < maxClayCost) {
                int forwardMinutes = ROUND_UP_DIV(clayRCost - sta.ore, sta.oreR) + 1;
                if (sta.minute + forwardMinutes < 32) {
                    State newState = fastForward(sta, forwardMinutes);
                    newState.clayR++;
                    newState.ore -= clayRCost;

                    queue.push(newState);
                }
            }

            // Obsidian
            if (sta.clayR > 0) {
                int forwardMinutes = std::max(ROUND_UP_DIV(obsRCost.clay - sta.clay, sta.clayR), ROUND_UP_DIV(obsRCost.ore - sta.ore, sta.oreR)) + 1;
                if (sta.minute + forwardMinutes < 32) {
                    State newState = fastForward(sta, forwardMinutes);
                    newState.obsR++;
                    newState.clay -= obsRCost.clay;
                    newState.ore -= obsRCost.ore;
                    queue.push(newState);
                }
            }

            // Geode
            if (sta.obsR > 0) {
                int forwardMinutes = std::max(ROUND_UP_DIV(geoRCost.ore - sta.ore, sta.oreR), ROUND_UP_DIV(geoRCost.obs - sta.obs, sta.obsR)) + 1;
                if (sta.minute + forwardMinutes < 32) {
                    State newState = fastForward(sta, forwardMinutes);
                    newState.geode += 32 - newState.minute;

                    maxGeodes = std::max(maxGeodes, newState.geode);
                    newState.ore -= geoRCost.ore;
                    newState.obs -= geoRCost.obs;
                    queue.push(newState);
                }
            }
        }

        totalGeodes *= maxGeodes;
        std::cout << "Blueprint " << blueprintNum << " can create " << maxGeodes << " geodes." << std::endl;
    }

    std::cout << "Total quality is " << totalGeodes << std::endl;
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