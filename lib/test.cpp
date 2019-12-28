#include <time.h>
#include <iostream>
#include "point.h"
#include "cluster.h"
#include "knn.h"

double f_rand(double f_min, double f_max) {
  double f = (double)rand() / RAND_MAX;
  return f_min + f * (f_max - f_min);
}

point* get_sample_points(int n, int dim, double f_min, double f_max) {
  point *p = new point[n];

  for (int i = 0; i < n; i++) {
    double *co = new double[dim];

    // Fill the point
    for (int j = 0; j < dim; j++) {
      co[j] = f_rand(f_min, f_max);
    }

    p[i] = point{
      co,
        dim
    };

  }

  return p;
}

clusters* get_sample_cluster(int n, int dim, int n_clusters) {
  // Will try to make equal distribution in all clusters

  point **point_mat = new point*[n_clusters];

  int point_in_cluster = n / n_clusters, p = 0;

  double center = 2, var = 1, shift = 2;
  for (int i = 0; i < n_clusters; i++) {
    point_mat[i] = get_sample_points(point_in_cluster, dim, center - var, center + var);
    p += point_in_cluster;
    center += shift;
  }

  // Flatten clusters
  point *points = new point[p];
  for (int i = 0; i < n_clusters; i++) {
    for (int j = 0; j < point_in_cluster; j++) {
      points[i * point_in_cluster + j] = point_mat[i][j];
    }
  }

  clusters *cl = init_clusters(points, p, n_clusters);

  return cl;
}

void test_distance() {
  double *co1 = new double[5];
  co1[0] = 1.0;
  co1[1] = 2.0;
  co1[2] = 3.0;
  co1[3] = 1.0;
  co1[4] = 1.0;

  point p1 = point{co1, 5};

  double *co2 = new double[5];
  co2[0] = 1.0;
  co2[1] = 2.0;
  co2[2] = 3.0;
  co2[3] = 1.0;
  co2[4] = 3.0;

  point p2 = point{co2, 5};

  double d = l2_distance(p1, p2);

  std::cout << "D " << d << std::endl;
  if (d != 2.0)
    std::cout << "distance gave " << d << " should be 2.0" << std::endl;

}

void test_knn_small_cluster() {
  clusters *cl = get_sample_cluster(20, 4, 5);
  print_clusters(cl);
  cl = em(cl, 10);
  print_clusters(cl);
}

void test_knn_large_cluster() {
  std::cout << "Getting Large cluster 1000000 points in R10 with 10 clusters" << std::endl;
  time_t timestamp = time(NULL);
  clusters *cl = get_sample_cluster(1000000, 10, 10);
  time_t finish = time(NULL);

  std::cout << "Took " << difftime(finish, timestamp) << " seconds" << std::endl;

  std::cout << "Clustering" << std::endl;
  timestamp = time(NULL);
  em(cl, 10);
  finish = time(NULL);
  std::cout << "Took " << difftime(finish, timestamp) << " seconds" << std::endl;
}

void test_knn_prediction() {
  clusters *cl = get_sample_cluster(20, 3, 3);
  print_clusters(cl);
  cl = em(cl, 10);
  print_clusters(cl);



  std::cout << "Predicting on same as is training" << std::endl;
  clusters *res = predict(cl, cl->po, 18, 3);
  print_clusters(res);

  std::cout << "Predicting on new" << std::endl;
  point *po = get_sample_points(10, 3, 0, 10);
  res = predict(cl, po, 10, 3);
  print_clusters(res);
}


int main() {
  test_distance();

  srand(101);

  test_knn_prediction();


  return 0;
}
