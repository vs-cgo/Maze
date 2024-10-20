#include "maze_test.h"

namespace s21 {
TEST_F(MazeTest, Bad) {
  auto c = std::make_unique<OpenCommand>("example/example_bad.txt");
  EXPECT_THROW(controller.Run(c.get()), bad_file);
}
TEST_F(MazeTest, Open) {
  auto c = std::make_unique<OpenCommand>("example/example_1.txt");
  controller.Run(c.get());
  std::vector<std::vector<int>> maze = controller.Get();
  EXPECT_EQ(10, static_cast<int>(maze.size()));
  EXPECT_EQ(10, static_cast<int>(maze[0].size()));
  for (int i = 0; i < 10; i++) {
    EXPECT_TRUE(maze[i][9] & Wall::kRight);
    EXPECT_TRUE(maze[9][i] & Wall::kBot);
  }
}
TEST_F(MazeTest, GenMaze) {
  auto c = std::make_unique<GenCommand>(10, 10);
  controller.Run(c.get());
  std::vector<std::vector<int>> maze = controller.Get();
  EXPECT_EQ(10, static_cast<int>(maze.size()));
  EXPECT_EQ(10, static_cast<int>(maze[0].size()));
  for (int i = 0; i < 10; i++) {
    EXPECT_TRUE(maze[i][9] & Wall::kRight);
    EXPECT_TRUE(maze[9][i] & Wall::kBot);
  }
}
TEST_F(MazeTest, GenSMaze) {
  auto c = std::make_unique<GenCommandS>(10, 10);
  controller.Run(c.get());
  std::vector<std::vector<int>> maze = controller.Get();
  EXPECT_EQ(10, static_cast<int>(maze.size()));
  EXPECT_EQ(10, static_cast<int>(maze[0].size()));
  for (int i = 0; i < 10; i++) {
    EXPECT_TRUE(maze[i][9] & Wall::kRight);
    EXPECT_TRUE(maze[9][i] & Wall::kBot);
  }
}
TEST_F(MazeTest, GenMaze2) {
  auto c = std::make_unique<GenCommand>(5, 5);
  controller.Run(c.get());
  std::vector<std::vector<int>> maze = controller.Get();
  EXPECT_EQ(5, static_cast<int>(maze.size()));
  EXPECT_EQ(5, static_cast<int>(maze[0].size()));
  for (int i = 0; i < 5; i++) {
    EXPECT_TRUE(maze[i][4] & Wall::kRight);
    EXPECT_TRUE(maze[4][i] & Wall::kBot);
  }
}
TEST_F(MazeTest, GenMaze3) {
  auto c = std::make_unique<GenCommand>(1, 1);
  controller.Run(c.get());
  std::vector<std::vector<int>> maze = controller.Get();
  EXPECT_EQ(1, static_cast<int>(maze.size()));
  EXPECT_EQ(1, static_cast<int>(maze[0].size()));
  for (int i = 0; i < 1; i++) {
    EXPECT_TRUE(maze[i][0] & Wall::kRight);
    EXPECT_TRUE(maze[0][i] & Wall::kBot);
  }
}
TEST_F(MazeTest, GenMaze4) {
  auto c = std::make_unique<GenCommand>(15, 17);
  controller.Run(c.get());
  std::vector<std::vector<int>> maze = controller.Get();
  EXPECT_EQ(15, static_cast<int>(maze.size()));
  EXPECT_EQ(17, static_cast<int>(maze[0].size()));
  for (int i = 0; i < 15; i++) {
    EXPECT_TRUE(maze[i][16] & Wall::kRight);
  }
  for (int i = 0; i < 17; i++) {
    EXPECT_TRUE(maze[14][i] & Wall::kBot);
  }
}
TEST_F(MazeTest, SaveMaze) {
  auto c = std::make_unique<GenCommand>(5, 5);
  controller.Run(c.get());
  std::vector<std::vector<int>> maze = controller.Get();
  EXPECT_EQ(5, static_cast<int>(maze.size()));
  EXPECT_EQ(5, static_cast<int>(maze[0].size()));
  auto s = std::make_unique<SaveCommand>("example/save_test_5x5.txt");
  controller.Run(s.get());
}
TEST_F(MazeTest, Solve) {
  auto c = std::make_unique<GenCommand>(50, 50);
  controller.Run(c.get());
  std::vector<std::vector<int>> maze = controller.Get();
  EXPECT_EQ(50, static_cast<int>(maze.size()));
  EXPECT_EQ(50, static_cast<int>(maze[0].size()));
  int count_path = bfs(maze, {10, 4}, {34, 19});
  EXPECT_EQ(1, count_path);
}
TEST_F(MazeTest, Solve2) {
  auto c = std::make_unique<GenCommand>(1, 1);
  controller.Run(c.get());
  std::vector<std::vector<int>> maze = controller.Get();
  EXPECT_EQ(1, static_cast<int>(maze.size()));
  EXPECT_EQ(1, static_cast<int>(maze[0].size()));
  int count_path = bfs(maze, {0, 0}, {0, 0});
  EXPECT_EQ(1, count_path);
}
TEST_F(MazeTest, Solve3) {
  auto c = std::make_unique<GenCommand>(10, 15);
  controller.Run(c.get());
  std::vector<std::vector<int>> maze = controller.Get();
  EXPECT_EQ(10, static_cast<int>(maze.size()));
  EXPECT_EQ(15, static_cast<int>(maze[0].size()));
  int count_path = bfs(maze, {0, 9}, {8, 13});
  EXPECT_EQ(1, count_path);
}
TEST_F(MazeTest, Solve4) {
  auto g = std::make_unique<GenCommand>(10, 15);
  controller.Run(g.get());
  auto s = std::make_unique<SolveCommand>(1, 4, 9, 15);
  controller.Run(s.get());
  std::vector<std::vector<int>> maze = s.get()->GetSolve();
  int r = 8, c = 14;
  while (true) {
    if (maze[r][c] & kVertTop) {
      --r;
    } else if (maze[r][c] & kVertBot) {
      ++r;
    } else if (maze[r][c] & kHorRight) {
      ++c;
    } else if (maze[r][c] & kHorLeft) {
      --c;
    } else {
      break;
    }
  }
  EXPECT_EQ(0, r);
  EXPECT_EQ(3, c);
}

TEST_F(MazeTest, GenCave) {
  auto cave = gencave.Generation(10, 10, 50);
  EXPECT_EQ(10, static_cast<int>(cave.size()));
  EXPECT_EQ(10, static_cast<int>(cave[0].size()));
  int count = 0;
  for (size_t i = 0; i < cave.size(); i++) {
    for (size_t k = 0; k < cave[0].size(); ++k) {
      EXPECT_TRUE(cave[i][k] >= 0 && cave[i][k] <= 1);
      if (cave[i][k]) ++count;
    }
  }
  EXPECT_TRUE(count >= 35 && count <= 65);
}
TEST_F(MazeTest, GenCave2) {
  auto cave = gencave.Generation(50, 50, 80);
  EXPECT_EQ(50, static_cast<int>(cave.size()));
  EXPECT_EQ(50, static_cast<int>(cave[0].size()));
  int count = 0;
  for (size_t i = 0; i < cave.size(); i++) {
    for (size_t k = 0; k < cave[0].size(); ++k) {
      EXPECT_TRUE(cave[i][k] >= 0 && cave[i][k] <= 1);
      if (cave[i][k]) ++count;
    }
  }
  EXPECT_TRUE(count >= 1750 && count <= 2250);
}
TEST_F(MazeTest, GenCave3) {
  auto cave = gencave.Generation(15, 15, 100);
  EXPECT_EQ(15, static_cast<int>(cave.size()));
  EXPECT_EQ(15, static_cast<int>(cave[0].size()));
  for (size_t i = 0; i < cave.size(); i++) {
    for (size_t k = 0; k < cave[0].size(); ++k) {
      EXPECT_TRUE(cave[i][k] == 1);
    }
  }
}
TEST_F(MazeTest, GenCave4) {
  auto cave = gencave.Generation(30, 30, 0);
  EXPECT_EQ(30, static_cast<int>(cave.size()));
  EXPECT_EQ(30, static_cast<int>(cave[0].size()));
  for (size_t i = 0; i < cave.size(); i++) {
    for (size_t k = 0; k < cave[0].size(); ++k) {
      EXPECT_TRUE(cave[i][k] == 0);
    }
  }
}
TEST_F(MazeTest, QLearning) {
  auto c = std::make_unique<GenCommand>(10, 10);
  controller.Run(c.get());
  controller.Train(10, 10);
  auto a = std::make_unique<AgentCommand>(1, 1, 10, 10, controller.GetTable());
  controller.Run(a.get());
  std::vector<std::vector<int>> maze = a.get()->GetSolve();
  EXPECT_EQ(10, static_cast<int>(maze.size()));
  EXPECT_EQ(10, static_cast<int>(maze[0].size()));
  int row = 0, col = 0;
  for (int i = 0; (row != 9 || col != 9) && i < 100; ++i) {
    if (maze[row][col] & kVertTop) {
      --row;
    } else if (maze[row][col] & kVertBot) {
      ++row;
    } else if (maze[row][col] & kHorRight) {
      ++col;
    } else if (maze[row][col] & kHorLeft) {
      --col;
    }
    EXPECT_TRUE(row >= 0 && row < 10);
    EXPECT_TRUE(col >= 0 && col < 10);
  }
  EXPECT_TRUE(row == 9 && col == 9);
}
TEST_F(MazeTest, QLearning2) {
  auto c = std::make_unique<GenCommand>(20, 20);
  controller.Run(c.get());
  controller.Train(16, 16);
  auto a = std::make_unique<AgentCommand>(1, 1, 16, 16, controller.GetTable());
  controller.Run(a.get());
  std::vector<std::vector<int>> maze = a.get()->GetSolve();
  EXPECT_EQ(20, static_cast<int>(maze.size()));
  EXPECT_EQ(20, static_cast<int>(maze[0].size()));
  int row = 0, col = 0;
  for (int i = 0; (row != 15 || col != 15) && i < 400; ++i) {
    if (maze[row][col] & kVertTop) {
      --row;
    } else if (maze[row][col] & kVertBot) {
      ++row;
    } else if (maze[row][col] & kHorRight) {
      ++col;
    } else if (maze[row][col] & kHorLeft) {
      --col;
    }
    EXPECT_TRUE(row >= 0 && row < 20);
    EXPECT_TRUE(col >= 0 && col < 20);
  }
  EXPECT_TRUE((row == 15 && col == 15));
}
TEST_F(MazeTest, QLearning3) {
  auto c = std::make_unique<GenCommand>(20, 20);
  controller.Run(c.get());
  controller.Train(18, 17);
  auto a = std::make_unique<AgentCommand>(1, 1, 18, 17, controller.GetTable());
  controller.Run(a.get());
  std::vector<std::vector<int>> maze = a.get()->GetSolve();
  EXPECT_EQ(20, static_cast<int>(maze.size()));
  EXPECT_EQ(20, static_cast<int>(maze[0].size()));
  int row = 0, col = 0;
  for (int i = 0; (row != 17 || col != 16) && i < 500; ++i) {
    if (maze[row][col] & kVertTop) {
      --row;
    } else if (maze[row][col] & kVertBot) {
      ++row;
    } else if (maze[row][col] & kHorRight) {
      ++col;
    } else if (maze[row][col] & kHorLeft) {
      --col;
    }
    EXPECT_TRUE(row >= 0 && row < 20);
    EXPECT_TRUE(col >= 0 && col < 20);
  }
  EXPECT_TRUE((row == 17 && col == 16));

  auto b = std::make_unique<AgentCommand>(15, 1, 18, 17, controller.GetTable());
  controller.Run(b.get());
  maze = b.get()->GetSolve();
  row = 14, col = 0;
  for (int i = 0; (row != 17 || col != 16) && i < 500; ++i) {
    if (maze[row][col] & kVertTop) {
      --row;
    } else if (maze[row][col] & kVertBot) {
      ++row;
    } else if (maze[row][col] & kHorRight) {
      ++col;
    } else if (maze[row][col] & kHorLeft) {
      --col;
    }
    EXPECT_TRUE(row >= 0 && row < 20);
    EXPECT_TRUE(col >= 0 && col < 20);
  }
  EXPECT_TRUE((row == 17 && col == 16));

  auto d = std::make_unique<AgentCommand>(1, 20, 18, 17, controller.GetTable());
  controller.Run(d.get());
  maze = d.get()->GetSolve();
  row = 0, col = 19;
  for (int i = 0; (row != 17 || col != 16) && i < 500; ++i) {
    if (maze[row][col] & kVertTop) {
      --row;
    } else if (maze[row][col] & kVertBot) {
      ++row;
    } else if (maze[row][col] & kHorRight) {
      ++col;
    } else if (maze[row][col] & kHorLeft) {
      --col;
    }
    EXPECT_TRUE(row >= 0 && row < 20);
    EXPECT_TRUE(col >= 0 && col < 20);
  }
  EXPECT_TRUE((row == 17 && col == 16));
}
int MazeTest::bfs(std::vector<std::vector<int>> &maze,
                  std::pair<int, int> start, std::pair<int, int> end) {
  int row = maze.size(), col = 0, count = 0;
  if (row) col = maze[0].size();

  std::queue<std::pair<int, int>> q;
  std::vector<std::vector<bool>> visited(row, std::vector<bool>(col, false));
  q.push(start);
  while (!q.empty()) {
    auto [r, c] = q.front();
    q.pop();
    visited[r][c] = true;
    if (r == end.first && c == end.second) {
      ++count;
      continue;
    }
    if (c - 1 >= 0 && !visited[r][c - 1] && !(maze[r][c - 1] & kRight))
      q.emplace(r, c - 1);
    if (c + 1 < col && !visited[r][c + 1] && !(maze[r][c] & kRight))
      q.emplace(r, c + 1);
    if (r - 1 >= 0 && !visited[r - 1][c] && !(maze[r - 1][c] & kBot))
      q.emplace(r - 1, c);
    if (r + 1 < row && !visited[r + 1][c] && !(maze[r][c] & kBot))
      q.emplace(r + 1, c);
  }
  return count;
}

}  // end namespace s21

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
