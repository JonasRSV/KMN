#include "knn.h"
#include <iostream>
#include <cstring>


mat::mat(int m, int n) {
  vec *matrix = new vec[m];
  for (int i = 0; i < m; i++) {
    matrix[i] = new vec(n);
  }

  this->m = m;
  this->matrix = matrix;
}

mat::~mat() {
  delete[] this->matrix;
}

void mat::extend(mat *points) {
  int new_m = this->m + points->m;

  vec *matrix = new vec[new_m];

  // copy from self to data
  for (int i = 0; i < this->m; i++)
    matrix[i] = this->matrix[i];

  for (int i = this->m; i < new_m; i++) 
    matrix[i] = points->matrix[i - this->m];

  // Free self
  if (this->matrix != nullptr) 
    delete[] this->matrix;

  // Set new data to self
  this->matrix = matrix;
  this->m = new_m;
}

std::ostream &operator<<(std::ostream &os, mat const &points) {
  for (int i = 0; i < points.m; i++) 
    os << points.matrix[i];

  os << std::endl;
  return os;
}
