#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Motivation for this program:
 * 
 * Will input town which has a pointer to importances [House1 (1), Police (7), FireStation (6), House2 (8)]
 * and distances
 * 
 *  0   6   6   7
 *  6   0   8  10
 *  6   8   0   2
 *  7  10   2   0
 * 
 * 
 * The remainder of the project requires that the initial matrix of importances be sorted, which
 * requires that the distances matrix be similarly "sorted".
 * 
 * 
 * So, this program will get an array of ints (importances) and an array of floats (distances).
 * 
 * Pseudocode:
 * 
 * (make_rank) Sorts array and creates a new array rank[n] with each rank[i] = j where i corresponds to the town
 * at i in the initial array and j corresponds to the location of the town in the sorted array.
 * 
 * (distance_sort) Now, sort all of the columns according to the rank matrix.  After that, sort all of the rows
 * according to the rank matrix.
 * 
 * Now, we have the desired output to give to the rest of the project.
 * 
 * 
 *  [VROOM VROOM VROOM goes the rest of the project]
 * 
 * 
 * (reversedistancesort) Now, take the output (this should be an array of 0's and 1's), and apply the reverse sort on the
 * columns and the rows according to the rank matrix.
 * 
 */


int n = 4;

int *make_rank(int *importances)
{
  int  *rank = malloc(n * sizeof(int));
  bool *done = malloc(n * sizeof(bool));

  for (int i = 0; i < n; i++)
  {
    int k = 0;
    while (done[k] == true)
    {
      k++;
    }
    int lowest = importances[k];
    int lowest_index = k;

    for (int j = 0; j < n; j++)
    {
      if (lowest > importances[j] && done[j] == false)
      {
        lowest_index = j;
        lowest = importances[j];
      }
    }
  
    rank[lowest_index] = n - i - 1;
    done[lowest_index] = true;

  }
  
  return rank;
}



float *distance_sort( int *rank, float *distances )
{
  float *distances_row = malloc(n * n * sizeof(float));
  float *distances_column = malloc(n * n * sizeof(float));
  
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      distances_row[n * rank[i] + j] = distances[n * i + j];
    }
  }
  
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      distances_column[n * j + rank[i]] = distances_row[n * j + i];
    }
  }
  
  return distances_column;
}


bool *reverse_matrix_sort( int *rank, bool *bools )
{
  bool *bools_row = malloc(n * n * sizeof(bool));
  bool *bools_column = malloc(n * n * sizeof(bool));
  
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      bools_row[n * i + j] = bools[n * rank[i] + j];
    }
  }
  
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      bools_column[n * j + i] = bools_row[n * j + rank[i]];
    }
  }
  
  return bools_column;
}



int main()
{
  // testing for make_rank
  int *importances = malloc(n * sizeof(int));
  importances[0] = 1;
  importances[1] = 7;
  importances[2] = 6;
  importances[3] = 8;
  for (int i = 0; i < n; i++) {printf("%d\n", (make_rank(importances))[i]);}
  
  // testing for distance_sort
  float *distances = malloc(n * n * sizeof(float));
  distances[0] = 0;
  distances[1] = 6;
  distances[2] = 6;
  distances[3] = 7;
  distances[4] = 6;
  distances[5] = 0;
  distances[6] = 8;
  distances[7] = 10;
  distances[8] = 6;
  distances[9] = 8;
  distances[10] = 0;
  distances[11] = 2;
  distances[12] = 7;
  distances[13] = 10;
  distances[14] = 2;
  distances[15] = 0;
  for (int i = 0; i < 16; i++) {printf("%f\n", (distance_sort( (make_rank(importances)), distances ))[i]);}
  
  // testing for reverse_matrix_sort (at the end with the matrix of bools)
  bool *bools = malloc(n * n * sizeof(bool));
  bools[0] = true;
  bools[1] = true;
  bools[2] = false;
  bools[3] = false;
  bools[4] = true;
  bools[5] = false;
  bools[6] = true;
  bools[7] = false;
  bools[8] = false;
  bools[9] = true;
  bools[10] = true;
  bools[11] = true;
  bools[12] = false;
  bools[13] = false;
  bools[14] = true;
  bools[15] = true;
  for (int i = 0; i < 16; i++) {printf("%d\n", (reverse_matrix_sort( make_rank(importances), bools ))[i]);}
}









