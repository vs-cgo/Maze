#include "command.h"

namespace s21 {
// open file
OpenCommand::OpenCommand(std::string str) : file_(str) {}
OpenCommand::~OpenCommand() {}
void OpenCommand::Execute(std::vector<std::vector<int>> &data) {
  std::ifstream is(file_);
  if (!is.is_open()) throw not_open();

  int row = 0, col = 0;
  char temp[256] = {0};
  if (is.getline(temp, 255).eof() || sscanf(temp, "%d%d", &row, &col) < 2 ||
      row < 1 || col < 1) {
    throw bad_file();
  }
  data = std::vector<std::vector<int>>(row, std::vector<int>(col, 0));
  for (int i = 0, half = row, add = 1;
       !is.getline(temp, 255).eof() || is.gcount();) {
    if (temp[0] < 48 || temp[0] > 57) continue;
    if (i >= half) {
      add = 2;
      i = 0;
    }
    char *ptr = temp, *end;
    for (int k = 0, x = 0; k < col; ++k) {
      x = strtol(ptr, &end, 10);
      if (ptr == end) throw bad_file();
      ptr = end;
      if (x > 0) data[i][k] += add;
    }
    ++i;
  }
}

// save maze
SaveCommand::SaveCommand(std::string str) : file_(str){};
SaveCommand::~SaveCommand() {}
void SaveCommand::Execute(std::vector<std::vector<int>> &data) {
  std::ofstream os(file_);
  if (!os.is_open()) throw not_open();

  int row = data.size(), col = data.size();
  std::string right, bot;

  os << row << ' ' << col << std::endl;

  for (int i = 0; i < row; ++i) {
    for (int k = 0; k < col; ++k) {
      right += (data[i][k] & Wall::kRight) ? '1' : '0';
      right += k < col - 1 ? ' ' : '\n';
      bot += (data[i][k] & Wall::kBot) ? '1' : '0';
      bot += k < col - 1 ? " " : (i < row - 1 ? "\n" : "");
    }
  }
  os << right << std::endl << bot;
}

// generate labirint
GenCommand::GenCommand(int row, int col)
    : row_(row), col_(col), rd(), gen(rd()), distrib(0, 1) {}
GenCommand::~GenCommand(){};

void GenCommand::Execute(std::vector<std::vector<int>> &data) {
  data = std::vector(row_, std::vector(col_, 0));
  std::vector<int> set(col_, 0);
  std::vector<int> line(col_, 0);
  for (int i = 0, end = row_ - 1; i < row_; ++i) {
    Fill(line, set);
    if (i != end) {
      RightWall(i, line, set, data);
      BotWall(i, line, set, data);
    } else {
      EndWall(i, line, data);
    }
  }
}
void GenCommand::Fill(std::vector<int> &line, std::vector<int> &set) {
  for (size_t i = 0; i < line.size(); ++i) {
    if (!line[i]) line[i] = GetSet(set);
  }
}
int GenCommand::GetSet(std::vector<int> &set) {
  for (size_t i = 0; i < set.size(); ++i) {
    if (!set[i]) {
      set[i] = 1;
      return i + 1;
    }
  }
  return -1;
}
void GenCommand::RightWall(int row, std::vector<int> &line,
                           std::vector<int> &set,
                           std::vector<std::vector<int>> &maze) {
  for (size_t i = 0; i < line.size(); ++i) {
    int wall = distrib(gen);
    if (wall || i == line.size() - 1) {
      maze[row][i] = 1;
    } else if (line[i] == line[i + 1]) {
      maze[row][i] = 1;
    } else {
      int set_from = line[i + 1], set_to = line[i];
      set[set_to - 1] += set[set_from - 1];
      set[set_from - 1] = 0;
      MergeSet(set_from, set_to, line);
    }
  }
}
void GenCommand::BotWall(int row, std::vector<int> &line, std::vector<int> &set,
                         std::vector<std::vector<int>> &maze) {
  for (size_t i = 0; i < line.size(); ++i) {
    int wall = distrib(gen);
    if (wall && set[line[i] - 1] > 1) {
      maze[row][i] += 2;
      set[line[i] - 1] -= 1;
      line[i] = 0;
    }
  }
}
void GenCommand::EndWall(int row, std::vector<int> &line,
                         std::vector<std::vector<int>> &maze) {
  for (size_t i = 0; i < line.size() - 1; ++i) {
    if (line[i] == line[i + 1]) {
      maze[row][i] = 3;
    } else {
      MergeSet(line[i + 1], line[i], line);
      maze[row][i] = 2;
    }
  }
  maze[row][line.size() - 1] = 3;
}
void GenCommand::MergeSet(int from, int to, std::vector<int> &line) {
  for (size_t i = 0; i < line.size(); ++i) {
    if (line[i] == from) line[i] = to;
  }
}

// GenerationCommand2
GenCommandS::GenCommandS(int r, int c)
    : row_(r),
      col_(c),
      maze(r, std::vector<Cell>(c, Cell())),
      rd(),
      gen(rd()),
      distrib(){};
GenCommandS::~GenCommandS() {}

void GenCommandS::Execute(std::vector<std::vector<int>> &data) {
  std::stack<std::pair<int, int>> stack;
  int x = 0, y = 0;
  stack.push({x, y});
  maze[y][x].visited = true;
  data = std::vector(row_, std::vector<int>(col_, 0));
  while (!stack.empty()) {
    x = stack.top().first;
    y = stack.top().second;
    stack.pop();

    std::vector<Direction> directions;

    if (isValid(x, y - 1) && !maze[y - 1][x].visited) directions.push_back(UP);
    if (isValid(x, y + 1) && !maze[y + 1][x].visited)
      directions.push_back(DOWN);
    if (isValid(x - 1, y) && !maze[y][x - 1].visited)
      directions.push_back(LEFT);
    if (isValid(x + 1, y) && !maze[y][x + 1].visited)
      directions.push_back(RIGHT);

    if (!directions.empty()) {
      stack.push({x, y});
      Direction dir = directions[distrib(gen) % directions.size()];
      switch (dir) {
        case UP:
          removeWall(x, y, x, y - 1);
          maze[y - 1][x].visited = true;
          stack.push({x, y - 1});
          break;
        case DOWN:
          removeWall(x, y, x, y + 1);
          maze[y + 1][x].visited = true;
          stack.push({x, y + 1});
          break;
        case LEFT:
          removeWall(x, y, x - 1, y);
          maze[y][x - 1].visited = true;
          stack.push({x - 1, y});
          break;
        case RIGHT:
          removeWall(x, y, x + 1, y);
          maze[y][x + 1].visited = true;
          stack.push({x + 1, y});
          break;
      }
    }
  }
  for (int i = 0; i < row_; ++i) {
    for (int k = 0; k < col_; ++k) {
      data[i][k] = maze[i][k].walls[1] * 2 + maze[i][k].walls[3];
    }
  }
}

bool GenCommandS::isValid(int x, int y) {
  return x >= 0 && x < col_ && y >= 0 && y < row_;
}
void GenCommandS::removeWall(int x1, int y1, int x2, int y2) {
  if (x1 == x2) {
    if (y1 < y2) {
      maze[y1][x1].walls[DOWN] = false;
      maze[y2][x2].walls[UP] = false;
    } else {
      maze[y1][x1].walls[UP] = false;
      maze[y2][x2].walls[DOWN] = false;
    }
  } else {
    if (x1 < x2) {
      maze[y1][x1].walls[RIGHT] = false;
      maze[y2][x2].walls[LEFT] = false;
    } else {
      maze[y1][x1].walls[LEFT] = false;
      maze[y2][x2].walls[RIGHT] = false;
    }
  }
}

// Solve Command
SolveCommand::SolveCommand(int sr, int sc, int er, int ec)
    : sr_(sr), sc_(sc), er_(er), ec_(ec), solve_(){};
SolveCommand::~SolveCommand(){};

void SolveCommand::Execute(std::vector<std::vector<int>> &data) {
  std::unordered_set<int> visited;
  std::unordered_map<int, int> parent;
  std::queue<int> queue;
  int row = data.size(), col = data[0].size();
  int start = (sr_ - 1) * col + sc_ - 1, end = (er_ - 1) * col + ec_ - 1,
      cur = -1;
  queue.push(start);
  visited.insert(start);
  parent[start] = -1;
  while (!queue.empty()) {
    cur = queue.front();
    queue.pop();
    visited.insert(cur);
    if (cur == end) break;
    int top = cur - col, bot = cur + col, left = cur - 1, right = cur + 1;
    if (top >= 0 && !(data[top / col][top % col] & Wall::kBot) &&
        !visited.count(top)) {
      queue.push(top);
      parent[top] = cur;
    }
    if (left >= 0 && cur % col != 0 &&
        !(data[left / col][left % col] & Wall::kRight) &&
        !visited.count(left)) {
      queue.push(left);
      parent[left] = cur;
    }
    if (bot / col < row && !(data[cur / col][cur % col] & Wall::kBot) &&
        !visited.count(bot)) {
      queue.push(bot);
      parent[bot] = cur;
    }
    if (cur % col < col - 1 && !(data[cur / col][cur % col] & Wall::kRight) &&
        !visited.count(right)) {
      queue.push(right);
      parent[right] = cur;
    }
  }
  solve_ = data;
  if (cur == end) AddPath(parent, end);
}
void SolveCommand::AddPath(std::unordered_map<int, int> &parent, int s) {
  for (int cur = parent[s], col = solve_[0].size(); cur >= 0;
       s = cur, cur = parent[s]) {
    if (abs(cur - s) == 1) {
      solve_[s / col][s % col] += (cur > s) ? Wall::kHorRight : Wall::kHorLeft;
    } else {
      solve_[s / col][s % col] += (cur > s) ? Wall::kVertBot : Wall::kVertTop;
    }
  }
}
std::vector<std::vector<int>> &SolveCommand::GetSolve() { return solve_; }

// AgentCommand
AgentCommand::AgentCommand(int sr, int sc, int er, int ec,
                           std::vector<std::vector<double>> &table)
    : sr_(sr), sc_(sc), er_(er), ec_(ec), maze_(), qtable_(table){};
AgentCommand::~AgentCommand(){};

void AgentCommand::Execute(std::vector<std::vector<int>> &data) {
  maze_ = data;
  int row = data.size(), col = 0;
  if (row > 0) col = data[0].size();
  if (!row || !col) return;
  int start = (sr_ - 1) * col + sc_ - 1, end = (er_ - 1) * col + ec_ - 1;
  maze_ = data;
  for (int i = 0; start != end; ++i) {
    int ind = GetMax(start);
    if (i > row * col) break;

    if (ind == UP) {
      maze_[start / col][start % col] += kVertTop;
      start -= col;
    } else if (ind == DOWN) {
      maze_[start / col][start % col] += kVertBot;
      start += col;
    } else if (ind == LEFT) {
      maze_[start / col][start % col] += kHorLeft;
      start -= 1;
    } else {
      maze_[start / col][start % col] += kHorRight;
      start += 1;
    }
  }
}
int AgentCommand::GetMax(int state) {
  int ind = 0;
  for (int i = 1; i < 4; ++i) {
    if (qtable_[state][i] > qtable_[state][ind]) ind = i;
  }
  return ind;
}
std::vector<std::vector<int>> &AgentCommand::GetSolve() { return maze_; }
}  // end namespace s21
