#include <iostream>
#include <float.h>
#include "knn.h"
#include "point.h"
#include <algorithm>


centroids* calc_centroids(centroids *cent, clusters *cl) {
  // For each cluster calculate its centroid
  // Basically the point in the "middle" of the cluster
  // Here we use l2 distance
  double counts[cl->n_clusters];

  for (int i = 0; i < cl->n_clusters; i++)
    counts[i] = 0;


  int dim = cent->po->dim;

  //Start by setting all centroids to 0
  for (int i = 0; i < cl->n_clusters; i++) {
    for (int j = 0; j < dim; j++) {
      cent->po[i].co[j] = 0;
    }
  }

  // Calculate sum of dimensions per label
  for (int i = 0; i < cl->n_points; i++) {
    counts[cl->labels[i]] += 1.0;
    for (int j = 0; j < dim; j++) {
      cent->po[cl->labels[i]].co[j] += cl->po[i].co[j];
    }
  }

  //Divide by counts
  for (int i = 0; i < cl->n_clusters; i++) {
    for (int j = 0; j < dim; j++) {
      cent->po[i].co[j] = cent->po[i].co[j] / (counts[i] + 1e-15);
    }
  }


  return cent;
}


clusters* re_cluster(centroids *cent, clusters *cl) {
  // For each point calculate the distance to each centroid
  // let the point belong to the closest centroid

  for (int i = 0; i < cl->n_points; i++) {
    int closest = 0;
    double closest_distance = DBL_MAX, dist;
    for (int j = 0; j < cl->n_clusters; j++) {

      dist = l2_distance(cent->po[j], cl->po[i]);
      if (dist < closest_distance) {
        closest_distance = dist;
        closest = j;
      }
    }

    cl->labels[i] = closest;
  }

  return cl;
}

clusters* em(clusters *cl, int max_iter) {

  centroids *cent = new centroids();
  cent->po = new point[cl->n_clusters];

  // Initialise centroids
  for (int i = 0; i < cl->n_clusters; i++) {
    cent->po[i].co = new double[cl->dim];
    cent->po[i].dim = cl->dim;
  }


  // Run EM algorithm to estimate clusters
  for (int i = 0; i < max_iter; i++) {
    cent = calc_centroids(cent, cl);
    cl = re_cluster(cent, cl);
  }

  return cl;
}


clusters* predict(clusters *cl, point *po, int n_points, int k) {
  // Do a knn prediction where k is the number of neighbours to look at
  // We will use a heap to find the k closest points making the full complexity of this
  // O(n_points_to_predict * n_points_in_clusters * log (k))

  int heap[k];
  int neighbour_counts[cl->n_clusters];

  int *labels = new int[n_points];
  for (int i = 0; i < n_points; i++) {
    // Reset heap
    for (int j = 0; j < k; j++) {
      heap[j] = 0.0;
    }

    // Define comparator for this point
    auto comparator = [cl, i, po](int ii, int jj) {
      return l2_distance(cl->po[ii], po[i]) <= l2_distance(cl->po[jj], po[i]);
    };

    for (int j = 0; j < cl->n_points; j++) {
      // If element is smaller than largest element we shall update the heap
      if (l2_distance(cl->po[heap[0]], po[i]) > l2_distance(cl->po[j], po[i])) {
        // This moves largest element to k - 1
        std::pop_heap(heap, heap + k, comparator);
        // Insert into heap
        heap[k - 1] = j;
        // push element into "correct" position in heap
        std::push_heap(heap, heap + k, comparator);
      }
    }

    // Now we have the k neighbours for point i and need to find
    // the most common neighbour label
    // We shall count the neighbour labels and then pick the largest one
    // First we reset previous count
    for (int j = 0; j < cl->n_clusters; j++) {
      neighbour_counts[j] = 0;
    }

    // do the counting
    for (int j = 0; j < k; j++) {
      neighbour_counts[cl->labels[heap[j]]] += 1;
    }

    // find most common label
    int most_common = 0;
    int most_common_counts = 0;
    for (int j = 0; j < cl->n_clusters; j++) {
      if (neighbour_counts[j] > most_common_counts) {
        most_common = j;
        most_common_counts = neighbour_counts[j];
      }
    }

    // We now assign the label most_common to item i
    labels[i] = most_common;
  }


  clusters *preds = new clusters();
  preds->dim = cl->dim;
  preds->n_clusters = -1;
  preds->n_points = n_points;
  preds->po = po;
  preds->labels = labels;

  return preds;

}



