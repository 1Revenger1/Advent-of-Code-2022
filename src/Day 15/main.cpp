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

static const int DAY = 15;
static const std::string NAME = "Monkey in the Middle";

typedef std::pair<int, int> range;

void task1(std::stringstream &input) {
    std::vector<range> ranges;
    std::string buf;

    std::unordered_set<int> beacons;

    const int SCAN_Y = 2000000;

    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        ss.seekg(12);

        std::pair<int, int> sensor;
        std::pair<int, int> beacon;

        ss >> sensor.first;
        ss.seekg(4, std::ios_base::cur);
        ss >> sensor.second;

        ss.seekg(25, std::ios_base::cur);
        ss >> beacon.first;
        ss.seekg(4, std::ios_base::cur);
        ss >> beacon.second;

        if (beacon.second == SCAN_Y) {
            beacons.insert(beacon.first);
        }

        int scanRange = std::abs(beacon.first - sensor.first) + std::abs(beacon.second - sensor.second);

        if (sensor.second > SCAN_Y && sensor.second - scanRange > SCAN_Y) continue;
        else if (sensor.second + scanRange < SCAN_Y) continue;

        int width = (scanRange - std::abs(SCAN_Y - sensor.second));

        range r(sensor.first - width, sensor.first + width);
        ranges.push_back(r);
    }

    int tot = 0;
    std::sort(ranges.begin(), ranges.end());

    std::vector<range> simRanges = {ranges[0]};
    for (range &r : ranges) {
        range &lastR = simRanges.back();

        // Overlap
        if (r.first <= lastR.second) {
            lastR.second = std::max(r.second, lastR.second);
        } else {
            simRanges.push_back(r);
        }
    }

    for (const range &r : simRanges) {
        tot += r.second - r.first + 1;
        for (const int b : beacons) {
            if (b >= r.first && b <= r.second) {
                tot--;
            }
        }
    }

    std::cout << "Unusable tiles: " << tot << std::endl;
}

struct Scanner {
    int x; int y; int range;
};

bool scanLine(int y, const std::vector<Scanner> &scanners) {
    std::vector<range> ranges;

    for (Scanner scan : scanners) {
        if (scan.y > y && scan.y - scan.range > y) continue;
        else if (scan.y + scan.range < y) continue;

        int width = (scan.range - std::abs(y - scan.y));

        range r(scan.x - width, scan.x + width);
        ranges.push_back(r);
    }

    std::sort(ranges.begin(), ranges.end());

    std::vector<range> simRanges = {ranges[0]};
    for (range &r : ranges) {
        range &lastR = simRanges.back();

        // Overlap
        if (r.first <= lastR.second) {
            lastR.second = std::max(r.second, lastR.second);
        } else {
            simRanges.push_back(r);
        }
    }

    if (simRanges[0].first <= 0 && simRanges[0].second >= 4000000) return false;

    std::cout << "Transmission power is: " << (uint64_t) (simRanges[0].second + 1) * (uint64_t) 4000000 + y << std::endl;
    return true;
}

struct SortScanners {
  bool operator() (const Scanner &left, const Scanner &right) { return left.x < right.x;}
};

void task2(std::stringstream &input) {
    std::string buf;

    std::vector<Scanner> scanners;

    while (std::getline(input, buf)) {
        std::stringstream ss(buf);
        ss.seekg(12);
        
        Scanner scan;

        std::pair<int, int> beacon;

        ss >> scan.x;
        ss.seekg(4, std::ios_base::cur);
        ss >> scan.y;

        ss.seekg(25, std::ios_base::cur);
        ss >> beacon.first;
        ss.seekg(4, std::ios_base::cur);
        ss >> beacon.second;

        scan.range = std::abs(beacon.first - scan.x) + std::abs(beacon.second - scan.y);
        scanners.push_back(scan);
    }

    std::sort(scanners.begin(), scanners.end(), SortScanners());

    for (int i = 0; i < 4000000; i++) {
        if (scanLine(i, scanners)) break;
    }
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