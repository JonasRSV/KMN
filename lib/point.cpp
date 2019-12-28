#include "point.h"
#include <math.h>
#include <iostream>


double l2_distance(point &p1, point &p2) {
  if (p1.dim != p2.dim)
    throw "Dimensions do not match";

  double norm = 0;
  for (int d = 0; d < p1.dim; d++)
    norm += ((p1.co[d] - p2.co[d]) * (p1.co[d] - p2.co[d]));

  return sqrt(norm);
}


void print_point(point &p) {
  for (int j = 0; j < p.dim; j++) {
    std::cout << p.co[j] << " ";
  }
  std::cout << std::endl;
}
