#include "cluster.h"
#include <iostream>

clusters* init_clusters(point *po, int n_points, int n_clusters) {
  clusters *cl = new clusters();

  cl->po = po;
  cl->n_points = n_points;
  cl->n_clusters = n_clusters;


  int *labels = new int[n_points];
  for (int i = 0; i < n_points; i++) {
    labels[i] = rand() % n_clusters;
  }

  cl->labels = labels;

  if (n_points > 0) {
    cl->dim = po->dim;
  }

  return cl;
}


void print_clusters(clusters *cl) {
  std::cout << "Number of points " << cl->n_points << std::endl;

  for (int i = 0; i < cl->n_points; i++) {
    std::cout << "Label: " << cl->labels[i] << " ";
    print_point(cl->po[i]);
  }

  std::cout << std::endl;
}
