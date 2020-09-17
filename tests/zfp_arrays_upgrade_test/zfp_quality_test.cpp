#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <algorithm> 
#include <cstdlib>
#include <math.h>
#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>  
#include <sys/time.h>
#include <bitset>
#include "zfparray.h"
#include "zfparray1.h"
#include "zfparray2.h"
#include "zfparray3.h"

double calculatePSNR(double* true_data, zfp::array1<double>::pointer comp_data, int size){
	double max_diff = 0;
	double rmse_sum = 0;
	double max_val;
	double min_val;
    int count = 0;

    for (int i = 0; i < size; i++){
        double a = true_data[i];
		double b = comp_data[i];

        // RMSE Work
        double rmse_diff = a - b;
		rmse_diff = rmse_diff * rmse_diff;
		rmse_sum = rmse_sum + rmse_diff;

        // PSNR Work
        if(a > max_val || count == 0){
            max_val = a;
        }
        if(a < min_val || count == 0){
            min_val = a;
        }

        // Max Diff Work
        double diff = fabs(a - b);
        if(diff > max_diff){
            max_diff = diff;
        }

        count++;
    }

    // Calculate Root Mean Square Error
    double rmse = rmse_sum / (size - 1);
    rmse = sqrt(rmse);

    double psnr = 0;
    if (rmse == 0){
        psnr = 1000;
    } else {
        psnr = 20 * (log10(((max_val - min_val) / rmse)));
    }
    //std::cout << "Max - Min: " << (max_val - min_val) << std::endl;
	//std::cout << "PSNR: " << psnr << std::endl;
    //std::cout << "RMSE: " << rmse << std::endl;
    //std::cout << "Max Difference: " << max_diff << std::endl;
    return rmse;
}

int main(int argc, char *argv[]) {
    // Create zfp arrays using the different rate measurements
    int size = 10000;
    zfp::array1<double> zfp_64(size, 64.0);
    zfp::array1<double> zfp_48(size, 48.0);
    zfp::array1<double> zfp_32(size, 32.0);
    zfp::array1<double> zfp_16(size, 16.0);
    zfp::array1<double> zfp_8(size, 8.0);

    // Create equivalent double arrays
    double true_64 [size];
    double true_48 [size];
    double true_32 [size];
    double true_16 [size];
    double true_8 [size];

    // Use a random number generator to fill each of the arrays
    double num_max = 10;
    srand (static_cast <unsigned> (time(0)));
    for (int i = 0; i < size; i++){
        double tmp_64 = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/num_max));
        zfp_64[i] = tmp_64;
        true_64[i] = tmp_64;
        double tmp_48 = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/num_max));
        zfp_48[i] = tmp_48;
        true_48[i] = tmp_48;
        double tmp_32 = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/num_max));
        zfp_32[i] = tmp_32;
        true_32[i] = tmp_32;
        double tmp_16 = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/num_max));
        zfp_16[i] = tmp_16;
        true_16[i] = tmp_16;
        double tmp_8 = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/num_max));
        zfp_8[i] = tmp_8;
        true_8[i] = tmp_8;
    }
    // Flush the cache of all zfp arrays now
    zfp_64.flush_cache();
    zfp_48.flush_cache();
    zfp_32.flush_cache();
    zfp_16.flush_cache();
    zfp_8.flush_cache();

    // Calculate Initial PSNR for each of the arrays
    std::cout << "Initial 64: " << std::endl;
    calculatePSNR(true_64, &zfp_64[0], size);
    std::cout << "Initial 48: " << std::endl;
    calculatePSNR(true_48, &zfp_48[0], size);
    std::cout << "Initial 32: " << std::endl;
    calculatePSNR(true_32, &zfp_32[0], size);
    std::cout << "Initial 16: " << std::endl;
    calculatePSNR(true_16, &zfp_16[0], size);
    std::cout << "Initial 8: " << std::endl;
    calculatePSNR(true_8, &zfp_8[0], size);
    std::cout << std::endl;

    // Do arithmetic on each of the arrays in order to simulate experimental process
    double rando = 0;
    std::ofstream output_file("output.csv");
    output_file << "Cycle,RMSE,Rate\n";
    for (int i = 0; i < 10000; i++){
        if (i % 2 == 0){
            // Choose a new rando
            rando = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/num_max));
            // Array Computations
           for (int j = 0; j < size; j++){
                true_64[j] = true_64[j] / (true_48[j] + rando);
                zfp_64[j] = zfp_64[j] / (true_48[j] + rando);
                true_48[j] = true_48[j] / (true_32[j] + rando);
                zfp_48[j] = zfp_48[j] / (true_32[j] + rando);
                true_32[j] = true_32[j] / (true_16[j] + rando);
                zfp_32[j] = zfp_32[j] / (true_16[j] + rando);
                true_16[j] = true_16[j] / (true_8[j] + rando);
                zfp_16[j] = zfp_16[j] / (true_8[j] + rando);
                true_8[j] = true_8[j] / (true_64[j] + rando);
                zfp_8[j] = zfp_8[j] / (true_64[j] + rando);
            } 
        } else {
            // Array Computations
           for (int j = 0; j < size; j++){
                true_8[j] = true_8[j] * (true_64[j] + rando);
                zfp_8[j] = zfp_8[j] * (true_64[j] + rando);
                true_16[j] = true_16[j] * (true_8[j] + rando);
                zfp_16[j] = zfp_16[j] * (true_8[j] + rando);
                true_32[j] = true_32[j] * (true_16[j] + rando);
                zfp_32[j] = zfp_32[j] * (true_16[j] + rando);
                true_48[j] = true_48[j] * (true_32[j] + rando);
                zfp_48[j] = zfp_48[j] * (true_32[j] + rando);
                true_64[j] = true_64[j] * (true_48[j] + rando);
                zfp_64[j] = zfp_64[j] * (true_48[j] + rando);
            }  
        }
        // Flush the cache of all zfp arrays now
        zfp_64.flush_cache();
        zfp_48.flush_cache();
        zfp_32.flush_cache();
        zfp_16.flush_cache();
        zfp_8.flush_cache();

        // Check Data Quality After Some Number of Cycles
        if (i % 1 == 0){
            // Calculate Initial PSNR for each of the arrays
            std::cout << "Cycle: " << i << " 64: " << std::endl;
            double psnr = calculatePSNR(true_64, &zfp_64[0], size);
            output_file << i << "," << psnr << "," << 64 << "\n";
            //std::cout << "Cycle: " << i << " 48: " << std::endl;
            psnr = calculatePSNR(true_48, &zfp_48[0], size);
            output_file << i << "," << psnr << "," << 48 << "\n";
            //std::cout << "Cycle: " << i << " 32: " << std::endl;
            psnr = calculatePSNR(true_32, &zfp_32[0], size);
            output_file << i << "," << psnr << "," << 32 << "\n";
            //std::cout << "Cycle: " << i << " 16: " << std::endl;
            psnr = calculatePSNR(true_16, &zfp_16[0], size);
            output_file << i << "," << psnr << "," << 16 << "\n";
            //std::cout << "Cycle: " << i << " 8: " << std::endl;
            psnr = calculatePSNR(true_8, &zfp_8[0], size);
            output_file << i << "," << psnr << "," << 8 << "\n";
            std::cout << std::endl;
        }
    }
    output_file.close();

    return 0; 
}