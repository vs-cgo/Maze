#ifndef _MAZE_VIEW_VIEW_H_
#define _MAZE_VIEW_VIEW_H_

#include <QFileDialog>
#include <QMainWindow>
#include <QSizePolicy>

#include "../controller/controller.h"
#include "cave/cave.h"
#include "draw/draw.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {
/**
 * @brief The View class для принятия параметров и вывода данных.
 */
class View : public QMainWindow {
  Q_OBJECT

 public:
  View(QWidget *parent = nullptr);
  ~View();

 protected:
  void mousePressEvent(QMouseEvent *event) override;
 private slots:
  void OpenFile();
  void Generate();
  void Generate2();
  void SaveMaze();
  void SolveMaze();
  void ChangeCoord();
  void DrawCave();
  void TrainAgent();
  void FindOut();

 private:
  void EmptyMaze();
  void NotOpen();
  void BadFile();
  void DropButton();
  void SetRange();
  void Wait();

 private:
  Ui::View *ui;
  Controller controller_;
  Draw *drawing_;
};
}  // end namespace s21
#endif  // _MAZE_VIEW_VIEW_H_
