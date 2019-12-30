#include <iostream>
#include <functional>
#include <cstring>
#include <string>

#pragma once

//=============== Vector Class =================

class vec {
public:
  int n;
  double *vector;

  vec(): n(0), vector(nullptr) {}
  vec(int n);
  vec(int n, double *vector): n(n), vector(vector){}

  void operator = (vec const &v) {
    this->n = v.n;
    this->vector = new double[v.n];
    memcpy(this->vector, v.vector, sizeof(double) * v.n);
  }

  void operator = (vec *v) {
    this->n = v->n;
    this->vector = new double[v->n];
    memcpy(this->vector, v->vector, sizeof(double) * v->n);
  }

  ~vec();

  void rand(double f_min, double f_max);
};

std::ostream &operator<<(std::ostream &os, vec const &vector);
//=============== Matrix Class =================

class mat {
  public:
  int m;
  vec *matrix;

  mat(): m(0), matrix(nullptr) {};
  mat(int m, int n);
  mat(int m, vec *matrix): m(m), matrix(matrix){}
  ~mat();
  void extend(mat *matrix);
};

std::ostream &operator<<(std::ostream &os, mat const &matrix);
//=============== Predictions Class =================

class preds {
  public:
  int *labels;
  mat *points;
  preds(int *labels, mat *points): labels(labels), points(points){}
};

std::ostream &operator<<(std::ostream &os, preds const &p);
//=============== KMN Class =================
typedef double (*distance)(const vec&, const vec&); //std::function<double(const vec&, const vec&)> distance;

class kmn {
  // Distance between two points
  distance d;

  // Data constructed by fitting
  int *labels;
  mat *points;
  int n;

  private:
  void update_centroids();
  void update_clusters();
  void clean();
  int calc_c(int m, int *labels);



  public:
  mat *cent;
  // Number of clusters to find
  int c;

  // For KNN
  kmn();
  kmn(distance d): d(d), labels(nullptr), points(nullptr), cent(nullptr), c(0) {}

  // For Kmeans
  kmn(int c);
  kmn(int c, distance d): d(d), labels(nullptr), points(nullptr), cent(nullptr), c(c) {}
  ~kmn();

  void kneigh(mat *points, int *labels);
  preds* kmeans(mat *points, int max_iter);
  preds* predict_knn(mat *points, int k);
  preds* predict_cent(mat *points);
};



double l2(const vec&, const vec&);
