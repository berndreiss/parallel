#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define TIMER 50

int randomDec(int current);

int main(int argc, char **argv) {
  int size, rank, bombed=0;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int buffer = TIMER;
  
  int currentThread = 0;


  while(!bombed){
     if (rank == currentThread){
        MPI_Ssend(&buffer, 1, MPI_INTEGER, (currentThread + 1) % size, 0xa, MPI_COMM_WORLD);
     }
     if (rank == (currentThread + 1) % size){
        MPI_Recv(&buffer, 1, MPI_INTEGER, currentThread, 0xa, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        buffer = randomDec(buffer);
        if (buffer > 0)
        printf("Process %d received the bomb, still counting: %d\n", rank, buffer);
        else {
           bombed = 1;
           buffer = rank;
        }
     }
     currentThread = (currentThread + 1) % size;
     MPI_Bcast(&bombed, 1, MPI_INTEGER, currentThread, MPI_COMM_WORLD);
     MPI_Bcast(&buffer, 1, MPI_INTEGER, currentThread, MPI_COMM_WORLD);
     if (bombed && rank != currentThread)
        printf("This is Process %d, and the loser is %d\n", rank, buffer);

  }
  MPI_Finalize();
  return 0;
}

int randomDec(int current){
   srand(time(NULL));
   int retVal = current - random() % 10;
   return (retVal > 0) ? retVal : 0;
}
