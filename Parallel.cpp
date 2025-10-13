#include <iostream>
#include <mpi.h>

using namespace std;

#include "Parallel.h"


int Initialize(int argc/*=-1*/, char **argv/*=NULL*/) {
	if (argc < 0)   MPI_Init(NULL, NULL);
  else            MPI_Init(&argc, &argv);
  return 0 ;
}



int Finalize() {
	
  MPI_Finalize() ;	

  return 0 ;
}


void Barrier() {
  MPI_Barrier(MPI_COMM_WORLD) ;
}

double Clock() {
  return MPI_Wtime() ;
}

void GetRank(int &ntasks, int &id) {
  MPI_Comm_rank(MPI_COMM_WORLD, &id) ;
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks) ;
}

