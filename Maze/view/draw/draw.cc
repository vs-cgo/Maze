#include "draw.h"

namespace s21 {

Draw::Draw(QWidget *parent) : QWidget{parent}, data_() {}

void Draw::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, false);
  painter.fillRect(rect(), QColor(Qt::white));
  if (data_.empty() || data_[0].empty()) return;
  QPen pen_black(Qt::black, 2), pen_red(Qt::red, 2);
  painter.setPen(pen_black);
  painter.drawLine(QPoint(1, 0), QPoint(1, 500));
  painter.drawLine(QPoint(0, 1), QPoint(500, 1));
  painter.drawLine(QPoint(499, 0), QPoint(499, 500));
  painter.drawLine(QPoint(0, 499), QPoint(500, 499));
  int row = data_.size(), col = data_[0].size();
  double step_x = 500.0 / row, step_y = 500.0 / col;
  double x = step_y, y = step_x;
  for (int i = 0; i < row; ++i, y += step_x, x = step_y) {
    for (int k = 0; k < col; ++k, x += step_y) {
      if (data_[i][k] & Wall::kRight) {
        painter.drawLine(QPoint(x, y), QPoint(x, y - step_x));
      }
      if (data_[i][k] & Wall::kBot) {
        painter.drawLine(QPoint(x, y), QPoint(x - step_y, y));
      }
    }
  }
  painter.setPen(pen_red);
  x = step_y / 2, y = step_x / 2;
  for (int i = 0; i < row; ++i, y += step_x, x = step_y / 2) {
    for (int k = 0; k < col; ++k, x += step_y) {
      if (data_[i][k] & Wall::kVertTop) {
        painter.drawLine(QPoint(x, y), QPoint(x, y - step_x));
      }
      if (data_[i][k] & Wall::kVertBot) {
        painter.drawLine(QPoint(x, y), QPoint(x, y + step_x));
      }
      if (data_[i][k] & Wall::kHorRight) {
        painter.drawLine(QPoint(x, y), QPoint(x + step_y, y));
      }
      if (data_[i][k] & Wall::kHorLeft) {
        painter.drawLine(QPoint(x, y), QPoint(x - step_y, y));
      }
    }
  }
}
void Draw::pass(std::vector<std::vector<int>> &v) { data_ = v; }
}  // namespace s21
