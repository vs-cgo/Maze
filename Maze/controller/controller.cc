#include "controller.h"

namespace s21 {

Controller::Controller() : model_(){};
Controller::~Controller(){};

void Controller::Run(Command* c) { model_.Run(c); }
void Controller::Train(int row, int col) { model_.Train(row, col); };
std::vector<std::vector<int>>& Controller::Get() { return model_.Get(); }
std::vector<std::vector<double>>& Controller::GetTable() {
  return model_.GetTable();
}
}  // end namespace s21
