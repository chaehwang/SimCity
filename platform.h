#include <stdbool.h>
#include "roads.c"

typedef struct {
  int n;
  int min_m;
  int max_m;
  road_construction *optimal_constructions;
} platform;

platform new_platform(int n);
void extend_platform(town t, platform *old_p, platform *new_p);
void extend_rc(town t, road_construction rc, platform *new_p);
void add_rc(road_construction rc, platform *new_p); 

