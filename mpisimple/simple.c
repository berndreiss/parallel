#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
  int rank, tmp, result=0;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    tmp = 1234;
    MPI_Send((void *)&tmp, 1, MPI_INT, 1, 15, MPI_COMM_WORLD);
    MPI_Recv((void *)&tmp, 1, MPI_INT, 1, 16, MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);
   if (tmp != 1235)
      result = -3;
  } else { // rank == 1
  tmp = 0;
  MPI_Recv((void *)&tmp, 1, MPI_INT, 0, 15, MPI_COMM_WORLD,
  MPI_STATUS_IGNORE);
  tmp++;
  MPI_Send((void *)&tmp, 1, MPI_INT, 0, 16, MPI_COMM_WORLD);
  result = 0;
  }
  MPI_Finalize();
  printf("Process %d, result: %d\n", rank, tmp);
  return result;
}
