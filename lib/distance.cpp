#include "kmn.h"
#include <math.h>

double l2(const vec &p1, const vec &p2) {
  double norm = 0;
  for (int d = 0; d < p1.n; d++)
    norm += ((p1.vector[d] - p2.vector[d]) * (p1.vector[d] - p2.vector[d]));
  return sqrt(norm);
}
