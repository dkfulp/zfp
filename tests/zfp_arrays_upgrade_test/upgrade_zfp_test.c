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
    int length = 6;
    double rate = 48.0;
    zfp::array1<double> zvector(length, rate);

    // Fill vector with numbers
    for (int i = 0; i < length; i++){
        zvector[j] = i;
        zvector.flush_cache();
    }

    // Resize zfp array with save command
    int new_length = 10;
    zvector.resize(new_length, save=true);

    // Print out values of array
    for (int i = 0; i < new_length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }

    // Resize zfp array without save command
    int newer_length = 15;
    zvector.resize(newer_length, clear=true);

    // Print out values of array
    for (int i = 0; i < newer_length; i++){
        std::cout << zvector[i] << " " << std::endl;
    }
}
