#include "./model.h"

namespace s21 {
Model::Model() : data_() {}
Model::~Model() {}

void Model::Run(Command *c) {
  try {
    c->Execute(data_);
  } catch (...) {
    data_.clear();
    throw;
  }
}
void Model::Train(int row, int col) {
  QLearning l(row, col, data_);
  l.Training();
  q_table_ = l.Get();
}
std::vector<std::vector<int>> &Model::Get() { return data_; }
std::vector<std::vector<double>> &Model::GetTable() { return q_table_; }
}  // end namespace s21
