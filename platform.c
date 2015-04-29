#include <stdbool.h>
#include "platform.h"
// given an array of length n, if all edges are on the left
bool full(bool *array, int n, int edges)
{
  for (int i = 0; i < edges; i++)
    if (array[i] == 0)
      return false;
  return true;
}

void next(bool *array, int n)
{
  int last_true = n - 1;
  while (!array[last_true])
    last_true--;
  int to_increment = last_true;
  while (array[to_increment - 1])
    to_increment--;
  int num_to_move_back = last_true - to_increment;
  array[to_increment - 1] = 1;
  for (int i = to_increment; i < n - num_to_move_back; i++)
    array[i] = 0;
  for (int i = n - num_to_move_back; i < n; i++)
    array[i] = 1;
}

int min(int n1, int n2)
{
  if (n1 <= n2)
    return n1;
  else
    return n2;
}

platform *new_platform(int n)
{
  platform *new_p = malloc(sizeof(platform));
  new_p->n = n;
  new_p->min_m = n - 1;
  new_p->max_m = min(n * MAX_K / 2, (n * n - n)/2);
  int num_to_create = new_p->max_m - new_p->min_m + 1;
  new_p->optimal_constructions = 
    calloc(num_to_create, sizeof(road_construction));
  for (int i = 0; i < num_to_create; i++)
  {
    new_p->optimal_constructions[i].n = n;
    new_p->optimal_constructions[i].m = i + new_p->min_m;
    new_p->optimal_constructions[i].degree = calloc(n, sizeof(int));
    new_p->optimal_constructions[i].roads = calloc(n * n, sizeof(bool));
    new_p->optimal_constructions[i].optimality = INFTY;
  }
  return new_p;
}

void extend_platform(town t, platform *old_p, platform *new_p)
{
  int num_to_extend = old_p->max_m - old_p->min_m + 1;
  for (int i = 0; i < num_to_extend; i++)
    extend_rc(t, old_p->optimal_constructions[i], new_p);
}

void extend_rc(town t, road_construction rc, platform *new_p)
{
  for (int edges = 1; edges <= MAX_K; edges++)
  {
    if (rc.n < edges) 
      break;
    bool *roads = calloc(rc.n, sizeof(bool));
    for (int i = 0; i < rc.n; i++)
      roads[i] = 0;
    for (int i = rc.n - edges; i < rc.n; i++)
      roads[i] = 1;
      
    /* printf("Original: %d", edges);
    for (int i = 0; i < rc.n; i++)
    {
      printf("\n");
      for (int j = 0; j < rc.n; j++)
        printf("%d ", rc.roads[rc.n * i + j]);
    } */
   
    while(true)
    {
      road_construction new_rc;
      new_rc.n = rc.n + 1;
      new_rc.m = rc.m + edges;
      new_rc.degree = calloc(rc.n + 1, sizeof(int));
      for (int i = 0; i < rc.n; i++)
        new_rc.degree[i] = rc.degree[i];
      new_rc.degree[rc.n] = edges;
      new_rc.roads = calloc(new_rc.n * new_rc.n, sizeof(bool));

      for (int i = 0; i < new_rc.n; i++)
        for (int j = 0; j < new_rc.n; j++)
          new_rc.roads[new_rc.n * i + j] = 0;
      
      for (int i = 0; i < rc.n; i++)
        for (int j = 0; j < i; j++)
          new_rc.roads[new_rc.n * i + j] = new_rc.roads[new_rc.n * j + i] 
            = rc.roads[rc.n * i + j];
      
      /*printf("After");
      for (int i = 0; i < new_rc.n; i++)
    {
      printf("\n");
      for (int j = 0; j < new_rc.n; j++)
        printf("%d ", new_rc.roads[new_rc.n * i + j]);
    } */
        
      for (int i = 0; i < rc.n; i++)
        if (roads[i])
        {
          new_rc.roads[(rc.n + 1) * rc.n + i] = 
            new_rc.roads[(rc.n + 1)*i + rc.n] = 1;
          new_rc.degree[i]++;
        }
          
        // if maximum degree exceeded for some building, skip this combination 
        // of road construction
      bool max_k_exceeded = false;
      for (int i = 0; i < rc.n; i++)
        if (new_rc.degree[i] > MAX_K)
        {
          free(new_rc.degree);
          free(new_rc.roads);
          max_k_exceeded = true;
          break;
        }
      if (max_k_exceeded)
      {
        if(full(roads, rc.n, edges))
          break;
        next(roads, rc.n); // printf("Continue\n");
        continue;
      }
      else 
      {
        float *td = traffic_dist(t, new_rc, new_rc.n);
        float *times_matrix = times(td, new_rc.n);
        new_rc.optimality = times_to_optimality(t, times_matrix);
        /*for (int i = 0; i < new_rc.n; i++)
        { 
          printf("\n"); 
          for (int j = 0; j < new_rc.n; j++)
            printf("%d ", new_rc.roads[new_rc.n * i + j]);
        }
        printf("\n Degrees: ");
        for (int i = 0; i < new_rc.n; i++)
          printf("%d ", new_rc.degree[i]);
        printf("\n");
        printf("\n TD Matrix: ");
        for (int i = 0; i < new_rc.n; i++)
        { 
          printf("\n"); 
          for (int j = 0; j < new_rc.n; j++)
            printf("%f ", td[new_rc.n * i + j]);
        }
        printf("\n");
        printf("(Graph above): Here: %d, %f\n", edges, new_rc.optimality); */
        free(td);
        free(times_matrix);
        add_rc(new_rc, new_p);        
        if(full(roads, rc.n, edges))
          break;
        else
          next(roads, rc.n);
      }
    }
    free(roads);
  }
  
  /* printf("Town information: distances: \n");
  for (int i = 0; i < t.n; i++)
  {
    printf("\n");
    for (int j = 0; j < t.n; j++)
    {
      printf("%f ", t.distances[t.n * i + j]);
    }
  } */
}

// adds a road construction to the new platform if the road 
// construction is better than the one stored so far
void add_rc(road_construction rc, platform *new_p)
{
  int index = rc.m - new_p->min_m;
  if (new_p->optimal_constructions[index].optimality > rc.optimality)
  {
    free(new_p->optimal_constructions[index].degree);
    free(new_p->optimal_constructions[index].roads);
    new_p->optimal_constructions[index] = rc;
  }
  else 
  {
    free(rc.degree);
    free(rc.roads);
  }
}
