#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

/*
C version based on Fortran example in "Introduction to High Performance
Computing for Scientists and Engineers" by Georg Hager and Gerhard Wellein, CRC
Press.

Chapter 1
*/

int 
dummy(double *a, double *b, double *c, double *d) {
    return 0;
}

int 
main(int argc, char **argv) {

    /* choose r and n such that overall execution time stays roughly constant */
    int n = 175000000; // array size
    int r = 5; // number of test repetitions
    int i, j; // loop counters
    double *a, *b, *c, *d; // arrays
    struct timespec start, stop; // start and stop timers (seconds)
    double accum;
    double mflops;

    /* malloc the data arrays */
    a = (double *)malloc(n * sizeof(double));
    b = (double *)malloc(n * sizeof(double));
    c = (double *)malloc(n * sizeof(double));
    d = (double *)malloc(n * sizeof(double));

    /* initialise b, c and d */
    for (i=0; i<n; i++) {
        b[i] = 1.0;
        c[i] = 2.0;
        d[i] = 3.0;
    }

    clock_gettime(CLOCK_REALTIME, &start);

    /* MAIN LOOP */
    for (j=0; j<r; j++) {
        for (i=0; i<n; i++) {
            a[i] = b[i] + c[i] * d[i];
        }
        /* prevent loop interchange by optimising compiler */
        if (a[2] < 0) {
            dummy(a,b,c,d);
        }
    }

    clock_gettime(CLOCK_REALTIME, &stop);

    accum = ( stop.tv_sec - start.tv_sec )
          + ( stop.tv_nsec - start.tv_nsec )
            / (double) 1e9;
    mflops = r * n * 2.0 / accum / 1e6;

    printf("Elapsed time (seconds): %f\n", accum);
    printf("Rate (MFLOPS/second): %f\n", mflops);

    free(a); free(b); free(c); free(d);

    return 0;
}
