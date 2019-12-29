#include "knn.h"
#include <iostream>

vec::vec(int n) {
  double *vector = new double[n];
  for (int i = 0; i < n; i++) 
    vector[i] = 0;

  this->n = n;
  this->vector = vector;
}

void vec::rand(double f_min, double f_max) {
  for (int i = 0; i < this->n; i++) 
    this->vector[i] = f_min + ((double)std::rand() / RAND_MAX) * (f_max - f_min);
}


vec::~vec() {
  delete[] this->vector;
}

std::ostream &operator<<(std::ostream &os, vec const &v) {
  for (int i = 0; i < v.n; i++)
    os << v.vector[i] << " ";

  os << std::endl;
  return os;
}
