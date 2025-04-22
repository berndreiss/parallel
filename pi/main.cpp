#include <cstdio>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXN 2147483647
#define RUNS 2

int MODE = 1;

void runTest(long n);

int main(int argc, char *argv[]) {
    

    printf("N, serial, critical(2), atomic(2), reduction(2), critical(4), atomic(4), reduction(4), critical(8), atomic(8), reduction(8), critical(16), atomic(16), reduction(16), \n");
    runTest(1 << 20);
    runTest(1 << 21);
    runTest(1 << 23);
    runTest(1 << 24);
    runTest(1 << 26);

    MODE = 0;

    printf("N, serial, critical(2), atomic(2), reduction(2), critical(4), atomic(4), reduction(4), critical(8), atomic(8), reduction(8), critical(16), atomic(16), reduction(16), \n");
    runTest(1 << 20);
    runTest(1 << 21);
    runTest(1 << 23);
    runTest(1 << 24);
    runTest(1 << 26);

    return 0;
}
double f(double x) { return 4.0 / (1 + x * x); }

double CalcPiSerial(int n) {
    if (n <= 0 || n > MAXN) {
        return 0.0;
    }
    const double fH = 1.0 / (double)n;

    double fSum = 0.0;
    double fX;

    for (int i = 0; i < n; i += 1) {
        fX = fH * ((double)i + 0.5);
        fSum += f(fX);
    }
    return fH * fSum;
}

double CalcPiParallelCritical(int n, int threads) {
    if (n <= 0 || n > MAXN) {
        return 0.0;
    }
    const double fH = 1.0 / (double)n;

    double fSum = 0.0;
    double fX;

    omp_set_num_threads(threads);
#pragma omp parallel for 
    for (int i = 0; i < n; i += 1) {
        fX = fH * ((double)i + 0.5);
#pragma omp critical
        fSum += f(fX);
    }
    return fH * fSum;
}
double CalcPiParallelAtomic(int n, int threads) {
    if (n <= 0 || n > MAXN) {
        return 0.0;
    }
    const double fH = 1.0 / (double)n;

    double fSum = 0.0;
    double fX;

    omp_set_num_threads(threads);
#pragma omp parallel for 
    for (int i = 0; i < n; i += 1) {
        fX = fH * ((double)i + 0.5);
#pragma omp atomic
        fSum += f(fX);
    }
    return fH * fSum;
}
double CalcPiParallelReduction(int n, int threads) {
    if (n <= 0 || n > MAXN) {
        return 0.0;
    }
    const double fH = 1.0 / (double)n;

    double fSum = 0.0;
    double fX;

    omp_set_num_threads(threads);
#pragma omp parallel for reduction(+:fSum)
    for (int i = 0; i < n; i += 1) {
        fX = fH * ((double)i + 0.5);
        fSum += f(fX);
    }
    return fH * fSum;
}

void runTest(long n){
    double pi;
    double start, end, serial, parallel;

    printf("%ld, ", n);

    start = omp_get_wtime();
    for (int i = 0; i < RUNS; i++)
        pi = CalcPiSerial(n);
    end = omp_get_wtime();

    serial = end-start;
    //printf("Serial: %ld\n", serial);
    if (MODE)
        printf("%.6f, ", serial);
    else
       printf("%d, ", 1);

    for (int t = 2; t <= 16; t *=2){
        start = omp_get_wtime();
        for (int i = 0; i < RUNS; i++)
            pi = CalcPiParallelCritical(n, t);
        end = omp_get_wtime();
        parallel = end - start;

        if (MODE)
            printf("%.6f, ", parallel);
        else
           printf("%.6f, ", serial / parallel);
        //
        //printf("Parallel (critical, %d): %ld\n", t, end-start);
        
        start = omp_get_wtime();
        for (int i = 0; i < RUNS; i++)
            pi = CalcPiParallelAtomic(n, t);
        end = omp_get_wtime();
        parallel = end - start;

        if (MODE)
            printf("%.6f, ", parallel);
        else
           printf("%.6f, ", serial / parallel);

        //printf("Parallel (atomic, %d): %ld\n", t, end-start);

        start = omp_get_wtime();
        for (int i = 0; i < RUNS; i++)
            pi = CalcPiParallelReduction(n, t);
        end = omp_get_wtime();
        parallel = end - start;

        if (MODE)
            printf("%.6f, ", parallel);
        else
           printf("%.6f, ", serial / parallel);
        //
        //printf("Parallel (reduction, %d): %ld\n", t, end-start);

    }
    //printf("Pi is %f \n", pi);
    printf("\n");
}

