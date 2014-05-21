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

double
do_triad(double *a, double *b, double *c, double *d, int n, int r) {
    
    // run the vector triad calculations.
    // Returns: elapsed time (seconds)

    int n2;
    int i, j;
    double elapsed;
    struct timespec start, stop;
    int max_n = 10000000; /* n should never exceed this, to avoid running out of system memory */

    n2 = n/2;

    /* start timer */
    clock_gettime(CLOCK_REALTIME, &start);

    if (n < max_n) {
        /* MAIN LOOP */
        for (j=0; j<r; j++) {
            for (i=0; i<n; i++) {
                a[i] = b[i] + c[i] * d[i];
            }
            /* prevent loop interchange by optimising compiler */
            if (a[n2] < 0) {
                dummy(a,b,c,d);
            }
        }
    } else {
        exit(3);
    }

    /* stop timer */
    clock_gettime(CLOCK_REALTIME, &stop);

    elapsed = ( stop.tv_sec - start.tv_sec )
            + ( stop.tv_nsec - start.tv_nsec )
            / (double) 1e9;

    return elapsed;
}

int 
main(int argc, char **argv) {

    /* choose r and n such that overall execution time stays roughly constant */
    int n; // array size
    int r = 0; // number of test repetitions
    int i;
    double *a, *b, *c, *d; // arrays
    double elapsed;
    double mflops;

    //printf("r\tn\telapsed\tmflops\n"); 
    //printf("n\tmflops\n"); 

    if (argc < 2) {
        printf("Enter a value for N (array length)\n");
        exit(2);
    } else {
        n = atoi(argv[1]);
    }

    /* malloc the data arrays */
    if (!(a = (double *)malloc(n * sizeof(double)))) { printf("malloc failed (a)\n"); return 1; }
    if (!(b = (double *)malloc(n * sizeof(double)))) { printf("malloc failed (b)\n"); return 1; }
    if (!(c = (double *)malloc(n * sizeof(double)))) { printf("malloc failed (c)\n"); return 1; }
    if (!(d = (double *)malloc(n * sizeof(double)))) { printf("malloc failed (d)\n"); return 1; }

    /* initialise b, c and d */
    for (i=0; i<n; i++) {
        b[i] = 1.0;
        c[i] = 2.0;
        d[i] = 3.0;
    }

    do_triad(a, b, c, d, n, r);

    while (1) {

        elapsed = do_triad(a, b, c, d, n, r);

        if (elapsed > 0.015) { 
            // enough work done
            break; 
        } else if (elapsed < 0.0000000001) {
            // not enough work done
            exit(4);
        } else {
            // go around again
            r += 10000;
        }

    } // while

    mflops = r * n * 2.0 / elapsed / 1e6;
    //printf("%d\t%d\t%f\t%f\n", r, n, elapsed, mflops); 
    printf("%d\t%f\n", n, mflops); 

    free(a); free(b); free(c); free(d);

    return 0;
}
