#ifndef _MAZE_VIEW_DRAW_DRAW_H_
#define _MAZE_VIEW_DRAW_DRAW_H_

#include <QPaintEvent>
#include <QPainter>
#include <QWidget>

#include "../../model/command/command.h"

namespace s21 {
/**
 * @brief The Draw class для отрисовки лабиринта в поле 500x500 px.
 */
class Draw : public QWidget {
  Q_OBJECT
 public:
  explicit Draw(QWidget *parent = nullptr);
  /**
   * @brief pass метод для передачи лабиринта.
   * @param[in] maze лабиринтю
   */
  void pass(std::vector<std::vector<int>> &maze);
 signals:
 protected:
  void paintEvent(QPaintEvent *even) override;

 private:
  std::vector<std::vector<int>> data_;
};

}  // namespace s21

#endif  // _MAZE_VIEW_DRAW_DRAW_H_
