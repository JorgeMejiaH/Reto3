#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

double estimatePi(double total_throws, double hits, const double factor){
    //Throw thosses
    for(int k=hits=0; k<total_throws; k++){
        //Find two random numbers whithin 0..1
        double x_axis = rand() * factor;
        double y_axis = rand() * factor;
        if(x_axis*x_axis + y_axis*y_axis < 1){ //Inside de Circle
            hits++;
        }
    }
    double Pi_aprox = 4.0 * hits / total_throws;
    return hits;
}

int main(int argc, char *argv[]){

    MPI_Init(&argc, &argv);

    struct timespec start, end;
    double elapsed_time;
    double k, total_throws, hits;
    const double factor = 1.0 / RAND_MAX;
    total_throws = atoi(argv[1]);
    int verbose = 0;

    if (argc > 2 && strcmp(argv[2], "-v") == 0) {//comando verbose
        verbose = 1;
    }

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    clock_gettime(CLOCK_MONOTONIC, &start); //Inicia Captura del tiempo

    //initialize random generator
    srand((unsigned)time(NULL));

    double local_total_throws = total_throws / size;


    double local_hits = 0;
    local_hits = estimatePi(local_total_throws, local_hits, factor);

    MPI_Reduce(&local_hits, &hits, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    if (rank == 0) {
        double global_aproxPi = 4.0 * hits / total_throws;

        if (verbose) {
            printf("Pi approx: %lf\n", global_aproxPi);
        }

        printf(elapsed_time);
    }

    
    MPI_Finalize();
    return 0;
}