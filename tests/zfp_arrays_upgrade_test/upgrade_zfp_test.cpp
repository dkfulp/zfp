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

int main(){
    // Testing new Resize Function
    std::cout << "Test Started" << std::endl;

    int length = 10;
    double rate = 48.0;
    zfp::array1<double> zvector(length, rate);

    // Fill vector with numbers
    std::cout << "Initializing Vector: " << std::endl;
    for (int i = 0; i < length; i++){
        zvector[i] = i;
        zvector.flush_cache();
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array to 15 items
    int new_length = 15;
    zvector.resize(new_length, false, true);

    // Print out values of array
    std::cout << new_length << " Resized Vector: " << std::endl;
    for (int i = 0; i < new_length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array to 25 items
    int newer_length = 25;
    zvector.resize(newer_length, false, true);

    // Print out values of array
    std::cout << newer_length << " Resized Vector: " << std::endl;
    for (int i = 0; i < newer_length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array to 10 items
    int newest_length = 10;
    zvector.resize(newest_length, false, true);

    // Print out values of array
    std::cout << newest_length << " Resized Vector: " << std::endl;
    for (int i = 0; i < newest_length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    std::cout << "Test Completed" << std::endl;
}
