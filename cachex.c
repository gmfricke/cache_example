#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** genRandomMatrix(int n, int max){
  int i, j;
  int** mat = malloc(n * sizeof(int*));
  for (i=0; i < n; i++){
    mat[i] = malloc(n * sizeof(int));
    for (j=0; j < n; j++)
      mat[i][j] = 1 + rand() % max;
  }
  return mat;
} 

void free_all(int** mat, int n){
  int i;
  for (i=0; i < n; i++)
    free(mat[i]);
  free(mat);
}

float averageMatRowMajor(int** mat, int n){
  int i, j, total=0;
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      total+= mat[i][j];
    }
  }
  return (float)total/(n*n);
}

float averageMatColumnMajor(int** mat, int n){
  int i, j, total=0;
  for (i = 0; i < n; i++){
    for (j = 0; j < n; j++){
      total+= mat[j][i];
    }
  }
  return (float)total/(n*n);
}

int main(int argc, char** argv) {
  if (argc != 2){
    fprintf(stderr, "usage: %s <n>\n", argv[0]);
    fprintf(stderr, "where <n> is the dimension of the matrix\n");
    return 1;
  }

  long long unsigned int row_time_diff = -1;
  long long unsigned int col_time_diff = -1;

  int i;
  int n = strtol(argv[1], NULL, 10);

  srand(time(NULL));
  
  int ** matrix = genRandomMatrix(n, 100);

  /* get CPU time used */
  struct timespec tstart;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tstart);
  
  float res = 0;
  res = averageMatRowMajor(matrix, n);

  struct timespec tend;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tend);
  
  row_time_diff = 1e9 * (tend.tv_sec - tstart.tv_sec) + tend.tv_nsec - tstart.tv_nsec;
  printf("Row major average is: %.2f; time is %llu nanoseconds\n", res, (long long unsigned int) row_time_diff);
  
  /* get CPU time used */
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tstart);

  res = averageMatColumnMajor(matrix, n);

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tend);
  
  col_time_diff = 1e9 * (tend.tv_sec - tstart.tv_sec) + tend.tv_nsec - tstart.tv_nsec;
  printf("Column major average is: %.2f; time is %llu nanoseconds\n", res, (long long unsigned int) col_time_diff);

  printf("\nRow major indexing was %f times faster than column major indexing.\n", row_time_diff/col_time_diff);

  free_all(matrix, n);
  return 0;
}
