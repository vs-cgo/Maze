#ifndef _MAZE_VIEW_CAVE_GEN_GENCAVE_H_
#define _MAZE_VIEW_CAVE_GEN_GENCAVE_H_

#include <random>
#include <vector>

namespace s21 {
/**
 * @brief The GenCave class для генерации пещеры.
 */
class GenCave {
 public:
  /**
   * @brief GenCave конструктор.
   */
  GenCave();
  /**
   * @brief Generation
   * @param[in] row количество рядов
   * @param[in] col количество столбцов
   * @param[in] chance шанс на зарождение клетки(0 - 100)
   * @return готовую пещеру.
   */
  std::vector<std::vector<int>> Generation(int row, int col, int chance);
};
}  // end namespace s21

#endif  // _MAZE_VIEW_CAVE_GEN_GENCAVE_H_
