#include "b_search.h"
#include <stdlib.h>
#include <stdio.h>


// helpers
static void unsorted_initialization (int *arr, int n);
static void sorted_initialization (int *arr, int n);
static void linear_search_test(int *unsorted, int *sorted, int n);
static void binary_search_test(int *sorted, int n);

void unsorted_initialization (int *arr, int n){
  if(!arr)
    return;
  for (int i = 0; i<n; i++)
    arr[i] = n - i;
}

static void sorted_initialization (int *arr, int n){
  if (!arr)
    return;
  for (int i = 0; i < n; i++)
    arr[i] = 2 * i;
}

static void linear_search_test( int *unsorted, int *sorted, int n){

  int expected_check = linear_search(unsorted, n, 5);
  if(expected_check != 5)
    printf("- Error: function returned an index different fron 4 : arr[%d] = %d\n", expected_check, unsorted[expected_check]);
  else
    printf("- in bound key: pass\n");

  int out_of_bound_key = linear_search(unsorted, n, n+1);
  if(out_of_bound_key != -1)
    printf("- Error: called an out of bound index got:  %d\n", out_of_bound_key);
  else
    printf("- out of bound key: pass\n");
  int first_element = sorted[0];
  int solution = linear_search(sorted, n, first_element);
  if (solution != 0)
    printf("- Error: binary search for %d returned index: %d that references sorted[%d] = %d\n", first_element, solution, solution, sorted[solution]);
  else
    printf("- first element found: pass\n");
  int err = linear_search(NULL, n, n);
  if (err != -2)
    printf("- Error: passed a NULL pointer and got  %d\n", err);
  else
    printf("- broken pointer check: pass\n");



}


static void binary_search_test(int *sorted, int n){
  int first_element = sorted[0];
  int solution = b_search (sorted, 0, n, first_element);
  if (solution != 0)
    printf("- Error: binary search for %d returned index: %d that references sorted[%d] = %d\n", first_element, solution, solution, sorted[solution]);
  else
    printf("- first element found: pass\n");

  int m = n/2; // ill user m that is not a elementin the array to cek for out of odmain elements
  solution = b_search(sorted, 0, n - 1, m ); // probably this is disgusting coding but i'll just use this var
  if (solution == -1){
    printf("- absent value: pass\n");
  }
  else
    printf("- Error: b_search returned %d\n", solution);

  int test = sorted[m];  // ill look into a random element in the array if he finds it
  solution = b_search(sorted, 0, n - 1, test);
  if (solution != m)
    printf("- Error: binary search for %d returned index %d which is %d\n", test, solution, sorted[solution]);
  else
    printf("- random middle element: pass\n");

  test = sorted[0] - 1;
  solution = b_search(sorted, 0, n - 1, test);
  if (solution != -1)
    printf("- Error: out of domain value didnt give the right error: %d\n",solution);
  else
    printf("- absent out of bound <: pass\n");

  test = sorted[n-1] + 1;
  solution = b_search(sorted, 0, n - 1, test);
  if (solution != -1)
    printf("- Error: out of domain value didnt give the right error: %d\n",solution);
  else
    printf("- absent out of bound >: pass\n");

  int * mono = malloc(sizeof (int));
  mono[0]= 1;
  solution = b_search(mono, 0, 1, 1);
  if(solution != 0)
    printf("- Error: has problem with array of 1 elements\n");
  else
    printf("- single element array: pass\n");
}



int main (void){
  const int n = 10;
  int *unsorted = malloc(sizeof(int) * n);
  int *sorted = malloc(sizeof(int) * n);


  if (!unsorted){
    printf("malloc failed\n");
    return -2;
  }

  unsorted_initialization(unsorted, n);
  sorted_initialization(sorted, n);

  printf("correctness tests:\n");
  printf("\nlinear sort test:\n");

  linear_search_test(unsorted, sorted, n);

  printf("\nbinary search tests:\n");

  binary_search_test(sorted, n);








  free(sorted);
  free(unsorted);
  return 0;
}
