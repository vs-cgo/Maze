#ifndef _MAZE_MODEL_MODEL_H
#define _MAZE_MODEL_MODEL_H

#include <vector>

#include "./command/command.h"
#include "./learning/qlearning.h"

namespace s21 {
/**
 * @brief The Model class для хранения лабиринта и таблицы после q-learning.
 */
class Model {
 public:
  Model();
  ~Model();
  /**
   * @brief Run метод для запуска производных команд по указателю на базовый
   * класс.
   * @param[in] c указатель на команду.
   */
  void Run(Command *c);
  /**
   * @brief Train метод для запуска обучения агента.
   * @param[in] row ряд для целевой ячейки.
   * @param[in] col столбей для целевой ячейки.
   */
  void Train(int row, int col);
  /**
   * @brief Get метод для получения лабиринта.
   * @return лабиринит
   */
  std::vector<std::vector<int>> &Get();
  /**
   * @brief GetTable для получения q-table.
   * @return таблица.
   */
  std::vector<std::vector<double>> &GetTable();

 private:
  std::vector<std::vector<int>> data_;
  std::vector<std::vector<double>> q_table_;
};

}  // end namespace s21
#endif  // _MAZE_MODEL_MODEL_H
