#include "cave.h"

namespace s21 {
Cave::Cave(QWidget *parent) : QDialog{parent}, generation() {
  Set();
  connect(open, SIGNAL(clicked()), this, SLOT(Open()));
  connect(gen, SIGNAL(clicked()), this, SLOT(Gen()));
  connect(save, SIGNAL(clicked()), this, SLOT(Save()));
  connect(manual, SIGNAL(clicked()), this, SLOT(Manual()));
  connect(automatic, SIGNAL(clicked()), this, SLOT(Automatic()));
  for (auto w : {cha, row, col}) {
    connect(w, SIGNAL(textChanged(QString)), this, SLOT(ResetInfo()));
  }
  for (auto w : {step, lim_life, lim_death}) {
    connect(w, SIGNAL(textChanged(QString)), this, SLOT(ResetInfo()));
  }
  connect(timer, SIGNAL(timeout()), this, SLOT(Timer()));
}

void Cave::Set() {
  setMinimumSize(780, 520);
  layout = new QGridLayout(this);
  layout->setSpacing(5);
  layout->setRowStretch(0, 1);
  layout->setColumnStretch(0, 1);
  layout->setAlignment(Qt::AlignmentFlag::AlignTop);
  layout->addItem(
      new QSpacerItem(520, 20, QSizePolicy::Fixed, QSizePolicy::Fixed), 0, 0, 1,
      1);
  layout->setContentsMargins(10, 10, 10, 10);

  open = new QPushButton("Open", this);
  save = new QPushButton("Save", this);
  gen = new QPushButton("Generation", this);
  layout->addWidget(open, 0, 1, 1, 2);
  layout->addWidget(save, 0, 3, 1, 2);
  layout->addWidget(gen, 1, 1, 1, 4);

  info = new QLabel("", this);
  label_chance = new QLabel("Шанс рождения:", this);
  label_row = new QLabel("Строк:", this);
  label_col = new QLabel("Столбцов:", this);

  info->setMinimumHeight(25);
  info->setMinimumWidth(200);
  label_chance->setMinimumHeight(25);
  label_row->setMinimumHeight(25);
  label_col->setMinimumHeight(25);

  info->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Fixed);
  label_chance->setSizePolicy(QSizePolicy::Policy::Fixed,
                              QSizePolicy::Policy::Fixed);
  label_row->setSizePolicy(QSizePolicy::Policy::Fixed,
                           QSizePolicy::Policy::Fixed);
  label_col->setSizePolicy(QSizePolicy::Policy::Fixed,
                           QSizePolicy::Policy::Fixed);

  layout->addWidget(info, 10, 1, 1, 4);
  layout->addWidget(label_chance, 2, 1, 1, 2);
  layout->addWidget(label_row, 3, 1, 1, 2);
  layout->addWidget(label_col, 4, 1, 1, 2);

  row = new QLineEdit("10", this);
  col = new QLineEdit("10", this);
  cha = new QLineEdit("50", this);
  row->setAlignment(Qt::AlignmentFlag::AlignRight);
  col->setAlignment(Qt::AlignmentFlag::AlignRight);
  cha->setAlignment(Qt::AlignmentFlag::AlignRight);
  cha->setMinimumSize(25, 25);
  row->setMinimumSize(25, 25);
  col->setMinimumSize(25, 25);
  cha->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
  row->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
  col->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
  QRegularExpression re("[1-9]|[1-4][0-9]|50");
  QRegularExpression rc("0|[1-9][0-9]|100");
  QRegularExpressionValidator *v_row = new QRegularExpressionValidator(re, row);
  QRegularExpressionValidator *v_col = new QRegularExpressionValidator(re, col);
  QRegularExpressionValidator *v_cha = new QRegularExpressionValidator(rc, cha);
  row->setValidator(v_row);
  col->setValidator(v_col);
  cha->setValidator(v_cha);

  layout->addWidget(cha, 2, 3, 1, 2);
  layout->addWidget(row, 3, 3, 1, 2);
  layout->addWidget(col, 4, 3, 1, 2);

  manual = new QPushButton("Ручная", this);
  automatic = new QPushButton("Автоматическая", this);
  automatic->setCheckable(true);
  label_step = new QLabel("Шаг:", this);
  layout->addWidget(label_step, 7, 1, 1, 2);

  layout->addWidget(manual, 5, 1, 1, 2);
  layout->addWidget(automatic, 6, 1, 1, 2);

  lim_life = new QSpinBox(this);
  lim_life->setRange(0, 7);
  lim_life->setValue(4);
  lim_life->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
  lim_life->setAlignment(Qt::AlignmentFlag::AlignRight);
  lim_life->setMinimumHeight(25);

  lim_death = new QSpinBox(this);
  lim_death->setRange(0, 7);
  lim_death->setValue(3);
  lim_death->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
  lim_death->setAlignment(Qt::AlignmentFlag::AlignRight);
  lim_death->setMinimumHeight(25);

  step = new QSpinBox(this);
  step->setRange(0, 10000);
  step->setValue(500);
  step->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
  step->setAlignment(Qt::AlignmentFlag::AlignRight);
  step->setMinimumHeight(25);
  step->setSuffix(" мс");

  label_live = new QLabel("Предел рождения", this);
  label_death = new QLabel("Предел смерти", this);
  label_live->setMinimumHeight(25);
  label_live->setMaximumHeight(25);
  label_death->setMinimumHeight(25);
  label_death->setMaximumHeight(25);
  label_live->setSizePolicy(QSizePolicy::Policy::Preferred,
                            QSizePolicy::Policy::Fixed);
  label_death->setSizePolicy(QSizePolicy::Policy::Preferred,
                             QSizePolicy::Policy::Fixed);

