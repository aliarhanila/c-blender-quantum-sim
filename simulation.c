#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Simulation definitions
#define Space_Frontier 5.0
#define Number_of_points 1000
#define Generated_dots 0
#define RAND_DOUBLE(min, max) (min + ((double)rand() / RAND_MAX) * (max - min))

/* Wave equation on hydrogen atom */
double psi_1s(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    return exp(-r);
}

/* Probability distribution according to Born's interpretation */
double bornM(double x, double y, double z) {
    double psi = psi_1s(x, y, z);
    double probability = psi * psi;
    return probability;
}

int main() {
    int target_dots = Number_of_points;
    int generated_dots = Generated_dots;
    double border = Space_Frontier;

    srand(time(NULL)); // Start random number generator

    // File Operations
    FILE *qfile = fopen("S1_orbitals.csv", "w");
    if (qfile == NULL) {
        printf("Error: File is not generated!\n");
        return 1;
    }

    // CSV Headers for Blender
    fprintf(qfile, "x,y,z\n");

    printf("Simulation Started %d dot calculating..\n", target_dots);

    while (generated_dots < target_dots) {
        double x = RAND_DOUBLE(-border, border);
        double y = RAND_DOUBLE(-border, border);
        double z = RAND_DOUBLE(-border, border);

        double test = RAND_DOUBLE(0.0, 1.0);

        if (test < bornM(x, y, z)) {
            fprintf(qfile, "%f,%f,%f\n", x, y, z);
            generated_dots++;
        }
    }

    fclose(qfile);
    printf("Simulation finished all datas saved in csv file\n");

    return 0;
}
