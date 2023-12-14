#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

uint64_t tilt_north(std::vector<std::string>& lines);
uint64_t tilt_south(std::vector<std::string>& lines);
uint64_t tilt_east(std::vector<std::string>& lines);
uint64_t tilt_west(std::vector<std::string>& lines);

struct VectorHasher {
  int operator()(const std::vector<std::string>& V) const {
    int hash = V.size();
    for (const auto& i : V) {
      std::hash<std::string> h;
      hash ^= h(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
  }
};

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::ifstream f(argv[i]);
    if (!f) continue;
    std::vector<std::string> lines;
    {
      std::string line;
      while (std::getline(f, line)) {
        lines.push_back(line);
      }
    }

    std::ios_base::sync_with_stdio(true);
    int times = 1000000000;
    typedef std::vector<std::string> key_t;
    typedef std::unordered_map<key_t, uint64_t, VectorHasher> history_t;
    history_t history;
    std::unordered_map<uint64_t, history_t::iterator> history_index;    
    auto print_lines = [&]() {
      for (const std::string& line : lines) {
        std::cerr << line << std::endl;
      }
    };

    for (int seq = 0; seq < times; ++seq) {
      tilt_north(lines);
      tilt_west(lines);
      tilt_south(lines);
      tilt_east(lines);
      uint64_t load = 0;
      for (int line_index = 0; line_index < lines.size(); ++line_index) {
        for (int j = 0; j < lines[line_index].length(); ++j) {
          if (lines[line_index][j] == 'O') {
            load += (lines.size() - line_index);
          }
        }
      }
      //std::cerr << "Seq " << seq << " load " << load << std::endl;
      // print_lines();
      auto it = history.find(lines);
      if (it != history.end()) {
        int sequence_length = seq - it->second;
        int left = times - seq - 1;
        int offset = left % sequence_length;

        if (it->second + offset != seq) {
          auto iterator = history_index.find(it->second + offset);
          lines = iterator->second->first;
        }
        break;
      }

      auto it2 = history.insert(std::make_pair(lines, seq)).first;
      history_index.insert(std::make_pair(seq, it2));      
    }
    std::cerr << std::endl;

    uint64_t load = 0;
    for (int line_index = 0; line_index < lines.size(); ++line_index) {
      for (int j = 0; j < lines[line_index].length(); ++j) {
        if (lines[line_index][j] == 'O') {
          load += (lines.size() - line_index);
        }
      }
    }
    std::cerr << argv[i] << ": " << load << std::endl;  // 0=64, 1=100310
  }
  return 0;
}

uint64_t tilt_north(std::vector<std::string>& lines) {
  int width = lines.front().length();
  std::vector<int> free_index(width, -1);
  uint64_t x = 0;
  for (int line_index = 0; line_index < lines.size(); ++line_index) {
    for (int j = 0; j < width; ++j) {
      switch (lines[line_index][j]) {
        case '#':
          free_index[j] = -1;
          break;
        case 'O':
          if (free_index[j] != -1) {
            lines[free_index[j]][j] = 'O';
            x += j * (lines.size() - free_index[j]);
            lines[line_index][j] = '.';
            free_index[j]++;
          } else {
            x += j * (lines.size() - line_index);
          }
          break;
        case '.':
          if (free_index[j] == -1) free_index[j] = line_index;
          break;
        default:
          break;
      }
    }
  }
  return x;
}

uint64_t tilt_south(std::vector<std::string>& lines) {
  int width = lines.front().length();
  std::vector<int> free_index(width, -1);
  uint64_t x = 0;
  for (int line_index = lines.size() - 1; line_index >= 0; --line_index) {
    for (int j = 0; j < width; ++j) {
      switch (lines[line_index][j]) {
        case '#':
          free_index[j] = -1;
          break;
        case 'O':
          if (free_index[j] != -1) {
            lines[free_index[j]][j] = 'O';
            x += j * (lines.size() - free_index[j]);
            lines[line_index][j] = '.';
            free_index[j]--;
          } else {
            x += j * (lines.size() - line_index);
          }
          break;
        case '.':
          if (free_index[j] == -1) free_index[j] = line_index;
          break;
        default:
          break;
      }
    }
  }
  return x;
}

uint64_t tilt_west(std::vector<std::string>& lines) {
  int width = lines.front().length();
  int height = lines.size();
  std::vector<int> free_index(height, -1);
  uint64_t x = 0;
  for (int j = 0; j < width; ++j) {
    for (int line_index = 0; line_index < height; ++line_index) {
      switch (lines[line_index][j]) {
        case '#':
          free_index[line_index] = -1;
          break;
        case 'O':
          if (free_index[line_index] != -1) {
            lines[line_index][free_index[line_index]] = 'O';
            x += j * (lines.size() - line_index);
            lines[line_index][j] = '.';
            free_index[line_index]++;
          } else {
            x += j * (lines.size() - line_index);
          }
          break;
        case '.':
          if (free_index[line_index] == -1) free_index[line_index] = j;
          break;
        default:
          break;
      }
    }
  }
  return x;
}

uint64_t tilt_east(std::vector<std::string>& lines) {
  int width = lines.front().length();
  int height = lines.size();
  std::vector<int> free_index(height, -1);
  uint64_t x = 0;
  for (int j = width - 1; j >= 0; --j) {
    for (int line_index = 0; line_index < height; ++line_index) {
      switch (lines[line_index][j]) {
        case '#':
          free_index[line_index] = -1;
          break;
        case 'O':
          if (free_index[line_index] != -1) {
            lines[line_index][free_index[line_index]] = 'O';
            x += j * (lines.size() - line_index);
            lines[line_index][j] = '.';
            free_index[line_index]--;
          } else {
            x += j * (lines.size() - line_index);
          }
          break;
        case '.':
          if (free_index[line_index] == -1) free_index[line_index] = j;
          break;
        default:
          break;
      }
    }
  }
  return x;
}
