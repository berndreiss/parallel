#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ITER 10000
#define WARMUP 10

int main(int argc, char **argv) {
  int rank, size;
  int max = 1<<22;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char *sendbuf = (char *) malloc(max);
  char *recvbuf = (char *) malloc(max);

  double t1 = 0, t2;

  for (long long size=1; size <= max; size *=2){
     int iter = MAX_ITER;
     if(size > (1<<16))
        iter = 1000;

     for (int i = 0; i < iter+WARMUP; i++){
        if (i == WARMUP-1)
           t1 = MPI_Wtime();
        if (rank == 0){
           MPI_Send(sendbuf, size, MPI_CHAR, 1, 0xa, MPI_COMM_WORLD);
           MPI_Recv(recvbuf, size, MPI_CHAR, 1, 0xb, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank == 1){
           MPI_Recv(recvbuf, size, MPI_CHAR, 0, 0xa, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           MPI_Send(sendbuf, size, MPI_CHAR, 0, 0xb, MPI_COMM_WORLD);
        }
     }
     t2 = MPI_Wtime();
     if (rank == 0)
         printf("%lld, %.3f\n", size, (t2-t1)/iter*1e6);
  }


  free(sendbuf);
  free(recvbuf);

  MPI_Finalize();

  return 0;
}
