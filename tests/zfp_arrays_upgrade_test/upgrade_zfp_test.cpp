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

    int length_0 = 10;
    double rate = 48.0;
    zfp::array1<double> zvector(length_0, rate);

    // Fill vector with numbers
    std::cout << "Initializing Vector: " << std::endl;
    for (int i = 0; i < length_0; i++){
        zvector[i] = i;
        zvector.flush_cache();
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array to 15 items
    int length_1 = 15;
    zvector.resize(length_1, false, true);

    // Set a couple of items to see if they persist
    zvector[11] = 3.1415926;
    zvector[12] = 128.123;
    zvector.flush_cache();
    // Print out values of array
    std::cout << length_1 << " Resized Vector: " << std::endl;
    for (int i = 0; i < length_1; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array to 20 items
    int length_2 = 20;
    zvector.resize(length_2, false, true);

    // Print out values of array
    std::cout << length_2 << " Resized Vector: " << std::endl;
    for (int i = 0; i < length_2; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array to 10 items
    int length_3 = 15;
    zvector.resize(length_3, false, true);

    // Print out values of array
    std::cout << length_3 << " Resized Vector: " << std::endl;
    for (int i = 0; i < length_3; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array to 25 items
    int length_4 = 25;
    zvector.resize(length_4, false, true);

    // Print out values of array
    std::cout << length_4 << " Resized Vector: " << std::endl;
    for (int i = 0; i < length_4; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    std::cout << "Test Completed" << std::endl;
}
