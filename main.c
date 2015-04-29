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
    do
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
        rc.optimality = times_to_optimality(t, times(traffic_dist(t,rc,t.n),t.n));
        
        if (connected(rc))
        {
            sum+= rc.optimality;
            num++;
            if (rc.optimality < opt[0])
                opt[0] = rc.optimality;
        }  
        printf("HERE\n");
        for (int i = 0; i < (t.n*t.n - t.n)/2; i++)
          printf("%d", tri[i]);
        printf(" ");
        next(tri, (t.n*t.n-t.n)/2);
    }
    while (!full(tri, (t.n*t.n-t.n)/2, edges));
    opt[1] = sum/num;
    //free(tri);
    //free(all);
    return opt;
}

void test_bruteforce()
{
    town t;
    t.n = 4;
    float dist[16] = {0,1,2,3,1,0,4,5,2,4,0,6,3,5,6,0};
    t.distances = dist;
    int importances[4] = {3,1,5,2};
    t.importances = importances;
    printf("%f \n %f \n", bruteforce(t,2)[0], bruteforce(t,2)[1]);
}

int main()  
{ 
  /*platform *test = malloc(sizeof(platform));
  *test = new_platform(5);
  printf("%d, %d", test->min_m, test->max_m);
  
  hi *x = malloc(sizeof(hi));
  x->importances = malloc(12 * sizeof(int));
  x->importances[3] = 5;
 // *x = y(3);
  printf("HERE: %d", x->importances[3]);
  
  */
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
  test_bruteforce();
}


