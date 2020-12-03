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
    zvector.resize(length_1, true); //, true);

    // Set a couple of items to see if they persist 
    zvector[10] = 3.1415926;                        
    zvector.flush_cache();
    zvector[11] = 128.123;
    zvector.flush_cache();
    //QUESTION: Why do I need to flush cache after each? shouldn't this be done automatically?

    // Print out values of array
    std::cout << length_1 << " Resized Vector: " << std::endl;
    for (int i = 0; i < length_1; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array to 20 items
    int length_2 = 12;
    zvector.resize(length_2, true); //, true);

    // Print out values of array
    std::cout << length_2 << " Resized Vector: " << std::endl;
    for (int i = 0; i < length_2; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array to 10 items
    int length_3 = 25;
    zvector.resize(length_3, true); //, true);

    // Print out values of array
    std::cout << length_3 << " Resized Vector: " << std::endl;
    for (int i = 0; i < length_3; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array to 25 items
    int length_4 = 20;
    zvector.resize(length_4, true); //, true);

    // Print out values of array
    std::cout << length_4 << " Resized Vector: " << std::endl;
    for (int i = 0; i < length_4; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    std::cout << "Test Completed" << std::endl;
}