  layout->addWidget(label_live, 8, 1, 1, 2);
  layout->addWidget(label_death, 8, 3, 1, 2);
  layout->addWidget(lim_life, 9, 1, 1, 2);
  layout->addWidget(lim_death, 9, 3, 1, 2);
  layout->addWidget(step, 7, 3, 1, 2);

  open->setFocus();
  timer = new QTimer(this);
}
void Cave::ResetInfo() { info->setText(""); }
void Cave::Open() {
  if (automatic->isChecked()) {
    timer->stop();
    automatic->setChecked(false);
  }
  info->setText("");
  QString file = QFileDialog::getOpenFileName(
      this, "Open file", QCoreApplication::applicationDirPath(), "", 0,
      QFileDialog::DontUseNativeDialog);
  if (file == "") return;

  try {
    std::ifstream is(file.toStdString());
    if (!is.is_open()) throw not_open();

    int r = 0, c = 0;
    char temp[256] = {0};
    if (is.getline(temp, 255).eof() || sscanf(temp, "%d%d", &r, &c) < 2 ||
        r < 1 || c < 1) {
      throw bad_file();
    }
    cave_ = std::vector<std::vector<int>>(r, std::vector<int>(c, 0));

    for (int i = 0; i < r && (!is.getline(temp, 255).eof() || is.gcount());
         ++i) {
      char *ptr = temp, *end;
      for (int k = 0, x = 0; k < c; ++k) {
        x = strtol(ptr, &end, 10);
        if (ptr == end) break;  // Mb throw bad_file?
        ptr = end;
        if (x == 1) ++cave_[i][k];
      }
    }
  } catch (not_open) {
    info->setText("<font color='red'>Файл не открыт!</font>");
  } catch (bad_file) {
    info->setText("<font color='red'>Некорректный файл!</font>");
  }
}
void Cave::Gen() {
  if (automatic->isChecked()) {
    timer->stop();
    automatic->setChecked(false);
  }
  info->setText("");
  int r = row->text().toInt(), c = col->text().toInt(), v = cha->text().toInt();
  generation.Generation(r, c, v);
  cave_ = generation.Generation(r, c, v);
  update();
}
void Cave::Save() {
  if (automatic->isChecked()) {
    timer->stop();
    automatic->setChecked(false);
  }
  info->setText("");
  if (cave_.empty()) {
    info->setText(
        "<font color='red'>Нет готовой пещеры.<br>Загрузите или сгенерируйте "
        "новую.</font>");
    return;
  }
  QString file = QFileDialog::getSaveFileName(
      this, "Save file", QCoreApplication::applicationDirPath(), "", 0,
      QFileDialog::DontUseNativeDialog);
  if (file == "") return;
  try {
    std::ofstream os(file.toStdString());
    if (!os.is_open()) throw not_open();

    int r = cave_.size(), c = cave_[0].size();
    os << r << ' ' << c << std::endl;

    std::string str;
    for (int i = 0; i < cave_.size(); ++i) {
      for (int k = 0; k < cave_[0].size(); ++k) {
        str += cave_[i][k] ? '1' : '0';
        str += k < c - 1 ? " " : (i < r - 1 ? "\n" : "");
      }
    }
    os << str;
  } catch (...) {
    info->setText("<font color='red'>Невозможно открыть файл</font>");
  }
}
void Cave::Timer() { NextStep(); }
void Cave::Manual() {
  info->setText("");
  NextStep();
}
void Cave::Automatic() {
  info->setText("");
  if (automatic->isChecked()) {
    timer->start(step->value());
  } else {
    timer->stop();
  }
}
void Cave::NextStep() {
  if (cave_.empty() || cave_[0].empty()) {
    info->setText(
        "<font color='red'>Нет готовой пещеры.<br>Загрузите или сгенерируйте "
        "новую.</font>");
    return;
  }
  int row = cave_.size(), col = cave_[0].size();
  int ld = lim_death->value(), ll = lim_life->value();
  std::vector<std::vector<int>> temp(row, std::vector<int>(col, 0));
  for (int i = 0; i < row; ++i) {
    for (int k = 0; k < col; ++k) {
      int neighbors = CountNeighbors(i, k, row, col);
      if (cave_[i][k]) {
        if (neighbors >= ld) temp[i][k] = 1;
      } else {
        if (neighbors > ll) temp[i][k] = 1;
      }
    }
  }
  if (cave_ == temp) {
    timer->stop();
    automatic->setChecked(false);
  }
  cave_ = temp;
  this->update();
}
int Cave::CountNeighbors(int i, int k, int row, int col) {
  int count = 0;
  for (int r = i - 1; r <= i + 1; ++r) {
    if (r < 0 || r == row) {
      count += 3;
      continue;
    }
    for (int c = k - 1; c <= k + 1; ++c) {
      if (r == i && c == k) continue;
      if (c < 0 || c == col || cave_[r][c]) {
        count += 1;
      }
    }
  }
  return count;
}
void Cave::paintEvent(QPaintEvent *event) {
  QDialog::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, false);
  // painter.fillRect(rect(), QColor(Qt::darkGrey));
  painter.fillRect(10, 10, 500, 500, QColor(Qt::white));
  if (cave_.empty() || cave_[0].empty()) return;
  int row = cave_.size(), col = cave_[0].size();
  double r = 10, c = 10, step_r = 500.0 / row, step_c = 500.0 / col;
  for (int i = 0; i < row; ++i, r += step_r, c = 10) {
    for (int k = 0; k < col; ++k, c += step_c) {
      if (cave_[i][k])
        painter.fillRect(c, r, step_c, step_r, QColor(Qt::black));
    }
  }
}
}  // end namespace s21
