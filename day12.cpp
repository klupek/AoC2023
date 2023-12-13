#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

uint64_t recursive_check(const std::string& line,
                         const std::vector<int>& badgroups,
                         std::map<std::pair<int, int>, uint64_t>& cache,
                         int line_index, int group_index);

bool check_n_bytes(const std::string& line, int index, int count) {
  if (index + count > line.length()) return false;
  for (int i = 0; i < count && i + index < line.length(); ++i) {
    if (!(line[index + i] == '#' || line[index + i] == '?')) return false;
  }
  if (index + count  < line.length() && line[index + count] == '#')
    return false;
  else
    return true;
}

int main(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    std::ifstream f(argv[i]);
    std::string line;
    uint64_t sumAll = 0;
    int multiplier = 5;
    int lineNumber = 0;
    while (std::getline(f, line)) {
      lineNumber++;

      uint64_t sum = 0;
      std::vector<int> unknown;
      std::vector<int> badgroups;
      std::string left;
      int badcount = 0;
      int i;
      int badpipes = 0;
      std::map<std::pair<int, int>, uint64_t> cache;
      for (i = 0; i < line.length() && line[i] != ' '; ++i) {
        left += line[i];
      }
      std::string tmp = left;
      for (int i = 1; i < multiplier; ++i) {
        left += "?" + tmp;
      }

      std::string right = line.substr(i);
      for (int j = 0; j < left.length() && left[j] != ' '; ++j) {
        if (left[j] == '?')
          unknown.push_back(j);
        else if (left[j] == '#')
          badpipes++;
      }
      {
        std::istringstream iss(right);
        int v;
        while (iss >> v) {
          iss.ignore(1);
          badgroups.push_back(v);
          badcount += v;
        }
      }
      badcount *= multiplier;
      {
        auto tmp = badgroups;
        for (int k = 1; k < multiplier; ++k) {
          for (int x : tmp) {
            badgroups.push_back(x);
          }
        }
      }

      sum = recursive_check(left, badgroups, cache, 0, 0);
      std::cerr << " - " << lineNumber << ":" <<  sum;
      sumAll += sum; 
    }

    std::cerr << "Sum " << sumAll << std::endl; // 1909291258644
  }
}

uint64_t recursive_check(const std::string& line,
                         const std::vector<int>& badgroups,
                         std::map<std::pair<int, int>, uint64_t>& cache,
                         int line_index, int group_index) {
  uint64_t result = 0;
  if (group_index == badgroups.size()) {
    bool good = true;
    for (int i = line_index; i < line.length() && good; ++i) {
      if (line[i] == '#') good = false;
    }
    if (good)
      return 1;
    else
      return 0;
  } else if (line_index >= line.length()) {
    return 0;
  }
  auto it = cache.find(std::make_pair(line_index, group_index));
  if (it != cache.end()) {
    return it->second;
  }

  const int group_count = badgroups[group_index];
  int badPipesRight = 0;
  for (int k = group_index + 1; k < badgroups.size(); ++k) {
    badPipesRight += badgroups[k];
  }

  for (int i = line_index;
       i < line.length() && line.length() - i >= badPipesRight; ++i) {
    if (line[i] == '#' || line[i] == '?') {
      if (check_n_bytes(line, i, group_count)) {
        const std::string& nextline = line;
        result += recursive_check(nextline, badgroups, cache,
                                  i + group_count + 1, group_index + 1);
      }
      if (line[i] == '#') break;
    }
  }

  cache[std::make_pair(line_index, group_index)] = result;
  return result;
}
