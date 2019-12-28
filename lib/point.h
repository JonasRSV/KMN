#pragma once

struct point {
  //coordinates
  double *co;
  int dim;
};

double l2_distance(point &p1, point &p2);
void print_point(point &p);

