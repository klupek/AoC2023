#include <fstream>
#include <iostream>
#include <list>
#include <string>

int main(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    std::ifstream f(argv[i]);
    std::string input;
    uint64_t part1sum = 0;
    typedef std::pair<std::string, int> entry_t;
    std::list<entry_t> hashmap[256];
    while (std::getline(f, input, ',')) {
      int val = 0;
      for (unsigned char c : input) {
        if (c != ' ') {
          val = ((val + c) * 17) % 256;
        }
      }
      part1sum += val;
      val = 0;
      int j;
      std::string label;
      for (j = 0; j < input.length() && input[j] != '=' && input[j] != '-';
           ++j) {
        label += input[j];
        val = ((val + input[j]) * 17) % 256;
      }
      char op = input[j++];
      int number = 0;
      for (; j < input.length(); ++j) {
        number = number * 10 + input[j] - '0';
      }
      if (op == '=') {
        auto& bucket = hashmap[val];
        auto it = std::find_if(bucket.begin(), bucket.end(),
                            [=](const entry_t& e) { return e.first == label; });
        if (it == bucket.end()) {
          bucket.push_back(entry_t(label, number));
        } else {
          it->second = number;
        }
      } else if (op == '-') {
        auto& bucket = hashmap[val];
        bucket.erase(
            std::remove_if(bucket.begin(), bucket.end(),
                           [=](const entry_t& x) { return x.first == label; }),
            bucket.end());
      }
    }
    uint64_t part2 = 0;
    for (int j = 0; j < 256; ++j) {
      if (!hashmap[j].empty()) {
        std::cerr << "Bucket " << j << ": ";
        int slot = 1;
        for (const auto& x : hashmap[j]) {
          part2 += (j+1) * slot * x.second;
          std::cerr << "[ " << x.first << " " << x.second << " ] ";
          slot++;
        }
        std::cerr << std::endl;
      }
    }
    std::cerr << "Part 1: " << part1sum << std::endl;
    std::cerr << "Part 2: " << part2 << std::endl;
  }
  return 0;
}
