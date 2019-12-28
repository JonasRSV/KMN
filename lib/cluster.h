#include "point.h"

#pragma once

struct clusters {
  int dim;
  int n_clusters;
  int n_points;
  point *po;
  int *labels;
};


clusters* init_clusters(point *points, int n_points, int n_clusters);
void print_clusters(clusters *cl);

