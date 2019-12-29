#include "knn.h"
#include <iostream>


std::ostream &operator<<(std::ostream &os, preds const &p) {
  for (int i = 0; i < p.points->m; i++) 
    os << "Label: " << p.labels[i] << " " << p.points->matrix[i];

  os << std::endl;
  return os;
}
