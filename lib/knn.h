#include "point.h"
#include "cluster.h"

#pragma once

struct centroids {
  point *po;
};

centroids* calc_centroids(centroids *cent, clusters *cl);
clusters* re_cluster(centroids *cent, clusters *cl);
clusters* em(clusters *cl, int max_iter);
clusters* predict(clusters *cl, point *po, int n_points, int k);


