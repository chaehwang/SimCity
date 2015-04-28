#include <stdbool.h>

#define INFTY 999999
#define MAX_K 3
#define P 8

typedef struct {
  int n;
  float *distances;
  int *importances;
} town;

typedef struct {
  int n;
  int m;
  int *degree;
  bool *roads;
  float optimality;
} road_construction;

bool connected(road_construction rc);
int *degree(road_construction c);
float *traffic_dist(town t, road_construction c, int n);
float *times(float *td, int n);
float times_to_optimality(town t, float *times);
