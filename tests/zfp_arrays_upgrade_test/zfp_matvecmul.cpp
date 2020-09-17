#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <algorithm> 
#include <cstdlib>
#include <math.h>
#include <iostream>
#include <random>
#include <iomanip>  
#include <sys/time.h>
#include "zfparray1.h"
#include "zfparray2.h"
#include "zfparray3.h"


void matvecmul(zfp::array2<double> zmatrix, zfp::array1<double> zvector, zfp::array1<double>& zresult, int M, int N){
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            zresult[i] += (zmatrix[i * N + j] * zvector[j]);
            zresult.flush_cache();
        }
        std::cout << std::setprecision(precision) << zresult[i] << " " << std::endl;
    }
}

int main(){
    int M = 5;
    int N = 3;
    int precision = 10;
    double min = -1000000;
    double max = 1000000;
    //srand((unsigned)time(NULL));
    srand(0);
    struct timeval start;
    struct timeval stop;
    double standard_time_taken;
    double zfp_time_taken;

    // initialize matrix and vector
    double** matrix = new double*[M];
    for (int i = 0; i < M; i++){
        matrix[i] = new double[N];
    }
    std::cout << "Matrix: " << std::endl;
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            matrix[i][j] = (max - min) * ((double)rand() / (double)RAND_MAX) + min;
            std::cout << std::setprecision(precision) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Vector: " << std::endl;
    double* vector = new double[N];
    for (int j = 0; j < N; j++){
        vector[j] = (max - min) * ((double)rand() / (double)RAND_MAX) + min;
        std::cout << std::setprecision(precision) << vector[j] << " " << std::endl;
    }

    // Do multiplication without zfp arrays
    std::cout << "Standard Results: " << std::endl;
    double* result = new double[M];
    gettimeofday(&start, NULL);
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            result[i] += (matrix[i][j] * vector[j]);
        }
        std::cout << std::setprecision(precision) << result[i] << " " << std::endl;
    }
    gettimeofday(&stop, NULL);
    standard_time_taken = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);

    // Place data into zfp arrays
    double rate = 48.0;
    std::cout << "ZFP Matrix: " << std::endl;
    zfp::array2<double> zmatrix(M,N, rate);
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            zmatrix[i * N + j] = matrix[i][j];
            zmatrix.flush_cache();
            std::cout << std::setprecision(precision) << zmatrix[i * N + j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "ZFP Vector: " << std::endl;
    zfp::array1<double> zvector(N, rate);
    for (int j = 0; j < N; j++){
        zvector[j] = vector[j];
        zvector.flush_cache();
        std::cout << std::setprecision(precision) << zvector[j] << " " << std::endl;
    }

    // Do multiplication with zfp arrays
    std::cout << "ZFP Results: " << std::endl;
    zfp::array1<double> zresult(M, rate);
    gettimeofday(&start, NULL);
    matvecmul(zmatrix, zvector, &zresult, M, N);
    //for (int i = 0; i < M; i++){
    //    for (int j = 0; j < N; j++){
    //        zresult[i] += (zmatrix[i * N + j] * zvector[j]);
    //        zresult.flush_cache();
    //    }
    //    std::cout << std::setprecision(precision) << zresult[i] << " " << std::endl;
    //}
    gettimeofday(&stop, NULL);
    zfp_time_taken = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);

    // Compare differences RMSE PSNR etc
    double max_diff = 0;
    double rmse_sum = 0;
    double max_val = -1;
    double min_val = -1;

    for (int i = 0; i < M; i++){
        double a = result[i];
        double b = zresult[i];

        // RMSE Work
        double rmse_diff = a - b;
        rmse_diff = rmse_diff * rmse_diff;
        rmse_sum = rmse_sum + rmse_diff;

        // PSNR Work
        if(a > max_val || max_val == -1){
            max_val = a;
        }
        if(a < min_val || min_val == -1){
            min_val = a;
        }

        double diff = fabs(a - b);
        if(diff > max_diff){
            max_diff = diff;
        }
    }

    // Calculate Root Mean Square Error
    double rmse = rmse_sum / (M - 1);
    rmse = sqrt(rmse);

    // Calculate PSNR 
    double psnr = 20 * log10((max_val - min_val) / rmse);

    // Print Metrics
    std::cout << "Standard Time Take: " << standard_time_taken << std::endl;
    std::cout << "ZFP Time Take: " << zfp_time_taken << std::endl;
    std::cout << "Maximum Absolute Difference: " << max_diff << std::endl;
    std::cout << "Root Mean Squared Error: " << rmse << std::endl;
    std::cout << "PSNR: " << psnr << std::endl;
}
