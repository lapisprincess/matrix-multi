#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

unsigned int mode;
unsigned int size, num_threads;
double **A, **B, **SEQ_MATRIX, **PAR_MATRIX;

int main(int argc, char *argv[]) {

    if ((argc == 3) && (strcmp(argv[1], "S") == 0)) {
        mode = 0;
        size = atoi(argv[2]);
        printf("========\n");
        printf("mode: sequential\n");
        printf("thread count: 1\n");
        printf("size: %d\n", size);
        printf("========\n");
    }

    else if ((argc == 4) && (strcmp(argv[1], "P") == 0)) {
        mode = 1;
        num_threads = atoi(argv[2]);
        size = atoi(argv[3]);
        printf("========\n");
        printf("mode: parallel\n");
        printf("thread count: %d\n", num_threads);
        printf("size: %d\n", size);
        printf("========\n");
    }

    else {
        printf("usages:\n");
        printf("mmm S <size>\n");
        printf("mmm P <threads> <size>\n");
        return 1;
    }

    // initialize my matrices
    mmm_init();

    // timer start
    double clockstart, clockseq, clockpar;
    clockstart = rtclock();

    // sequential matrix multip
    mmm_seq();
    clockseq = rtclock();
    printf("Sequential Time: %.6f sec\n", (clockseq - clockstart));

    // parallel matrix multip
    if (mode == 1) {
        // can only have as many threads as there are rows
        if (num_threads > size)
            num_threads = size;

        // establish arguments for threads
        size_t arg_size = num_threads * sizeof(thread_args);
        thread_args *args = (thread_args*) malloc(arg_size);
        int row = 0;
        for (int i = 0; i < num_threads; i++) {
            args[i].tid = i + 1;
            args[i].start_row = row;
            row = row + (size / num_threads);
            if (i == num_threads - 1) args[i].end_row = size;
            else args[i].end_row = row;
        }

        // create threads
        pthread_t *threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
        for (int i = 0; i < num_threads; i++) {
            pthread_create(&threads[i], NULL, mmm_par, &args[i]);
        }

        // join threads back up
        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }

        // manage timer
        clockpar = rtclock();
        printf("Parallel Time: %.6f sec\n", (clockpar - clockseq));
        printf("Speedup: %.6f sec\n", (clockseq - clockstart) / (clockpar - clockseq));
        printf("Largest error: %.6f\n", mmm_verify());

        // free threads etc
        free(threads);
        threads = NULL;
        free(args);
        args = NULL;
    }

    // freedom!!!!!
    mmm_freeup();

    // virtual mic drop
    return 0;
}
