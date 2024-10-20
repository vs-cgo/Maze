#include "view.h"

#include "./ui_view.h"

namespace s21 {
View::View(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::View), controller_() {
  ui->setupUi(this);

  drawing_ = new Draw(ui->centralwidget);
  drawing_->setObjectName("drawing");
  drawing_->setMinimumSize(QSize(500, 500));
  drawing_->setContentsMargins(10, 10, 10, 10);
  drawing_->setSizePolicy(QSizePolicy::Policy::Fixed,
                          QSizePolicy::Policy::Fixed);
  ui->gridLayout->addWidget(drawing_, 0, 0, 1, 1);
  connect(ui->buttonOpen, SIGNAL(clicked()), this, SLOT(OpenFile()));
  connect(ui->buttonGen, SIGNAL(clicked()), this, SLOT(Generate()));
  connect(ui->buttonGen2, SIGNAL(clicked()), this, SLOT(Generate2()));
  connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(SaveMaze()));
  connect(ui->buttonSolu, SIGNAL(clicked()), this, SLOT(SolveMaze()));
  for (auto w : {ui->start_r, ui->start_c, ui->end_r, ui->end_c}) {
    connect(w, SIGNAL(valueChanged(int)), this, SLOT(ChangeCoord()));
  }
  connect(ui->buttonCave, SIGNAL(clicked()), this, SLOT(DrawCave()));
  connect(ui->buttonAgent, SIGNAL(clicked()), this, SLOT(TrainAgent()));
  connect(ui->buttonOut, SIGNAL(clicked()), this, SLOT(FindOut()));
}

View::~View() { delete ui; }

void View::OpenFile() {
  ui->label->setText("");
  ui->buttonSolu->setChecked(false);
  QString name = QFileDialog::getOpenFileName(
      this, "Open file", QCoreApplication::applicationDirPath(), "", 0,
      QFileDialog::DontUseNativeDialog);
  if (name == "") return;
  try {
    auto c = std::make_unique<OpenCommand>(name.toStdString());
    controller_.Run(c.get());
    SetRange();
  } catch (not_open) {
    NotOpen();
  } catch (bad_file) {
    BadFile();
  }
  drawing_->pass(controller_.Get());
}

void View::Generate() {
  DropButton();
  auto c =
      std::make_unique<GenCommand>(ui->box_row->value(), ui->box_col->value());
  controller_.Run(c.get());
  SetRange();
  drawing_->pass(controller_.Get());
  drawing_->update();
}

void View::Generate2() {
  DropButton();
  auto c =
      std::make_unique<GenCommandS>(ui->box_row->value(), ui->box_col->value());
  controller_.Run(c.get());
  SetRange();
  drawing_->pass(controller_.Get());
  drawing_->update();
}

void View::SaveMaze() {
  ui->label->setText("");
  QString name = QFileDialog::getSaveFileName(
      this, "Save file", "/Users/kareemto/A1_Maze_CPP-1/src/test.txt", "", 0,
      QFileDialog::DontUseNativeDialog);  // CNANGE BEFOR FINISH
  if (name == "") return;
  if (controller_.Get().empty()) {
    EmptyMaze();
  } else {
    try {
      auto c = std::make_unique<SaveCommand>(name.toStdString());
      controller_.Run(c.get());
    } catch (not_open) {
      ui->label->setText("<font color='red'>Файл не открыт!</font>");
    }
  }
}
void View::ChangeCoord() {
  ui->label->setText("");
  if (!ui->buttonSolu->isChecked() && !ui->buttonAgent->isChecked()) return;
  std::vector<std::vector<int>> temp = controller_.Get();
  if (temp.empty()) {
    EmptyMaze();
    return;
  }
  if (ui->start_r->value() == ui->end_r->value() &&
      ui->start_c->value() == ui->end_c->value() &&
      (ui->buttonSolu->isChecked() || ui->buttonOut->isChecked())) {
    ui->label->setText(
        "<font color='red'>Начало и конец маршрута<br>находятся в одной "
        "точке!</font>");
    drawing_->pass(controller_.Get());
    drawing_->update();
    return;
  }
  if (ui->buttonSolu->isChecked()) {
    SolveMaze();
  } else if (ui->buttonAgent->isChecked()) {
    QString name = (reinterpret_cast<QSpinBox *>(sender()))->objectName();
    if (name == "end_r" || name == "end_c") {
      ui->buttonAgent->setChecked(false);
      ui->buttonOut->setChecked(false);
      SolveMaze();
    } else {
      FindOut();
    }
  }
}
void View::SolveMaze() {
  ui->label->setText("");
  ui->buttonAgent->setChecked(false);
  ui->buttonOut->setChecked(false);

  if (!ui->buttonSolu->isChecked()) {
    drawing_->pass(controller_.Get());
    drawing_->update();
  } else {
    std::vector<std::vector<int>> temp = controller_.Get();
    if (temp.empty()) {
      EmptyMaze();
      return;
    }
    int row = temp.size(), col = temp[0].size();
    int start_r = ui->start_r->value(), start_c = ui->start_c->value(),
        end_r = ui->end_r->value(), end_c = ui->end_c->value();
    if (start_r > row || start_c > col || end_r > row || end_c > col) {
      ui->label->setText(
          "<font color='red'>Координаты выходят<br>за размеры "
          "лабиринта</font>");
      return;
    }
    if (start_r == end_r && start_c == end_c) {
      ui->label->setText(
          "<font color='red'>Начало и конец маршрута<br>находятся в одной "
          "точке!</font>");
      drawing_->pass(controller_.Get());
      drawing_->update();
      return;
    }
    auto c = std::make_unique<SolveCommand>(start_r, start_c, end_r, end_c);
    controller_.Run(c.get());
    drawing_->pass(c.get()->GetSolve());
    drawing_->update();
  }
}

