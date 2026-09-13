#include "b_search.h"
#include <stdlib.h>
#include <stdio.h>


// helpers
static void unsorted_initialization (int *arr, int n);
static void sorted_initialization (int *arr, int n);
static int linear_search_test(int *unsorted, int *sorted, int n);
static int binary_search_test(int *sorted, int n);

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

int linear_search_test( int *unsorted, int *sorted, int n){
  int fail = 0;

  int expected_check = linear_search(unsorted, n, 5);
  if(expected_check != 5){
    printf("- Error: function returned an index different fron 4: %d\n", expected_check);
    fail++;
  }
  else
    printf("- in bound key: pass\n");

  int out_of_bound_key = linear_search(unsorted, n, n+1);
  if(out_of_bound_key != -1){
    printf("- Error: called an out of bound index got:  %d\n", out_of_bound_key);
    fail++;
  }
  else
    printf("- out of bound key: pass\n");
  int first_element = sorted[0];
  int solution = linear_search(sorted, n, first_element);
  if (solution != 0){
    printf("- Error: binary search for %d returned index: %d\n", first_element, solution);
    fail++;
  }
  else
    printf("- first element found: pass\n");

  int err = linear_search(NULL, n, n);
  if (err != -2){
    printf("- Error: passed a NULL pointer and got  %d\n", err);
    fail++;
  }
  else
    printf("- broken pointer check: pass\n");

  return fail;

}


int binary_search_test(int *sorted, int n){
  int fail = 0;

  int first_element = sorted[0];
  int solution = b_search (sorted, 0, n-1, first_element);
  if (solution != 0){
    printf("- Error: binary search for %d returned index: %d\n", first_element, solution);
    fail++;
  }
  else
    printf("- first element found: pass\n");

  int m = n/2; // ill user m that is not a elementin the array to cek for out of odmain elements
  solution = b_search(sorted, 0, n - 1, m ); // probably this is disgusting coding but i'll just use this var
  if (solution == -1){
    printf("- absent value: pass\n");
  }
  else{
    printf("- Error: b_search returned %d\n", solution);
    fail++;
  }

  int test = sorted[m];  // ill look into a random element in the array if he finds it
  solution = b_search(sorted, 0, n - 1, test);
  if (solution != m){
    printf("- Error: binary search for %d returned index %d\n", test, solution);
    fail++;
  }
  else
    printf("- random middle element: pass\n");

  test = sorted[0] - 1;
  solution = b_search(sorted, 0, n - 1, test);
  if (solution != -1){
    printf("- Error: out of domain value didnt give the right error: %d\n",solution);
    fail++;
  }
  else
    printf("- absent out of bound <: pass\n");

  test = sorted[n-1] + 1;
  solution = b_search(sorted, 0, n - 1, test);
  if (solution != -1){
    printf("- Error: out of domain value didnt give the right error: %d\n",solution);
    fail++;
  }
  else
    printf("- absent out of bound >: pass\n");

  int * mono = malloc(sizeof (int));
  mono[0]= 1;
  solution = b_search(mono, 0, 0, 1);
  if(solution != 0){
    printf("- Error: has problem with array of 1 elements\n");
    fail++;
  }
  else
    printf("- single element array: pass\n");
  free(mono);
  return fail;
}



int main (void){
  const int n = 10;
  int *unsorted = malloc(sizeof(int) * n);
  int *sorted = malloc(sizeof(int) * n);

  int fail = 0;

  if (!unsorted){
    printf("malloc failed\n");
    return -2;
  }

  unsorted_initialization(unsorted, n);
  sorted_initialization(sorted, n);

  printf("correctness tests:\n");
  printf("\nlinear sort test:\n");

  fail += linear_search_test(unsorted, sorted, n);

  printf("\nbinary search tests:\n");

  fail += binary_search_test(sorted, n);








  free(sorted);
  free(unsorted);
  return 0;
}
