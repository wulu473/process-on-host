
#include <mpi.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HOSTNAME_LENGTH 1024

int main(int argc,char* argv[])
{
  // Set up MPI
  int rank, size;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int root = 0;

  // Get hostname 
  char hostname[MAX_HOSTNAME_LENGTH];
  hostname[MAX_HOSTNAME_LENGTH-1] = '\0';
  gethostname(hostname,MAX_HOSTNAME_LENGTH-1);

  // Allocate arrays for root
  int *ranks;
  char *hostnames;
  if(rank==root)
  {
    ranks = (int*) malloc(size*sizeof(int));
    hostnames = (char*) malloc(size*MAX_HOSTNAME_LENGTH*sizeof(char));
  }

  // Send ranks to root
  MPI_Gather(&rank,1,MPI_INT,ranks,1,MPI_INT,root,MPI_COMM_WORLD);

  // Send hostnames to root
  MPI_Gather(hostname,MAX_HOSTNAME_LENGTH,MPI_CHAR,hostnames,MAX_HOSTNAME_LENGTH,MPI_CHAR,root,MPI_COMM_WORLD);

  // Output
  if(rank==root)
  {
    for(unsigned int i=0;i<size;i++)
    {
      printf("Process %d of %d on %s\n",ranks[i],size,hostnames+MAX_HOSTNAME_LENGTH*i);
    }
  }


  // Tidy up
  if(rank==root)
  {
    free(ranks);
    free(hostnames);
  }
  MPI_Finalize();
  return 0;
}