void View::DrawCave() {
  Cave *cave = new Cave(this);
  cave->setWindowTitle("Cave");
  cave->show();
}

void View::TrainAgent() {
  ui->label->setText("");
  ui->buttonSolu->setChecked(false);
  if (!ui->buttonAgent->isChecked()) {
    ui->buttonOut->setChecked(false);
    drawing_->pass(controller_.Get());
    drawing_->update();
    return;
  }
  std::vector<std::vector<int>> temp = controller_.Get();
  if (temp.empty()) {
    EmptyMaze();
    return;
  }
  int row = ui->end_r->value(), col = ui->end_c->value();
  if (temp.size() < row || temp[0].size() < col) {
    ui->label->setText(
        "<font color='red'>Координаты выходят<br>за размеры "
        "лабиринта</font>");
    ui->label->update();
    return;
  }
  drawing_->pass(controller_.Get());
  drawing_->update();
  Wait();
  controller_.Train(row, col);
  ui->label->setText("");
}
void View::FindOut() {
  ui->label->setText("");
  if (!ui->buttonAgent->isChecked()) {
    ui->buttonOut->setChecked(false);
  } else if (ui->start_r->value() == ui->end_r->value() &&
             ui->start_c->value() == ui->end_c->value()) {
    ui->label->setText(
        "<font color='red'>Начало и конец маршрута<br>находятся в одной "
        "точке!</font>");
    drawing_->pass(controller_.Get());
    drawing_->update();
  } else if (ui->buttonOut->isChecked()) {
    auto c = std::make_unique<AgentCommand>(
        ui->start_r->value(), ui->start_c->value(), ui->end_r->value(),
        ui->end_c->value(), controller_.GetTable());
    controller_.Run(c.get());
    drawing_->pass(c.get()->GetSolve());
    drawing_->update();
  } else {
    drawing_->pass(controller_.Get());
    drawing_->update();
  }
}
void View::EmptyMaze() {
  ui->label->setText(
      "<font color='red'>Нет готового лабиринта<br>Загрузите или "
      "сгенерируйте новый!</font>");
}
void View::NotOpen() {
  ui->label->setText("<font color='red'>Файл не открыт!</font>");
}
void View::BadFile() {
  ui->label->setText("<font color='red'>Неправильный формат файла!</font>");
}
void View::DropButton() {
  ui->label->setText("");
  ui->buttonSolu->setChecked(false);
  ui->buttonAgent->setChecked(false);
  ui->buttonOut->setChecked(false);
}
void View::SetRange() {
  std::vector<std::vector<int>> temp = controller_.Get();
  if (temp.empty()) return;
  ui->start_r->setMaximum(temp.size());
  ui->start_c->setMaximum(temp[0].size());
  ui->end_r->setMaximum(temp.size());
  ui->end_c->setMaximum(temp[0].size());
}
void View::mousePressEvent(QMouseEvent *event) {
  if (childAt(event->pos())->objectName() == "drawing") {
    std::vector<std::vector<int>> temp = controller_.Get();
    if (temp.empty()) return;
    double row = 500.0 / temp.size(), col = 500.0 / temp[0].size();
    if (event->button() == Qt::LeftButton) {
      ui->start_r->setValue((event->pos().ry() - 10) / row + 1);
      ui->start_c->setValue((event->pos().rx() - 10) / col + 1);
    } else if (event->button() == Qt::RightButton) {
      ui->end_r->setValue((event->pos().ry() - 10) / row + 1);
      ui->end_c->setValue((event->pos().rx() - 10) / col + 1);
    }
  }
};
void View::Wait() {
  ui->label->setText(
      "<font color='red'>Подождите, идет обучение агента!</font>");
  ui->label->repaint();
  QApplication::processEvents();
}
}  // end namespace s21
