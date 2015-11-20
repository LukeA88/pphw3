#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

int c = 0;

int isprime(long long int n) {
  int i,squareroot;
  if ((n%6==1||n%6==5)&&n>10) {
	  for(i=3;i*i<=n;i+=2)
		  if(n%i==0)
			return 0;
	return 1;
  }
  else
    return 0;
}

int main(int argc, char *argv[])
{
  MPI_Init(&argc,&argv);
  int rank,size;
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  int pc;       /* prime counter */
  long long int foundone; /* most recent prime found */
  long long int n, limit;

  sscanf(argv[1],"%lld",&limit);	
  if(rank==0)
  printf("Starting. Numbers to be scanned= %lld\n",limit);

  pc=0;     /* Assume (2,3,5,7) are counted here */

  double time = MPI_Wtime();

  //for (n=11; n<=limit; n=n+2) {
  for (n=11 + rank*2 ; n<=limit; n=n+size*2) {
    if (isprime(n)) {
      pc++;
      foundone = n;
    }			
  }

  int total_pc = 0;
  long long int total_foundone = 0;
  MPI_Reduce( &pc , &total_pc , 1 , MPI_INT , MPI_SUM , 0 , MPI_COMM_WORLD );
  MPI_Reduce( &foundone , &total_foundone , 1 , MPI_LONG_LONG_INT , MPI_MAX , 0 , MPI_COMM_WORLD );
  MPI_Barrier(MPI_COMM_WORLD);

  if(rank==0)
  printf("Done. Largest prime is %lld Total primes %d\n",total_foundone,total_pc+4);

  //printf("Time = %lfs\n",MPI_Wtime()-time);
  MPI_Finalize();
  return 0;
} 
