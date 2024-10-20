#include "gencave.h"

namespace s21 {
GenCave::GenCave(){};
std::vector<std::vector<int>> GenCave::Generation(int r, int c, int chance) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 99);
  auto cave = std::vector<std::vector<int>>(r, std::vector<int>(c, 0));
  for (int i = 0; i < r; ++i) {
    for (int k = 0; k < c; ++k) {
      int val = distrib(gen);
      if (val < chance) cave[i][k] = 1;
    }
  }
  return cave;
}
}  // end namespace s21
