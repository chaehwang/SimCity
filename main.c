#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test.c"
#include "platform.c"

float *bruteforce (town t, int edges)
{
    int tri_length = (t.n * t.n - t.n)/2;
    bool *tri = calloc(tri_length, sizeof(bool));
    bool *all = calloc((t.n*t.n), sizeof(bool));
    float sum = 0;
    int num = 0;
    float *opt = calloc(2, sizeof(float));  
    opt[0] = INFTY; 
    for (int i = tri_length - edges; i < tri_length; i++)
    {
        tri[i] = 1;
    }
    while(true)
    {
        int counter = 0;
        for (int i = 1; i < t.n; i++)
        {
            for(int j = 0; j < i; j++)
            {
              all[t.n * i + j] = all[t.n * j + i] = tri[counter];
              counter++;
            }
        }
        road_construction rc;
        rc.n = t.n;
        rc.m = edges;
        rc.roads = calloc(t.n * t.n, sizeof(bool));
        for (int i = 0; i < t.n * t.n; i++)
          rc.roads[i] = all[i];
        if (!connected(rc))
        {
          free(rc.roads);
          if (full(tri, tri_length, edges))
            break;
          next(tri, tri_length);
          continue;
        }
         
        rc.degree = degree(all,t.n);
        float *td = traffic_dist(t, rc, rc.n);
        float *times_matrix = times(td, rc.n);
        rc.optimality = times_to_optimality(t, times_matrix);

        free(rc.degree);
        free(td);
        free(times_matrix);      
        
            sum+= rc.optimality;
            num++;
            if (rc.optimality < opt[0])
            {     
              opt[0] = rc.optimality;
            }
          

        free(rc.roads);  
        if (full(tri, tri_length, edges))
          break;
        next(tri, tri_length);
    }
    opt[1] = sum/num;
    free(tri);
    free(all); 
    return opt;
}
 
void test_bruteforce(town t)
{
    /* printf("Brute force \n");
    town t;
    t.n = 3;
    int dist[9] = {0,3,5,3,0,4,5,4,0};
    t.distances = dist;
    int importances[3] = {1,2,3};
    t.importances = importances; */
    //printf("MIN: %f \t AVERAGE: %f\n", bruteforce(t,2)[0], bruteforce(t,2)[1]);
    int max_edges = min((t.n * t.n - t.n)/2, t.n * MAX_K/2);
    for (int i = t.n - 1; i <= max_edges; i++)
    {
      float *bf = bruteforce(t,i);
      printf("%d edges: MIN: %f AVERAGE: %f\n", i, bf[0], bf[1]);
      free(bf);
    }
}

