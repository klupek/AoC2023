#include <fstream>
#include <iostream>
#include <string>
#include <vector>
//#define DEBUG

int check(uint64_t a, uint64_t b, int bits) {
  int badbits = 0;
  while (bits > 0) {
    bool bitA = a % 2;
    bool bitB = b % 2;
    a /= 2;
    b /= 2;
    bits--;
    if (bitA != bitB) {
      badbits++;
    }
  }
  return badbits;
}

int doit(const std::vector<std::string>& source);

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    std::ifstream f(argv[i]);
    std::vector<std::string> source;
    int sum = 0;
    {
      std::string line;
      while (std::getline(f, line)) {
        if (line.empty()) {
          sum += doit(source);
          source.clear();
#ifdef DEBUG
          break;
#endif
        } else {
          source.push_back(line);
        }
      }
#ifndef DEBUG
      if (!source.empty()) {
        sum += doit(source);
      }
#endif
    }
    std::cerr << "Part 1: " << sum << std::endl;
  }
  return 0;
}

int doit(const std::vector<std::string>& source) {
  int width = source.front().length();
  int height = source.size();

  std::vector<std::vector<uint64_t>> columns_left(
      height, std::vector<uint64_t>(width, 0));
  std::vector<std::vector<uint64_t>> columns_right(
      height, std::vector<uint64_t>(width, 0));
  std::vector<std::vector<uint64_t>> rows_above(
      width, std::vector<uint64_t>(height, 0));
  std::vector<std::vector<uint64_t>> rows_below(
      width, std::vector<uint64_t>(height, 0));

  for (int row = 0; row < height; ++row) {
    columns_left[row][0] = (source[row][0] == '#');
    columns_right[row][width - 1] = (*source[row].rbegin() == '#');
    for (int column = 1; column < width; ++column) {
      int bit = source[row][column] == '#';
      int rbit = source[row][width - column - 1] == '#';
      columns_left[row][column] = columns_left[row][column - 1] * 2 + bit;
      columns_right[row][width - column - 1] =
          columns_right[row][width - column] * 2 + rbit;
    }
  }
  for (int column = 0; column < width; ++column) {
    rows_above[column][0] = source[0][column] == '#';
    rows_below[column][height - 1] = source[height - 1][column] == '#';
    for (int row = 1; row < height; ++row) {
      int bit = source[row][column] == '#';
      int rbit = source[height - row - 1][column] == '#';
      rows_above[column][row] = rows_above[column][row - 1] * 2 + bit;
      rows_below[column][height - row - 1] =
          rows_below[column][height - row] * 2 + rbit;
    }
  }

  int vertical = 0, horizontal = 0;
  int vertical2 = 0, horizontal2 = 0;

  for (int row = 1; row < height; ++row) {
    int badbits = 0;
    for (int column = 0; column < width; ++column) {
      int x = std::min(row, height - row);
      badbits += check(rows_above[column][row - 1], rows_below[column][row], x);
    }
    if (badbits == 0) {
      horizontal = row;
    } else if (badbits == 1) {
      horizontal2 = row;
    }
  }
  for (int column = 1; column < width; ++column) {    
    int badbits = 0;
    for (int row = 0; row < height; ++row) {
      int x = std::min(column, width - column);
      badbits += check(columns_left[row][column - 1], columns_right[row][column], x);      
    }
    if (badbits == 0) {
      vertical = column;
    } else if (badbits == 1) {
      vertical2 = column;
    }
  }
#ifdef DEBUG
  std::cerr << "Rows above: \n";
  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {
      std::cerr << rows_above[column][row] << "\t";
    }
    std::cerr << std::endl;
  }
  std::cerr << "Rows below: \n";
  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {
      std::cerr << rows_below[column][row] << "\t";
    }
    std::cerr << std::endl;
  }

  std::cerr << "Columns left: \n";
  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {
      std::cerr << columns_left[row][column] << "\t";
    }
    std::cerr << std::endl;
  }

  std::cerr << "Columns right: \n";
  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {
      std::cerr << columns_right[row][column] << "\t";
    }
    std::cerr << std::endl;
  }

  std::cerr << "Column diffs: \n";
  for (int row = 0; row < height; ++row) {
    std::cerr << "null\t";
    for (int column = 1; column < width; ++column) {
      int x = std::min(column, width - column);
      std::cerr << check(columns_left[row][column - 1],
                         columns_right[row][column], x)
                << "\t";
    }
    std::cerr << std::endl;
  }

  std::cerr << "Row diffs: \n";
  for (int column = 0; column < width; ++column) {
    std::cerr << "null\t";
  }
  std::cerr << std::endl;
  for (int row = 1; row < height; ++row) {
    ;
    for (int column = 0; column < width; ++column) {
      int x = std::min(row, height - row);
      std::cerr << check(rows_above[column][row - 1], rows_below[column][row],
                         x)
                << "\t";
    }
    std::cerr << std::endl;
  }
#endif
  //std::cerr << "row " << horizontal << ", column " << vertical << std::endl;
  std::cerr << "row2 " << horizontal2 << ", column2 " << vertical2 << std::endl;
  return vertical2 + horizontal2 * 100;
}
