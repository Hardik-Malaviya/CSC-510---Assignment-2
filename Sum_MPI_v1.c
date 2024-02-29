#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <sys/time.h>
#include "timer.h"

//Calculating Pi values:
double calculatePi(int n, int rank, int size) 
{
    double sum = 0.0;
    int i;

    for (i = rank; i <=n; i += size) 
    {
        double term = pow(-1.0, i) / (2.0 * i + 1.0);
        sum += term;
    }
    
    return sum;
}

//Main function: 
int main(int argc, char *argv[]) 
{
    int rank, size, n, src;
    double t, t1, t2;
    struct timeval start, end;
    
    //MPI intialising: 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) 
    {
        printf("Enter the number of n                          : ");     //input n from user
        fflush(stdout);
        scanf("%d", &n);
    }
    
    //Brodcast to other processors:
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    //function for calculating time of parallel runtime:
    MPI_Barrier(MPI_COMM_WORLD);             // Synchronize processes before starting the timer
    gettimeofday(&start, NULL);

    double localPi = calculatePi(n, rank, size);
    double globalPi;

    if (rank == 0) 
    {
        globalPi = localPi; 
  
        for (src = 1; src < size; src++) 
        {
            MPI_Recv(&localPi, 1, MPI_DOUBLE, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            globalPi += localPi;
        }
    } 
    else 
    {
        MPI_Send(&localPi, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
        
    MPI_Barrier(MPI_COMM_WORLD);   // Synchronize processes before starting the parallel timer
    gettimeofday(&end, NULL);
        
    t1 = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0;  // Parallel runtime

  
    if (rank == 0) 
    {

        gettimeofday(&start, NULL);

        double Serial_Pi = calculatePi(n, 0, 1);
  
        gettimeofday(&end, NULL);
       

        double Serial_runtime = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0; //Serial runtime
        
        //print out all the performance values:

        printf("\nValue of Pi (MPI point- to-point communication): %.6f\n",4 * globalPi);        
        printf("Serial Runtime (Ts)                            : %.6f\n" , Serial_runtime);
        printf("Parallel Runtime (Tp)                          : %.6f\n",t1);
        printf("Speedup (S)                                    : %.6f \n" , (Serial_runtime/t1));
        printf("Efficiency (E)                                 : %.6f \n" , (Serial_runtime/t1 /size));

         }

     

    MPI_Finalize();

    return 0;
}
