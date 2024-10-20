#ifndef _MAZE_CONTROLLER_CONTROLLER_H_
#define _MAZE_CONTROLLER_CONTROLLER_H_

#include "../model/model.h"

namespace s21 {
/**
 * @brief The Controller class для связывания класса модели и вью
 */
class Controller {
 public:
  Controller();
  ~Controller();
  /**
   * @brief Run метод для передачи команды в модель.
   * @param[in] c указатель на базовый класс для передачи команды.
   */
  void Run(Command* c);
  /**
   * @brief Train метод для запуска.
   * @param[in] row ряд для конечной точки.
   * @param[in] col колонка для конечной точки.
   */
  void Train(int row, int col);
  /**
   * @brief Get метод для полученя данный из модели для отображения их во вью.
   * @return возвращает матрицу с лабиринтом.
   */
  std::vector<std::vector<int>>& Get();
  /**
   * @brief GetTable метод для получения таблицы после применения метода
   * q-learning.
   * @return двумерный массив данных.
   */
  std::vector<std::vector<double>>& GetTable();

 private:
  Model model_;
};

}  // end namespace s21
#endif  //_MAZE_CONTROLLER_CONTROLLER_H_
