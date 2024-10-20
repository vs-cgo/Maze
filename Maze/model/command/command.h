#ifndef _MAZE_MODEL_COMMAND_COMMAND_H_
#define _MAZE_MODEL_COMMAND_COMMAND_H_

#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
/**
 *\namespace s21 пространство имен в котором работают данные классы
 */
namespace s21 {
/**
 * @brief The not_open struct исключение, выбрасывается если файл не был открыт.
 */
struct not_open {};
/**
 * @brief The bad_file struct, выбрасывается если некорректный формат(пр.:
 * размеры меньше 1, не хватает данных...)
 */
struct bad_file {};

/*! \enum The Wall enum для получения состояния ячейки.
 */
enum Wall {
  kRight = 1,     /*!< правая стенка */
  kBot,           /*!< нижняя стенка */
  kVertTop = 4,   /*!< путь в верхнюю ячейку */
  kVertBot = 8,   /*!< путь в нижнююю ячейку */
  kHorRight = 16, /*!< путь в правую ячейку */
  kHorLeft = 32   /*!< путь в левую ячейку */
};
/*! \enum s21::Direction The Direction enum для генерации красивого лабиринта.
 */
enum Direction {
  UP,   /*!< вверх */
  DOWN, /*!< вниз */
  LEFT, /*!< влево */
  RIGHT /*!< вправо */
};
/**
 * @brief The Cell struct с информацией.
 */
struct Cell {
  bool visited = false; /*!< переменнная для индикации посещения ячейки. */
  bool walls[4] = {true, true, true,
                   true}; /*!< наличие стенок у данной ячейки. */
};
/**
 * @brief The Command class абстрактный класс.
 */
class Command {
 public:
  virtual ~Command(){};
  /**
   * @brief Execute чистый метод для переопределения в производном классе.
   * @param[in] data лабиринт передается из модели.
   */
  virtual void Execute(std::vector<std::vector<int>> &data) = 0;
};
/**
 * @brief The OpenCommand class для открытия лабиринта.
 */
class OpenCommand : public Command {
 public:
  /**
   * @brief OpenCommand конструктор.
   * @param[in] str путь до файла на диске.
   */
  OpenCommand(std::string str);
  ~OpenCommand();
  /**
   * @brief Execute метод для открытия файла, парсинга его и сохранения на диске
   * @param[in, out] data массив для сохранения лабиринта.
   */
  void Execute(std::vector<std::vector<int>> &data) override;

 private:
  std::string file_;
};
/**
 * @brief The SaveCommand class для сохранения лабиринта на диск.
 */
class SaveCommand : public Command {
 public:
  /**
   * @brief SaveCommand конструктор.
   * @param[in] str путь до файла где надо сохранить.
   */
  SaveCommand(std::string str);
  ~SaveCommand();
  /**
   * @brief Execute метод для выполнения команды.
   * @param[in, out] data лабиринт откуда берутся данные.
   */
  void Execute(std::vector<std::vector<int>> &data) override;

 private:
  std::string file_;
};

// Generation maze
/**
 * @brief The GenCommand class генерация лабиринта по Эллеру.
 */
class GenCommand : public Command {
 public:
  /**
   * @brief GenCommand конструктор.
   * @param[in] row количество рядов
   * @param[in] col количество столбцов
   */
  GenCommand(int row = 1, int col = 1);
  ~GenCommand();
  /**
   * @brief Execute метод для вызова генерации лабиринта.
   * @param[out] data ссылка для сохранения сгенерированного лабиринта в модель.
   */
  void Execute(std::vector<std::vector<int>> &data) override;

 private:
  void Fill(std::vector<int> &line, std::vector<int> &set);
  int GetSet(std::vector<int> &set);
  void RightWall(int row, std::vector<int> &line, std::vector<int> &set,
                 std::vector<std::vector<int>> &maze);
  void BotWall(int row, std::vector<int> &line, std::vector<int> &set,
               std::vector<std::vector<int>> &maze);
  void EndWall(int row, std::vector<int> &line,
               std::vector<std::vector<int>> &maze);
  void MergeSet(int from, int to, std::vector<int> &line);

 private:
  int row_, col_;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> distrib;
};

// Generation2 maze
/**
 * @brief The GenCommandS class 2 способ генерации лабиринта
 */
class GenCommandS : public Command {
 public:
  /**
   * @brief GenCommandS конструктор.
   * @param[in] row количество рядов
   * @param[in] col количество столбцов
   */
  GenCommandS(int row = 1, int col = 1);
  ~GenCommandS();
  /**
   * @brief Execute метод для выполнения команды.
   * @param[out] data ссылка по которой сохраняется полученный лабиринт.
   */
  void Execute(std::vector<std::vector<int>> &data) override;

 private:
  bool isValid(int x, int y);
  void removeWall(int x1, int y1, int x2, int y2);

 private:
  int row_, col_;
  std::vector<std::vector<Cell>> maze;
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> distrib;
};

// Solve maze
/**
 * @brief The SolveCommand class нахождение из точки a в точку b.
 */
class SolveCommand : public Command {
 public:
  /**
   * @brief SolveCommand конструтктор.
   * @param[in] sr ряд начала пути.
   * @param[in] sc столбец начала пути.
   * @param[in] er ряд конец пути.
   * @param[in] ec столбец конца пути.
   */
  SolveCommand(int sr = 1, int sc = 1, int er = 1, int ec = 1);
  ~SolveCommand();
  /**
   * @brief Execute метод для исполнения команды.
   * @param[in] data структура с данными лабиринта.
   */
  void Execute(std::vector<std::vector<int>> &data) override;
  /**
   * @brief GetSolve метод для получения лабиринта с решением.
   * @return лабиринт с отмеченным реением для отображения во вью.
   */
  std::vector<std::vector<int>> &GetSolve();

 private:
  void AddPath(std::unordered_map<int, int> &parent, int b);

 private:
  int sr_, sc_, er_, ec_;
  std::vector<std::vector<int>> solve_;
};
/**
 * @brief The AgentCommand class для получения решения нахождения пути
 * посредством q-learning.
 */
class AgentCommand : public Command {
 public:
  /**
   * @brief AgentCommand конструктор.
   * @param[in] sr ряд начала пути.
   * @param[in] sc столбец начала пути.
   * @param[in] er ряд конца пути.
   * @param[in] ec столбец конца пути.
   * @param[in] table таблица полученная после обучения.
   */
  AgentCommand(int sr, int sc, int er, int ec,
               std::vector<std::vector<double>> &table);
  ~AgentCommand();
  /**
   * @brief Execute метод для внесения пути в лабиринт.
   * @param[in] data лабиринт где будет сохранен путь.
   */
  void Execute(std::vector<std::vector<int>> &data) override;
  /**
   * @brief GetSolve метод для получения лабиринта с информацией о пути.
   * @return готовый лабиринт для отображения во вью;
   */
  std::vector<std::vector<int>> &GetSolve();

 private:
  int GetMax(int state);

 private:
  int sr_, sc_, er_, ec_;
  std::vector<std::vector<int>> maze_;
  std::vector<std::vector<double>> qtable_;
};
}  // end namespace s21
#endif  //_MAZE_MODEL_COMMAND_COMMAND_H_
