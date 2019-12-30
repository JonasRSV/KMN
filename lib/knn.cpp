#include <iostream>
#include "kmn.h"
#include <algorithm>

void kmn::kneigh(mat *points, int *labels) {
  this->clean();

  if (points->m == 0) 
    throw "Cannot fit on no data";

  this->c = this->calc_c(points->m, labels);


  this->n = points->matrix[0].n;

  this->cent = new mat(this->c, this->n);

  this->labels = labels;
  this->points = points;
  this->update_centroids();
}

preds* kmn::predict_knn(mat *points, int k) {
  // TODO: Paralellise the outer for loop of the predictions

  // Do a knn prediction where k is the number of neighbours to look at
  // We will use a heap to find the k closest points making the full complexity of this
  // O(n_points_to_predict * n_points_in_clusters * log (k))
  if (this->points == nullptr || this->labels == nullptr)
    throw "Cannot do predict knn before fitting (Run Kmeans or Kneigh)";

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
