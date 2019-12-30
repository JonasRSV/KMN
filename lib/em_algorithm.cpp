#include "kmn.h"
#include <float.h>

preds* kmn::kmeans(mat *points, int max_iter) {
  // First clean up from any earlier run
  this->clean();

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
    this->update_centroids();
    this->update_clusters();
  }

  return new preds(this->labels, this->points);
}


void kmn::update_centroids() {
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

void kmn::update_clusters() {
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
