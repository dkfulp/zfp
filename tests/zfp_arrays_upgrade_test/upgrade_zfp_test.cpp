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

    // Initialize Initial Vector
    int length = 10;
    double rate = 48.0;
    zfp::array1<double> zvector(length, rate);



    // Fill vector with numbers
    std::cout << "1st Initialized Vector: " << std::endl;
    for (int i = 0; i < length; i++){
        zvector[i] = i;
        zvector.flush_cache();
        std::cout << zvector[i] << " " << std::endl;
    }
    // Resize array once to 15 items 
    length = 15;
    zvector.resize(length, true, true);
    // Print out values of array
    std::cout << "1st Resize Vector: " << std::endl;
    for (int i = 0; i < length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }



    // Fill vector with numbers
    std::cout << "2nd Initialized Vector: " << std::endl;
    for (int i = 0; i < length; i++){
        zvector[i] = i;
        zvector.flush_cache();
        std::cout << zvector[i] << " " << std::endl;
    }
    // Resize array once to 12 items
    length = 12;
    zvector.resize(length, true, true);
    // Print out values of array
    std::cout << "1st Resize Vector: " << std::endl;
    for (int i = 0; i < length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }
    // Resize array second to 20 items // If this goes too high then it breaks for some reason????
    length = 15;
    zvector.resize(length, true, true);
    // Print out values of array
    std::cout << "2nd Resize Vector: " << std::endl;
    for (int i = 0; i < length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }



    // Fill vector with numbers
    std::cout << "3rd Initialized Vector: " << std::endl;
    for (int i = 0; i < length; i++){
        zvector[i] = i;
        zvector.flush_cache();
        std::cout << zvector[i] << " " << std::endl;
    }
    // Resize array once to 18 items
    length = 18;
    zvector.resize(length, true, true);
    // Print out values of array
    std::cout << "1st Resize Vector: " << std::endl;
    for (int i = 0; i < length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }
    // Resize array second to 25 items
    length = 25;
    zvector.resize(length, true, true);
    // Print out values of array
    std::cout << "2nd Resize Vector: " << std::endl;
    for (int i = 0; i < length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }
    // Resize array third to 30 items
    length = 30;
    zvector.resize(length, true, true);
    // Print out values of array
    std::cout << "3rd Resize Vector: " << std::endl;
    for (int i = 0; i < length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    std::cout << "Test Completed" << std::endl;
}
