#include "roads.h"
#include <stdio.h>

bool connected(road_construction rc)
{
  int n = rc.n;
  // boolean for a node is connected to a base node. the graph
  // is connected if this is true for all node
  bool *connected_to_b0 = malloc(n * sizeof(bool));
  // boolean for if we have already made all neighbors of this
  // node have connected_to_b0 value of false
  bool *checked = malloc(n * sizeof(bool));
  
  
  connected_to_b0[0] = true;
  int num_connected_to_b0 = 1;
  
  while(num_connected_to_b0 < n)
  {
    // find the first node that is connected to base but not checked
    int current_node = -1;
    for (int i = 0; i < n; i++)
    {
      if (!checked[i] && connected_to_b0[i])
      {
        current_node = i;
        break;
      }
    }
    
    // if no such node exists, then no other nodes are connected to b0
    if (current_node == -1)
      return false;
    // otherwise, set connected_to_b0 for all neighbors to true
    else
    {  
      for (int i = 0; i < n; i++)
      {
        if (rc.roads[n * current_node + i]  && !connected_to_b0[i])
        {
          connected_to_b0[i] = true;
          num_connected_to_b0++;
        }
      }
      checked[current_node] = true;
    }
  }
  
  // if while loop was exited, then all nodes are connected to b0
  return true;
}

int *degree(road_construction rc)
{
  int *node_degree = malloc(rc.n * sizeof(int));
  for (int i = 0; i < rc.n; i++)
  {
    for (int j = i + 1; j < rc.n; j++)
      if (rc.roads[rc.n * i + j])
      {
        node_degree[i]++;
        node_degree[j]++;
      }
  }
  return node_degree;
}

// should return true
bool test_connected1()
{
  bool roads[25] = {0,0,1,0,0,0,0,1,0,1,1,1,0,1,1,0,0,1,0,0,0,1,0,0,0};
  road_construction rc;
  rc.roads = roads;
  rc.n = 5;
  return connected(rc);
}

// should return false
bool test_connected2()
{
  bool roads[25] = {0,0,0,0,0,0,0,1,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,0};
  road_construction rc;
  rc.roads = roads;
  rc.n = 5;
  return connected(rc);
}

// should return 1 2 4 1 2
void test_degrees()
{
  bool roads[25] = {0,0,1,0,0,0,0,1,0,1,1,1,0,1,1,0,0,1,0,0,0,1,0,0,0};
  road_construction rc;
  rc.roads = roads;
  rc.n = 5;
  int *degrees = degree(rc);
  for (int i = 0; i < 5; i++)
    printf("%d ", degrees[i]);
 // free(degrees);
}
float *traffic_dist(town t, road_construction c, int n)
{
    float *dist = malloc(n*n*sizeof(float));
    t.n=n;
   // int *importance = t.importances;
    int *degrees = degree(c);
    
    for(int i = 0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(c.roads[n*i+j])
            {
                //printf("%d and %d also %d plus %d", t.importances[i],degrees[i], t.importances[j], degrees[j]);
                dist[n*i+j]= t.distances[n*i+j]*(1+((float)t.importances[j]/(float)degrees[j]) + ((float)t.importances[i]/degrees[i]));
                //printf("%f", dist[n*i+j]);
            }
            else
                dist[n*i+j]=INFTY;
        }
    }
   
   return dist;
}

void test_distance()
{
    bool roads[25] = {0,0,1,0,0,0,0,1,0,1,1,1,0,1,1,0,0,1,0,0,0,1,1,0,0};
    road_construction rc;
    rc.roads = roads;
    rc.n = 5;
    
    town t;
    float distance[25]= {0.0,2.0,1.0,4.0,0.0,5.0,6.0,1.0,0.0,1.0,1.0,1.0,2.0,1.0,1.0,2.0,0.0,1.0,0.0,0.0,0.0,1.0,1.0,0.0,0.0};
    int important[5]={1,2,3,4,5};
    t.n = 5;
    t.distances = distance;
    t.importances = important;
    
    float *test = traffic_dist(t, rc, 5);
    for(int i = 0; i<25; i++)
    {
        printf("%f ", test[i]);
    }
    for(int i=0; i<5; i++)
    {
        printf("%d ", t.importances[i]);
    }
    free(test);
}