int main()  
{ 
  /*town test_town;
  test_town.n = 6;
  test_town.importances = malloc(test_town.n * sizeof(int));
  for (int i = 0; i < 6; i++)
    test_town.importances[i] = 1; 
  test_town.distances = malloc(test_town.n * test_town.n * sizeof(int));
  int dist[36] = 
    {0,1,3,2,1,1,
     1,0,9,4,3,1,
     3,9,0,2,3,2,
     2,4,2,0,6,1,
     1,3,3,6,0,3,
     1,1,2,1,3,0};
  test_town.distances = dist;
  
  road_construction r;
  r.n = 6; r.m = 8; 
  int r_degree[6] = {3,3,3,2,3,2};
  r.degree = r_degree;
  bool r_roads[36] =
    {0,1,1,0,1,0,
     1,0,1,1,0,0,
     1,1,0,0,1,0,
     0,1,0,0,0,1,
     1,0,1,0,0,1,
     0,0,0,1,1,0};
  r.roads = r_roads; */
  
  platform *cur_p = new_platform(2);
  cur_p->optimal_constructions[0].degree[0] = 1;
  cur_p->optimal_constructions[0].degree[1] = 1;
  cur_p->optimal_constructions[0].roads[0] = 0;
  cur_p->optimal_constructions[0].roads[1] = 1;
  cur_p->optimal_constructions[0].roads[2] = 1;
  cur_p->optimal_constructions[0].roads[3] = 0;
  
  int n;
  printf("Number of buildings: ");
  scanf("%d", &n);
  town t;
  t.n = n;
  t.distances = calloc(n * n, sizeof(int));
  t.importances = calloc(n, sizeof(int)); 
  printf("\nImportance array, separated by spaces: \n");
  for (int i = 0; i < n; i++)
    scanf("%d", &t.importances[i]);
  printf("\nDistance matrix, separated by spaces: \n");
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j <= i; j++)
    {
      printf("%d ", t.distances[n * i + j]);
    }
    
    for (int j = i + 1; j < n; j++)
    {
      scanf("%d", &t.distances[n * i + j]);
      t.distances[n * j + i] = t.distances[n * i + j]; 
    }
  }
  
  // Heuristic start
  time_t start_t, end_t;
  time(&start_t);
  
  // free
  int *ranks = make_rank(t.importances, t.n);
  int *sorted_importances = importance_sort(ranks, t.importances, t.n);
  int *sorted_distances = distance_sort(ranks, t.distances, t.n);
  free(t.importances);
  free(t.distances);
  t.importances = sorted_importances;
  t.distances = sorted_distances;
  
  
  for (int i = 3; i <= n; i++) 
  {
    platform *new_p = new_platform(i);
    town sub_town; 
    sub_town.n = i;
    sub_town.distances = calloc(i * i, sizeof(float));
    sub_town.importances = calloc(i, sizeof(int));
    for (int j = 0; j < i; j++)
    {
      sub_town.importances[j] = t.importances[j];
      for (int k = j + 1; k < i; k++)
        sub_town.distances[i * j + k] = sub_town.distances[i * k + j] = t.distances[n * j + k];
    } 
    extend_platform(sub_town, cur_p, new_p);
    free(sub_town.distances);
    free(sub_town.importances);
    int num_constructions = cur_p->max_m - cur_p->min_m + 1;
    for (int j = 0; j < num_constructions; j++)
    {
      free(cur_p->optimal_constructions[j].degree);
      free(cur_p->optimal_constructions[j].roads);
    }
    free(cur_p->optimal_constructions);
    free(cur_p);
    cur_p = new_p;    
  }  
  
  printf("\n\nHeuristic Results: \n");
  int num_rc = cur_p->max_m - cur_p->min_m + 1;
  // Print out adjacency matrix for each m
  
  FILE *f = fopen("file.txt", "w");
  
  for (int i = 0; i < num_rc; i++)
  {
      printf("%d edges: %f\n", i + cur_p->min_m, 
        cur_p->optimal_constructions[i].optimality);

  
  //prints out to a text file
      if(f==NULL)
      {
        printf("Error in opening file!\n");
        exit(1);
      }
      
      fprintf(f, "%d edges: %f\n", i + cur_p->min_m, 
        cur_p->optimal_constructions[i].optimality);
      fprintf(f, "\n");
      for(int k=0; k<n; k++)
      {
        for(int j=0; j<n; j++)
        {
            fprintf(f, "%d ", (cur_p->optimal_constructions[i].roads[n*k+j]));
        }
        fprintf(f, "\n");
      }
      fprintf(f, "\n");
  //fprintf(f, "Some text: %s\n", text);
  //fprintf(f, "Some text: %s\n", text);
  
  }
  fclose(f);
  // Heuristic end 
  time(&end_t);
  double diff_t = difftime(end_t, start_t);
  printf("Heuristic Time: %f seconds\n\n", diff_t);
  
  //free(cur_p->optimal_constructions->degree);
  //free(cur_p->optimal_constructions->roads);
  int num_constructions = cur_p->max_m - cur_p->min_m + 1;
    for (int j = 0; j < num_constructions; j++)
    {
      free(cur_p->optimal_constructions[j].degree);
      free(cur_p->optimal_constructions[j].roads);
    }
  free(cur_p->optimal_constructions);
  free(cur_p);
 
  int bf = 0;
  printf("Bruteforce Search? 1 = Yes, 0 = No: ");
    scanf("%d", &bf);
  if (bf)
  {
    printf("Brute Force Results: \n\n");
    time_t start_t, end_t;
    time(&start_t);
    test_bruteforce(t);
    time(&end_t);
    double diff_t = difftime(end_t, start_t);
    printf("\nBrute Force Time: %f seconds\n", diff_t);
  }
  
  
  free(ranks);
  free(t.distances);
  free(t.importances);

}
