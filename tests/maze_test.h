#ifndef _MAZE_TESTS_MAZE_TEST_H_
#define _MAZE_TESTS_MAZE_TEST_H_

#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "../Maze/controller/controller.h"
#include "../Maze/view/cave/gen/gencave.h"

namespace s21 {
class MazeTest : public testing::Test {
 protected:
  MazeTest()
      : controller(),
        gencave(),
        maze1("../example/example_1.txt"),
        maze2("../example/example_2.txt"),
        maze3("../example/example_3.txt"),
        maze4("../example/example_4.txt"),
        cave1("./example/cave_example_1.txt"),
        cave2("../example/cave_example_1.txt"),
        cave3("../example/cave_example_1.txt"){};
  ~MazeTest(){};

  void TearDown() override{};
  void SetUp() override{};
  int bfs(std::vector<std::vector<int>> &, std::pair<int, int> start,
          std::pair<int, int> end);
  Controller controller;
  GenCave gencave;
  std::string maze1, maze2, maze3, maze4, cave1, cave2, cave3;
};
}  // end namespace s21
#endif  // MAZE_TESTS_MAZE_TEST_H_
