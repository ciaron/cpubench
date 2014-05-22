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
main(int argc, char **argv) {

    /* choose r and n such that overall execution time stays roughly constant */
    int i, j, n, n2;
    long int r = 1; // number of test repetitions
    double *a, *b, *c, *d; // arrays
    double elapsed, mflops;
    struct timespec start, stop;

    //printf("r\tn\telapsed\tmflops\n"); 
    //printf("n\tmflops\n"); 

    if (argc < 2) {
        printf("Enter a value for N (array length)\n"); exit(2);
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

    while (1) {

        n2 = n/2;

        /* start timer */
        clock_gettime(CLOCK_REALTIME, &start);

        /* MAIN LOOP */
        for (j=0; j<r; j++) {
            for (i=0; i<n; i++) {
                a[i] = b[i] + c[i] * d[i];
            }
            /* prevent loop interchange
             * Make sure this function is only available at link time
             */
            if (a[n2] < 0) {
                dummy(a,b,c,d);
            }
        }

        /* stop timer */
        clock_gettime(CLOCK_REALTIME, &stop);

        elapsed = ( stop.tv_sec - start.tv_sec )
                + ( stop.tv_nsec - start.tv_nsec )
                / (double) 1e9;

        //printf("ELAPSED:%.9f\n", elapsed); 

        if (elapsed > 0.2) { 
            // enough work done
            break; 
        } else {
            // go around again
            r = r*2;
        }

    } // while

    mflops = r * n * 2.0 / elapsed / 1e6;
    
    //printf("%d\t%d\t%f\t%f\n", r, n, elapsed, mflops); 
    printf("%d\t%f\n", n, mflops); 

    free(a); free(b); free(c); free(d);

    return 0;
}
