#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>

int main(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    std::vector<std::string> cosmos;
    std::vector<int> rowMultiplier, columnMultiplier;
    std::vector<std::pair<int, int>> galaxies;
    std::string line;
    std::ifstream f(argv[i]);
    std::cout << "Reading " << argv[i] << std::endl;
    while (std::getline(f, line)) {
      cosmos.push_back(line);
    }
    for (const std::string& row : cosmos) {
      int count = std::accumulate(row.begin(), row.end(), 0,
                                  [](int s, char c) { return c == '#' ? s+1 : s; });
      rowMultiplier.push_back(count == 0 ? 2 : 1);
    }
    for (int i = 0; i < cosmos.front().length(); ++i) {
      int count = std::accumulate(
          cosmos.begin(), cosmos.end(), 0,
          [=](int s, std::string row) { return row[i] == '#' ? s+1 : s; });
      columnMultiplier.push_back(count == 0 ? 2 : 1);
    }
    for (int i = 0; i < cosmos.size(); ++i) {
      for (int j = 0; j < cosmos[i].length(); ++j) {
        if (cosmos[i][j] == '#') {
          galaxies.emplace_back(i, j);
        }
      }
    }
    uint64_t distanceSumPart1 = 0;
    uint64_t distanceSumPart2 = 0;
    for (int i = 0; i < galaxies.size(); ++i) {
      for (int j = i + 1; j < galaxies.size(); ++j) {
        int distance = 0, y1, y2, x1, x2;
        std::tie(y1, x1) = galaxies[i];
        std::tie(y2, x2) = galaxies[j];
        if (y2 < y1) std::swap(y1, y2);
        if (x2 < x1) std::swap(x1, x2);

        for (int k = x1+1; k <= x2; ++k) {
          distanceSumPart1 += (columnMultiplier[k] == 2 ? 2 : 1);
          distanceSumPart2 += (columnMultiplier[k] == 2 ? 1000000 : 1);          
        }
        for (int k = y1+1; k <= y2; ++k) {
          distanceSumPart1 += (rowMultiplier[k] == 2 ? 2 : 1);
          distanceSumPart2 += (rowMultiplier[k] == 2 ? 1000000 : 1);          
        }        
      }
    }
    std::cout << argv[i] << "part 1: " << distanceSumPart1 << " part 2: " << distanceSumPart2 << std::endl;
  }
}
