#include <QApplication>
#include <clocale>

#include "view/view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_NUMERIC, "en_US.UTF-8");
  s21::View v;
  v.setWindowTitle("Maze");
  v.show();
  return a.exec();
}
