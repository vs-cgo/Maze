#ifndef _MAZE_MODEL_LEARNING_QLEARNING_H_
#define _MAZE_MODEL_LEARNING_QLEARNING_H_

#include <iostream>
#include <random>
#include <vector>

#include "../command/command.h"

namespace s21 {
/**
 * @brief The QLearning class выполняющий reinforcement learning.
 */
class QLearning {
 public:
  /**
   * @brief QLearning конструктор.
   * @param[in] r ряд конца пути.
   * @param[in] c столбей конца пути.
   * @param[in] data структура лабиринта полученная от модели.
   */
  QLearning(int r, int c, std::vector<std::vector<int>> &data);
  ~QLearning();
  /**
   * @brief Training метод выполняющий обучения агента на основе q-learning.
   */
  void Training();
  /**
   * @brief Get  метод для получения таблицы с результатами.
   * @return структуру с данными от обучения.
   */
  std::vector<std::vector<double>> Get();

 private:
  double GetMax(int state);
  int EgreedyPolicy(int state, std::vector<int> &v, int e = 10);
  int GetReward(int state, int action, int &next_state);
  bool IsPosibleAction(int state, int action, int &next_state);
  void InitQTable();
  std::vector<int> PossiblePath(int next_state);

 private:
  int row_, col_, sr_, sc_, fin;
  double alfa, gamma;
  std::vector<std::vector<int>> maze_;
  std::vector<std::vector<double>> q_table_;
};
}  // end namespace s21
#endif  // _MAZE_MODEL_LEARNING_QLEARNING_H_
