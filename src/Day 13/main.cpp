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

static const int DAY = 13;
static const std::string NAME = "Monkey in the Middle";

struct TreeNode {
    bool isInt;
    std::vector<TreeNode> list;
    int val;
};

struct GraphNode {
    unsigned int dist = UINT32_MAX;
    GraphNode *prev = nullptr;
    std::pair<int, int> pos;
    bool visited = false;
    int height;
};

void parseLine(TreeNode &root, std::string input) {
    for (int i = 0; i < input.size(); i++) {
        char c = input[i];
        
        if (c == ']') {
            return;
        } if (c == '[') {
            int nest = 1;
            size_t end = i;
            while (nest != 0) {
                end++;
                if (input[end] == '[') nest++;
                else if (input[end] == ']') nest--;
            }

            TreeNode leaf;
            leaf.isInt = false;
            parseLine(leaf, input.substr(i + 1, end - i));
            root.list.push_back(leaf);
            i = end + 1;
        } else {
            TreeNode leaf;
            leaf.isInt = true;
            size_t end = input.find(',', i);
            if (end == -1) end = input.size();
            leaf.val = std::stoi(input.substr(i, end));
            i = end;
            root.list.push_back(leaf);
        }
    }
}

int compareLeftRight(const TreeNode &left, const TreeNode &right) {
    for (int i = 0; i < std::min(left.list.size(), right.list.size()); i++) {
        const TreeNode &leftLeaf = left.list[i];
        const TreeNode &rightLeaf = right.list[i];

        if (leftLeaf.isInt && rightLeaf.isInt) {
            if (leftLeaf.val > rightLeaf.val)
                return -1;
            else if (leftLeaf.val < rightLeaf.val)
                return 1;
            /* Else check rest of input */
        } else {
            TreeNode leftInput = leftLeaf;
            TreeNode rightInput = rightLeaf;
            
            if (leftInput.isInt) {
                leftInput.isInt = false;
                leftInput.list.push_back({true, {}, leftInput.val});
            } else if (rightInput.isInt) {
                rightInput.isInt = false;
                rightInput.list.push_back({true, {}, rightInput.val});
            }

            int res = compareLeftRight(leftInput, rightInput);

            if (res != 0) return res;
        }
    }

    // Lists are equal aside from length
    if (left.list.size() < right.list.size()) return 1;
    else if (left.list.size() > right.list.size()) return -1;
    else return 0;
}

void task1(std::stringstream &input) {
    std::string buf1, buf2;

    int idx = 0;
    int sum = 0;

    while (std::getline(input, buf1)) {
        std::getline(input, buf2);
        input.seekg(1, std::ios_base::cur);
        idx++;

        TreeNode root1;
        root1.isInt = false;
        TreeNode root2;
        root2.isInt = false;

        parseLine(root1, buf1.substr(1, buf1.size() - 2));
        parseLine(root2, buf2.substr(1, buf2.size() - 2));

        int res = compareLeftRight(root1, root2);
        if (res == 1) sum += idx;

        std::cout << "Result for " << idx << " is " << res << std::endl;        
    }

    std::cout << "Sum of ordered indices is " << sum << std::endl;
}

struct GraphSort {
    inline bool operator() (const TreeNode &left, const TreeNode &right) {
        return compareLeftRight(left, right) == 1;
    }
};

void task2(std::stringstream &input) {
    std::string buf1;
    std::vector<TreeNode> queue;

    while (std::getline(input, buf1)) {
        if (buf1 == "") continue;
        TreeNode root1;
        root1.isInt = false;

        parseLine(root1, buf1.substr(1, buf1.size() - 2));
        queue.push_back(root1);
    }

    TreeNode two {false, {}, 0};
    two.list.push_back({false, {}, 0});
    two.list[0].list.push_back({true, {}, 2});

    TreeNode six {false, {}, 0};
    six.list.push_back({false, {}, 0});
    six.list[0].list.push_back({true, {}, 6});

    queue.push_back(two);
    queue.push_back(six);

    std::sort(queue.begin(), queue.end(), GraphSort());
    int key = 1;

    for (int i = 0; i < queue.size(); i++) {
        const TreeNode &node = queue[i];
        if (node.list.size() == 1 && node.list[0].list.size() == 1 && node.list[0].list[0].val == 2) {
            key *= i + 1;
        }
        if (node.list.size() == 1 && node.list[0].list.size() == 1 && node.list[0].list[0].val == 6) {
            key *= i + 1;
            break;
        }
    }

    std::cout << key << std::endl;
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