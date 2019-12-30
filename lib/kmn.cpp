#include <iostream>
#include <float.h>
#include "kmn.h"
#include <set>


kmn::kmn(int c) {
  this->c = c;
  this->d = l2;

  this->labels = nullptr;
  this->points = nullptr;
  this->cent = nullptr;
}

kmn::kmn() {
  this->c = 0;
  this->d = l2;

  this->labels = nullptr;
  this->points = nullptr;
  this->cent = nullptr;
}

kmn::~kmn(){
  this->clean();
}

void kmn::clean() {
  if (this->points != nullptr) {
    delete this->points;
    this->points = nullptr;
  }

  if (this->cent != nullptr) {
    delete this->cent;
    this->cent = nullptr;
  }

  if (this->labels != nullptr) {
    delete[] this->labels;
    this->labels = nullptr;
  }
}


preds* kmn::predict_cent(mat *points) {
  if (this->cent == nullptr) 
    throw "Cannot do predict cent before fitting (Run Kmeans or Kneigh)";

      int *labels = new int[points->m];
  for (int i = 0; i < points->m; i++) {

    int label = 0;
    double bdist = DBL_MAX, dist;
    for (int j = 0; j < this->c; j++) {
      dist = this->d(points->matrix[i], this->cent->matrix[j]);
      if (dist < bdist) {
        bdist = dist;
        label = j;
      }
    }
    labels[i] = label;
  }
  return new preds(labels, points);
}

int kmn::calc_c(int m, int *labels) {
  int mx = 0;
  for (int i = 0; i < m; i++)
    if (labels[i] > mx)
      mx = labels[i];
  return mx + 1;
}
