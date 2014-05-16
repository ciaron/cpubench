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

    int max_n = 1000000; /* n should never exceed this, to avoid running out of system memory */
    /* choose r and n such that overall execution time stays roughly constant */
    int n = 0; //18750000; // array size
    int r; // number of test repetitions
    int i, j; // loop counters
    int step = 1;
    double *a, *b, *c, *d; // arrays
    struct timespec start, stop; // start and stop timers (seconds)
    double accum;
    double mflops;

    //printf("n\taccum\tmflops\n"); 
    printf("n\tmflops\n"); 

    while (n < max_n) {

        if (n >= 100) step = 10;
        if (n >= 1000) step = 10;
        if (n >= 10000) step = 100;
        if (n >= 100000) step = 1000;
        if (n >= 300000) step = 10000;

        n += step;
        //r = 187500000 / n;
        r = 18750000 / n;

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

        /* start timer */
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

        /* stop timer */
        clock_gettime(CLOCK_REALTIME, &stop);

        accum = ( stop.tv_sec - start.tv_sec )
              + ( stop.tv_nsec - start.tv_nsec )
                / (double) 1e9;

        mflops = r * n * 2.0 / accum / 1e6;

        //printf("%d\t%f\t%f\n", n, accum, mflops); 
        printf("%d\t%f\n", n, mflops); 

        free(a); free(b); free(c); free(d);

    } // while

    return 0;
}
