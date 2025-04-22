#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>

#define N 8192

float getRandFloat();

int main(){

   float *matrix;
   float *vector;
   float *resultRow;
   float *resultColumn;

   time_t start;
   time_t end;
   time_t timeRows;
   time_t timeColumns;

   srand(time(NULL));

   matrix = (float *) malloc(sizeof(float) * N*N);
   vector = (float *) malloc(sizeof(float) * N);
   resultRow = (float *) malloc(sizeof(float) * N);
   resultColumn = (float *) malloc(sizeof(float) * N);

   for (int i = 0; i < N*N; i++){
      if (i<N)
         vector[i] = getRandFloat();

      matrix[i] = getRandFloat();
   }

   start = clock();
   for (int i = 0; i < N; i++){
      for (int j=0; j < N; j++){
         resultRow[i] += matrix[i*N+j] * vector[j];
      }
   }
   end = clock();
   timeRows = end - start;

   start = clock();
   for (int i = 0; i < N; i++){
      for (int j=0; j < N; j++){
         resultColumn[j] += matrix[i+j*N] * vector[i];
      }
   }
   end = clock();
   timeColumns = end - start;

   for (int i = 0; i < N; i++){}


   printf("Time passed (rows): %ld\n", timeRows);
   printf("Time passed (columns): %ld\n", timeColumns);

   for (int i = 0; i < N; i++){
      if (resultRow[i] != resultColumn[i])
         printf("Results do not match!\n");
   }

   free(matrix);
   free(vector);
   free(resultRow);
   free(resultColumn);

   return 0;
}

float getRandFloat(){
   return (float) (rand()) / (float)(rand());
}
