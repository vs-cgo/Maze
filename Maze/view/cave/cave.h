#ifndef _MAZE_VIEW_CAVE_CAVE_H_
#define _MAZE_VIEW_CAVE_CAVE_H_

#include <QCoreApplication>
#include <QDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QRegularExpression>
#include <QSpinBox>
#include <QTimer>

#include "../../model/command/command.h"
#include "gen/gencave.h"

namespace s21 {
/**
 * @brief The Cave class позволяет открывать, генерировать, сохранять пещеру, а
 * также запускать клеточный автомат.
 */
class Cave : public QDialog {
  Q_OBJECT
 public:
  explicit Cave(QWidget *parent = nullptr);

 signals:

 protected:
  void paintEvent(QPaintEvent *event) override;
 private slots:
  void Open();
  void Gen();
  void Save();
  void Manual();
  void Automatic();
  void ResetInfo();
  void Timer();

 private:
  void Set();
  void NextStep();
  int CountNeighbors(int i, int k, int row, int col);

 private:
  QGridLayout *layout;
  QPushButton *open, *save, *gen, *manual, *automatic;
  QLabel *info, *label_step, *label_row, *label_col, *label_death, *label_live,
      *label_chance;
  QSpinBox *lim_life, *lim_death, *step;
  QLineEdit *cha, *row, *col;
  QTimer *timer;
  GenCave generation;
  std::vector<std::vector<int>> cave_;
};
}  // end namespace s21
#endif  // _MAZE_VIEW_CAVE_CAVE_H_
