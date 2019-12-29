#include <time.h>
#include <iostream>
#include "knn.h"

vec* mock_vec(int m, int n, double f_min, double f_max) {
  vec *points = new vec[m];
  for (int i = 0; i < m; i++) {
    points[i] = vec(n);
    points[i].rand(f_min, f_max);
  }
  return points;
}

mat* mock_clusters(int m, int n, int c) {

  int n_cluster_points = m / c;
  mat *data = new mat();
  double center = 2, var = 1, shift = 2;
  for (int i = 0; i < c; i++) {
    vec *points = mock_vec(n_cluster_points, n, center - var, center + var);
    mat tmp = mat(n_cluster_points, points);

    data->extend(&tmp);
    center += shift;
  }

  return data;
}


void test_performance_of_clustering() {

  std::cout << "Getting Large cluster 1 000 00 points in R10 with 10 clusters" << std::endl;
  time_t timestamp = time(NULL);
  mat *train_points = mock_clusters(100000, 10, 10);
  time_t finish = time(NULL);
  std::cout << "Took " << difftime(finish, timestamp) << " seconds" << std::endl;

  std::cout << "Getting Large cluster 1000 points in R10 with 10 clusters to predict" << std::endl;
  timestamp = time(NULL);
  mat *pred_points = mock_clusters(1000, 10, 10);
  finish = time(NULL);
  std::cout << "Took " << difftime(finish, timestamp) << " seconds" << std::endl;

  knn k = knn(10);
  std::cout << "Clustering" << std::endl;
  timestamp = time(NULL);
  k.fit(train_points, 10);
  finish = time(NULL);
  std::cout << "Took " << difftime(finish, timestamp) << " seconds" << std::endl;

  std::cout << "Predicting with k = 4 on 1000 points" << std::endl;
  timestamp = time(NULL);
  k.predict_knn(pred_points, 4);
  finish = time(NULL);
  std::cout << "Took " << difftime(finish, timestamp) << " seconds" << std::endl;

  std::cout << "Getting Large cluster 1 000 000 points in R10 with 10 clusters to predict" << std::endl;
  timestamp = time(NULL);
  pred_points = mock_clusters(1000000, 10, 10);
  finish = time(NULL);
  std::cout << "Took " << difftime(finish, timestamp) << " seconds" << std::endl;

  std::cout << "Predicting Centroids on 1 000 000 points" << std::endl;
  timestamp = time(NULL);
  k.predict_cent(pred_points);
  finish = time(NULL);
  std::cout << "Took " << difftime(finish, timestamp) << " seconds" << std::endl;
}


void test_printing_distances() {
  vec *points = mock_vec(5, 3, 0, 10);
  for (int i = 0; i < 5; i++) 
    std::cout << points[i];

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      std::cout << i << " " << j << " " << l2(points[i], points[j]) << std::endl;
    }
  }
}

void test_clustering() {
  mat *train_points = mock_clusters(10, 3, 3);

  vec *matrix = mock_vec(10, 3, 0, 10);
  mat pred = mat(10, matrix);

  std::cout << "Training " << std::endl << *train_points;
  std::cout << std::endl;
  std::cout << "Pred " << std::endl << pred;

  knn k = knn(3);
  k.fit(train_points, 20);

  std::cout << "Centroids " << std::endl << *k.cent << std::endl;

  std::cout << "Trained KNN prediction " << std::endl << *k.predict_knn(train_points, 3) << std::endl;
  std::cout << "Trained Centroid prediction " << std::endl << *k.predict_cent(train_points) << std::endl;

  std::cout << "Pred KNN prediction " << std::endl << *k.predict_knn(&pred, 3) << std::endl;
  std::cout << "Pred Centroid prediction " << std::endl << *k.predict_cent(&pred) << std::endl;

}

void test_multiple_clusters() {
  mat *d1 = mock_clusters(10, 3, 3);
  mat *d2 = mock_clusters(10, 3, 3);
  mat *d3 = mock_clusters(10, 3, 3);

  std::cout << *d1 << *d2 << *d3 << std::endl;

}

int main() {
  srand(5);

  //test_multiple_clusters();
  //test_printing_distances();
  //mat *data = mock_clusters(10, 3, 3);

  //std::cout << *data << std::endl;
  //test_clustering();
  test_performance_of_clustering();

  return 0;
}
