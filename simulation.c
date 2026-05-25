#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Simulation boundary and point definitions
#define Space_Frontier 30.0  // Expanded border for larger orbitals like 3d and 4f
#define Number_of_points 1000000 // Set to 1M for Blender volumetric rendering
#define Generated_dots 0
#define RAND_DOUBLE(min, max) (min + ((double)rand() / RAND_MAX) * (max - min))

/* =========================================
   S ORBITALS (Spherical)
   ========================================= */

// 1s orbital: Standard sphere
double psi_1s(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    return exp(-r);
}

// 2s orbital: Sphere with one radial node (hollow layer)
double psi_2s(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    return 0.1 * (2.0 - r) * exp(-r / 2.0);
}

/* =========================================
   P ORBITALS (Dumbbell shape)
   ========================================= */

// 2p_z orbital: Dumbbell along the Z-axis
double psi_2pz(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    return 0.1 * z * exp(-r / 2.0); 
}

// 2p_x orbital: Dumbbell along the X-axis
double psi_2px(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    return 0.1 * x * exp(-r / 2.0);
}

/* =========================================
   D ORBITALS (Clovers and Torus)
   ========================================= */

// 3d_xy orbital: Four-leaf clover between X and Y axes
double psi_3dxy(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    return 0.5 * (x * y) * exp(-r / 3.0);
}

// 3d_z2 orbital: Dumbbell on Z-axis with a torus on X-Y plane
double psi_3dz2(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    return 0.03 * (3.0 * z * z - r * r) * exp(-r / 3.0);
}

/* =========================================
   F ORBITALS (Complex multi-lobed structures)
   ========================================= */

// 4f_xyz orbital: Eight-lobed structure located in the octants
double psi_4f_xyz(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    // 0.01 is a safety multiplier to prevent infinite Monte Carlo loops
    return 0.01 * (x * y * z) * exp(-r / 4.0);
}

// 4f_z3 orbital: Multi-lobed dumbbell-like structure on Z-axis with rings
double psi_4f_z3(double x, double y, double z) {
    double r = sqrt(x*x + y*y + z*z);
    return 0.005 * z * (5.0 * z * z - 3.0 * r * r) * exp(-r / 4.0);
}


/* =========================================
   PROBABILITY DISTRIBUTION (Born's Interpretation)
   ========================================= */

double bornM(double x, double y, double z) {
    // Change the function below to render a different orbital.
    // Example: psi_1s, psi_2pz, psi_3dxy, psi_4f_xyz
    
    double psi = psi_4f_xyz(x, y, z); // Currently set to 4f_xyz
    
    double probability = psi * psi;
    return probability;
}

/* =========================================
   MAIN SIMULATION LOOP
   ========================================= */

int main() {
    int target_dots = Number_of_points;
    int generated_dots = Generated_dots;
    double border = Space_Frontier;

    srand(time(NULL)); // Start random number generator

    // File Operations
    FILE *qfile = fopen("orbitals.csv", "w");
    if (qfile == NULL) {
        printf("Error: File is not generated!\n");
        return 1;
    }

    // CSV Headers for Blender
    fprintf(qfile, "x,y,z\n");

    printf("Simulation Started. %d dots calculating...\n", target_dots);

    // Monte Carlo Rejection Sampling Loop
    while (generated_dots < target_dots) {
        
        double x = RAND_DOUBLE(-border, border);
        double y = RAND_DOUBLE(-border, border);
        double z = RAND_DOUBLE(-border, border);

        double test = RAND_DOUBLE(0.0, 1.0);

        // If the random test value is within the probability density, accept the point
        if (test < bornM(x, y, z)) {
            fprintf(qfile, "%f,%f,%f\n", x, y, z);
            generated_dots++;
        }
    }

    fclose(qfile);
    printf("Simulation finished. All data saved successfully in the CSV file.\n");

    return 0;
}
