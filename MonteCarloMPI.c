#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <mpi.h>

double estimate_pi(long total_drops, double needle_length, double line_spacing) {
    long crosses = 0;

    for (long i = 0; i < total_drops; i++) {
        double x = (double)rand() / RAND_MAX;  // Posición del extremo de la aguja [0, 1]
        double angle = (double)rand() / RAND_MAX * M_PI;  // Ángulo entre la aguja y las líneas [0, π/2]

        if (x <= needle_length * 0.5 * cos(angle)) {
            crosses++;
        }
    }

    // Calcular la probabilidad de cruce
    double probability = (double)crosses / total_drops;

    // Estimar π
    double pi_estimate = ((2 * needle_length) / (probability * line_spacing)) / 2;
    return crosses;
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);

    srand((unsigned)time(NULL));  // Inicializar generador de números aleatorios

    struct timespec start, end;
    double elapsed_time;
    long total_drops = atoi(argv[1]);  // Número total de lanzamientos de la aguja
    double needle_length = 1.0;  // Longitud de la aguja
    double line_spacing = 2.0;   // Espaciado entre las líneas
    int verbose = 0;
    double crosses = 0;

    if (argc > 2 && strcmp(argv[2], "-v") == 0) {//comando verbose
        verbose = 1;
    }

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    clock_gettime(CLOCK_MONOTONIC, &start); //Inicia Captura del tiempo

    double local_total_drops = total_drops / size;

    double local_crosses = 0;

    local_crosses = estimate_pi(local_total_drops, needle_length, line_spacing);
    MPI_Reduce(&local_crosses, &crosses, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    if (rank == 0) {
        double probability = (double)crosses / total_drops;
        double global_aproxPi = ((2 * needle_length) / (probability * line_spacing)) / 2;

        if (verbose) {
            printf("Pi approx: %lf\n", global_aproxPi);
        }

        printf("%f\n",elapsed_time);
    }

    
    MPI_Finalize();

    return 0;
}