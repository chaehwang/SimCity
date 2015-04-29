#include <stdio.h>
#include <stdlib.h>
#include "platform.c"

typedef struct {
  int n;
  int *importances;
} hi; 

int y(int t)
{ 
  return 3;  
}

float *bruteforce (town t, int edges)
{
    bool *tri = malloc((t.n*t.n-t.n)/2*sizeof(bool));
    bool *all = malloc((t.n*t.n)*sizeof(bool));
    float sum = 0;
    int num = 0;
    float *opt = malloc(sizeof(float)*2);
    opt[0] = INFTY;
    for (int i=(t.n*t.n-t.n)/2-edges; i<(t.n*t.n-t.n)/2; i++)
    {
        tri[i]=1;
    }
    while(true)
    {
        int counter = 0;
        for (int i=0; i<t.n;i++)
        {
            for(int j=0; j<=i;j++)
            {
                if (i==j)
                {
                    all[t.n*i+j] = 0;
                }
                else
                {
                    all[t.n*i+j] = all[t.n*j+i] = tri[counter];
                    counter++;
                }
            }
        }
        road_construction rc;
        rc.n = t.n;
        rc.m = edges;
        rc.roads = malloc(t.n * t.n * sizeof(bool));
        for (int i = 0; i < t.n*t.n; i++)
          rc.roads[i] = all[i];
        //rc.roads = all;
        rc.degree = degree(all,t.n);
        // TODO: free times and traffic_dist
        rc.optimality = times_to_optimality(t, times(traffic_dist(t,rc,t.n),t.n));
        
        if (connected(rc))
        {
            sum+= rc.optimality;
            num++;
            if (rc.optimality < opt[0])
                opt[0] = rc.optimality;
        }
        /*for (int i = 0; i < (t.n*t.n-t.n)/2; i++)
            printf("%d", tri[i]);
        printf(" ");*/  
        if (full(tri, (t.n*t.n-t.n)/2, edges))
            break;
        next(tri, (t.n*t.n-t.n)/2);
    }
    opt[1] = sum/num;
    //free(tri);
    //free(all);
    return opt;
}

void test_bruteforce()
{
    town t;
    t.n = 3;
    float dist[9] = {0,3,4,3,0,5,4,5,0};
    t.distances = dist;
    int importances[3] = {1,1,1};
    t.importances = importances;
    printf("MIN: %f\nAVERAGE: %f\n", bruteforce(t,2)[0], bruteforce(t,2)[1]);
}

int main()  
{ 
  town test_town;
  test_town.n = 4;
  test_town.importances = malloc(test_town.n * sizeof(int));
  for (int i = 0; i < 4; i++)
    test_town.importances[i] = 1; 
  test_town.distances = malloc(test_town.n * test_town.n * sizeof(int));
  test_town.distances[1] = 3;
  test_town.distances[2] = 5;
  test_town.distances[3] = 7;
  test_town.distances[4] = 3;
  test_town.distances[6] = 3;
  test_town.distances[7] = 9;
  test_town.distances[8] = 5;
  test_town.distances[9] = 3;
  test_town.distances[11] = 5;
  test_town.distances[12] = 7;
  test_town.distances[13] = 9;
  test_town.distances[14] = 5;
  //platform *test = new_platform(5);
  //printf("%d, %d", test->min_m, test->max_m);
  
  
  platform *cur_p = new_platform(2); 
  cur_p->optimal_constructions[0].degree[0] = 1;
  cur_p->optimal_constructions[0].degree[1] = 1;
  cur_p->optimal_constructions[0].roads[0] = 0;
  cur_p->optimal_constructions[0].roads[1] = 1;
  cur_p->optimal_constructions[0].roads[2] = 0;
  cur_p->optimal_constructions[0].roads[3] = 1;
  //printf("%d, %d", test.min_m, test.max_m);
       
    /*platform *new_p = new_platform(3);
    town sub_town;
    sub_town.n = 3;
    sub_town.distances = malloc(3 * 3 * sizeof(float));
    sub_town.importances = malloc(3 * sizeof(int));
    for (int j = 0; j < 3; j++)
    {
      sub_town.importances[j] = test_town.importances[j];
      for (int k = 0; k < 3; k++)
        sub_town.distances[3 * j + k] = sub_town.distances[3 * k + j] = test_town.distances[4 * j + k];
    }
    extend_platform(sub_town, cur_p, new_p);
     */
  
  for (int i = 3; i <= 4; i++)
  {
    platform *new_p = new_platform(i);
    town sub_town;
    sub_town.n = i;
    sub_town.distances = malloc(i * i * sizeof(float));
    sub_town.importances = malloc(i * sizeof(int));
    for (int j = 0; j < i; j++)
    {
      sub_town.importances[j] = test_town.importances[j];
      for (int k = j + 1; k < i; k++)
        sub_town.distances[i * j + k] = sub_town.distances[i * k + j] = test_town.distances[4 * j + k];
    } 
    extend_platform(sub_town, cur_p, new_p);
    free(cur_p->optimal_constructions);
    free(cur_p);
    cur_p = new_p;    
  }  
  
  /* bool *arr = malloc(3 * sizeof(bool));
  arr[0] = 0; arr[1] = 0; arr[2] = 1;
  next(arr, 3);
  for (int i = 0; i < 3; i++)
    printf("%d ", arr[i]); */
  /*
  hi *x = malloc(sizeof(hi)); 
  x->importances = malloc(12 * sizeof(int));
  x->importances[3] = 5;
 // *x = y(3);
  printf("HERE: %d", x->importances[3]); */
  
  /* int *test = malloc(12 * sizeof(road_construction)); */
  
  /* if (test[0] == NULL)
    printf("hi"); */ 
  /* test[5] = 10;
  test[3] = 8;
  for (int i = 0; i < 12; i++)
  { 
    printf("%d\n", test[i]);
  }
  printf("%d\n", sizeof(int)); */
  //test_bruteforce();
}


