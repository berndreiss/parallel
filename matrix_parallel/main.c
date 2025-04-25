#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>

#define RUNS 1

float getRandFloat();
double matrixMult(int n);
void run(int n);
void initMatrix(int n);

float *matrix;
float *vector;
float *result;

int main(){


   srand(time(NULL));

   initMatrix(1024);

   printf("Type, 1, 2, 4, 8\n");

   printf("static (16): 1024, ");
   omp_set_schedule(omp_sched_static, 16);
   run(1024);

   printf("static (32): 1024, ");
   omp_set_schedule(omp_sched_static, 32);
   run(1024);

   printf("dynamic (1): 1024, ");
   omp_set_schedule(omp_sched_dynamic, 1);
   run(1024);
   
   printf("dynamic (4): 1024, ");
   omp_set_schedule(omp_sched_dynamic, 4);
   run(1024);

   free(matrix);
   free(vector);
   free(result);

   initMatrix(2048);

   printf("Type, 1, 2, 4, 8\n");
   printf("static (16): 2048, ");
   omp_set_schedule(omp_sched_static, 16);
   run(2048);

   printf("static (32): 2048, ");
   omp_set_schedule(omp_sched_static, 32);
   run(2048);
   
   printf("dynamic (1): 2048, ");
   omp_set_schedule(omp_sched_dynamic, 1);
   run(2048);

   printf("dynamic (4): 2048, ");
   omp_set_schedule(omp_sched_dynamic, 4);
   run(2048);

   free(matrix);
   free(vector);
   free(result);

   initMatrix(8192);

   printf("Type, 1, 2, 4, 8\n");
   printf("static (16): 8192, ");
   omp_set_schedule(omp_sched_static, 16);
   run(8192);
   
   printf("static (32): 8192, ");
   omp_set_schedule(omp_sched_static, 32);
   run(8192);
   
   printf("dynamic (1): 8192, ");
   omp_set_schedule(omp_sched_dynamic, 1);
   run(8192);

   printf("dynamic (4): 8192,");
   omp_set_schedule(omp_sched_dynamic, 4);
   run(8192);
   
   free(matrix);
   free(vector);
   free(result);

   return 0;
}

void run(int n){

   double time = 0;
   double serial = 0;
   omp_set_num_threads(1);
   for (int i=0; i<RUNS; i++)
       serial += matrixMult(n);
   printf("%d, ", 1);
   omp_set_num_threads(2);
   for (int i=0; i<RUNS; i++)
       time = matrixMult(n);
   printf("%f, ", serial / time / RUNS);
   time = 0;
   omp_set_num_threads(4);
   for (int i=0; i<RUNS; i++)
       time = matrixMult(n);
   printf("%f, ", serial / time / RUNS);
   omp_set_num_threads(8);
   time = 0;
   for (int i=0; i<RUNS; i++)
       time = matrixMult(n);
   printf("%f, ", serial / time / RUNS);
   printf("\n");
}

float getRandFloat(){
   return (float) (rand()) / (float)(rand());
}

void initMatrix(int n){

   matrix = (float *) malloc(sizeof(float) * n*n);
   vector = (float *) malloc(sizeof(float) * n);
   result = (float *) malloc(sizeof(float) * n);

   for (int i = 0; i < n*n; i++){
      if (i<n)
         vector[i] = getRandFloat();

      matrix[i] = getRandFloat();
   }
}

double matrixMult(int n){

   double start;
   double end;
   double time;


   start = omp_get_wtime();
#pragma omp parallel for
   for (int i = 0; i < n; i++){
      for (int j=0; j < n; j++){
         result[i] += matrix[i*n+j] * vector[j];
         int d = 0;
         for (int k = 0; k<result[i];k++)
            d++;
      }
   }
   end = omp_get_wtime();
   time = end - start;

   return time;
}
