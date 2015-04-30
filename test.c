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
    while (done[k])
      k++;
    int lowest = importances[k];
    int lowest_index = k;

    for (int j = 0; j < n; j++)
      if (lowest > importances[j] && !done[j])
      {
        lowest_index = j;
        lowest = importances[j];
      }
  
    rank[lowest_index] = n - i - 1;
    done[lowest_index] = true;
  }  
  
  free(done);
  return rank;
}

int *distance_sort(int *rank, int *distances, int n)
{
  int *sorted = malloc(n * n * sizeof(int));
      
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      sorted[n * rank[j] + rank[i]] = distances[n * j + i];
  
  return sorted;
}

bool *reverse_matrix_sort(int *rank, bool *bools, int n)
{
  bool *ret = malloc(n * n * sizeof(bool));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      ret[n * i + j] = bools[n * rank[i] + rank[j]];
      
  return ret;
}

int main()
{
  // testing for make_rank
  int *importances = malloc(n * sizeof(int));
  importances[0] = 1;
  importances[1] = 7;
  importances[2] = 6;
  importances[3] = 8;
  int *ranks = make_rank(importances);
  for (int i = 0; i < n; i++) 
    printf("%d ", ranks[i]);
  printf("\n");
  free(ranks);
  
  // testing for distance_sort
  int *distances = malloc(n * n * sizeof(int));
  distances[0] = 0;
  distances[1] = 6;
  distances[2] = 6;
  distances[3] = 7;
  distances[4] = 6;
  distances[5] = 0;
  distances[6] = 8;
  distances[7] = 5;
  distances[8] = 6;
  distances[9] = 8;
  distances[10] = 0;
  distances[11] = 2;
  distances[12] = 7;
  distances[13] = 5;
  distances[14] = 2;
  distances[15] = 0;
  int *ranks0 = make_rank(importances);
  int *sorted_distances = distance_sort(ranks0, distances, n);
  for (int i = 0; i < 4; i++) 
  {  
    printf("\n");
    for (int j = 0; j < 4; j++)
      printf("%d ", sorted_distances[n * i + j]);
  }
  printf("\n");
  free(distances);
  free(ranks0);
  free(sorted_distances);
  free(importances);
  
  // testing for reverse_matrix_sort (at the end with the matrix of bools)
  int test_importances[4] = {3, 8, 2, 9};
  bool bools[16] =
  {0, 1, 1, 0,
   1, 0, 1, 0,
   1, 1, 0, 1,
   0, 0, 1, 0};
  
  int *ranks1 = make_rank(test_importances);
  bool *reversed = reverse_matrix_sort(ranks1, bools, n);
  for (int i = 0; i < 4; i++) 
  {  
    printf("\n");
    for (int j = 0; j < 4; j++)
      printf("%d ", reversed[n * i + j]);
  }
  free(ranks1);
  free(reversed);
}
