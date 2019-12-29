#include <iostream>
#include <float.h>
#include "knn.h"
#include <algorithm>


knn::knn(int c) {
  this->c = c;
  this->d = l2;

  this->labels = nullptr;
  this->cent = nullptr;

}


preds* knn::fit(mat *points, int max_iter) {
  // First clean up from any earlier run
  if (this->labels != nullptr)
    delete[] this->labels;

  if (this->cent != nullptr)
    delete[] this->cent;

  if (points->m == 0) 
    throw "Cannot fit on no data";

  // Prepair for the EM algorithm
  this->n = points->matrix[0].n;
  this->points = points;

  // Initialise labels randomly
  this->labels = new int[points->m];
  for (int i = 0; i < points->m; i++) {
    labels[i] = std::rand() % this->c;
  }

  this->cent = new mat(this->c, this->n);

  // Run EM algorithm to estimate clusters
  for (int i = 0; i < max_iter; i++) {
    update_centroids();
    update_clusters();
  }

  return new preds(this->labels, this->points);
}


void knn::update_centroids() {
  // Find the center of each cluster
  double counts[this->c];
  for (int i = 0; i < this->c; i++)
    counts[i] = 0.0;

  // Reset centroids
  for (int i = 0; i < this->c; i++) {
    for (int j = 0; j < this->n; j++) {
      this->cent->matrix[i].vector[j] = 0;
    }
  }

  // Calculate sum of dimensions per label
  for (int i = 0; i < this->points->m; i++) {
    counts[this->labels[i]] += 1.0;
    for (int j = 0; j < this->n; j++) {
      this->cent->matrix[this->labels[i]].vector[j] += this->points->matrix[i].vector[j];
    }
  }

  //Divide by counts
  for (int i = 0; i < this->c; i++) {
    for (int j = 0; j < this->n; j++) {
      this->cent->matrix[i].vector[j] = this->cent->matrix[i].vector[j] / (counts[i] + 1e-15);
    }
  }
}

void knn::update_clusters() {
  // For each center let each point belong to the center closest to it
  for (int i = 0; i < this->points->m; i++) {
    int label = 0;
    double dist_min = DBL_MAX, dist;
    for (int j = 0; j < this->c; j++) {
      dist = this->d(this->cent->matrix[j], this->points->matrix[i]);
      if (dist < dist_min) {
        dist_min = dist;
        label = j;
      }
    }
    this->labels[i] = label;
  }
}


preds* knn::predict_knn(mat *points, int k) {
  // TODO: Paralellise the outer for loop of the predictions

  // Do a knn prediction where k is the number of neighbours to look at
  // We will use a heap to find the k closest points making the full complexity of this
  // O(n_points_to_predict * n_points_in_clusters * log (k))

  int heap[k];
  int counts[this->c];

  int *labels = new int[points->m];
  for (int i = 0; i < points->m; i++) {

    // Reset heap
    for (int j = 0; j < k; j++) 
      heap[j] = 0.0;

    // Define comparator for this point
    auto comparator = [i, this, points](int i1, int i2) {
      return this->d(this->points->matrix[i1], points->matrix[i]) <= this->d(this->points->matrix[i2], points->matrix[i]);
    };

    for (int j = 0; j < this->points->m; j++) {
      // If element is smaller than largest element we shall update the heap
      if (comparator(j, heap[0])) { 
        // This moves largest element to k - 1
        std::pop_heap(heap, heap + k, comparator);
        // Insert into heap
        heap[k - 1] = j;
        // push element into "correct" position in heap
        std::push_heap(heap, heap + k, comparator);
      }
    }

    // Now find the type of neighbour that is most prevalent
    for (int j = 0; j < this->c; j++)
      counts[j] = 0;

    for (int j = 0; j < k; j++) 
      counts[this->labels[heap[j]]] += 1;

    // find most common label
    int label = 0;
    int most = 0;
    for (int j = 0; j < this->c; j++) {
      if (counts[j] > most) {
        label = j;
        most = counts[j];
      }
    }

    labels[i] = label;
  }

  return new preds(labels, points);
}

preds* knn::predict_cent(mat *points) {
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
