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
#include <bitset>
#include "mpi.h"
#include "zfparray.h"
#include "zfparray1.h"
#include "zfparray2.h"
#include "zfparray3.h"



struct ZFPStruct{
    int a;
    int b;
    int c;
    zfp::array1<double> region_state;
};


int main(int argc, char *argv[])
{
    // Create a zfp array
    zfp::array1<double> temp(9, 64.0);
    temp[0] = 1234.211343;
    temp[1] = 122.4532;
    temp[2] = 464.11;
    temp[3] = 95904.34065;
    temp[4] = 1234.211343;
    temp[5] = 122.4532;
    temp[6] = 464.11;
    temp[7] = 95904.34065;
    temp[8] = 1234.211343;
    temp.flush_cache();
    std::cout << temp.compressed_size() << std::endl;
    std::cout << sizeof(temp) << std::endl;

    // // Get the array header and compressed data
    // const zfp::array::header head = temp.get_header();
    // uchar* ptr = temp.compressed_data();

    // // Iterate through the data and print the bits
    // for (int i = 0; i < temp.compressed_size(); i++){
    //     std::bitset<8> x(ptr[i]);
    //     std::cout << x << " ";
    // }
    // std::cout << std::endl;

    // Attempt to transform the uchar list back to a zfp array
    //zfp::array1<double> temp2(&head, ptr);
    
    // Create a zfp struct
    ZFPStruct zfp_struct;
    zfp_struct.a = 10;
    zfp_struct.b = 20;
    zfp_struct.c = 30;
    zfp_struct.region_state.resize(9);
    zfp_struct.region_state.set_rate(64.0);

    // Create MPI Datatype to send ZFPStruct
    int i, j, myrank;
    MPI_Status status;
    MPI_Datatype zfptype;

    MPI_Datatype type[1] = {MPI_BYTE};
    int blocklen[1] = {12 + sizeof(temp)};
    MPI_Aint disp[1] = {0};

    //struct Partstruct particle[1000];
    //int i, j, myrank;
    //MPI_Status status;
    //MPI_Datatype Particletype;
    //MPI_Datatype type[3] = { MPI_CHAR, MPI_DOUBLE, MPI_CHAR };
    //int blocklen[3] = { 1, 6, 7 };
    //MPI_Aint disp[3] = {0, 1 * sizeof(char), 1 * sizeof(char) + 6 * sizeof(double)};

    //MPI_Datatype type[1] = { MPI_BYTE };
    //int blocklen[1] = { 56 };
    //MPI_Aint disp[1] = {0};
 
    MPI_Init(&argc, &argv);
 
    MPI_Type_create_struct(1, blocklen, disp, type, &zfptype);
    MPI_Type_commit(&zfptype);
 
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
 
    if (myrank == 0){
        ZFPStruct zfp_struct_og;
        zfp_struct.a = 10;
        zfp_struct.b = 20;
        zfp_struct.c = 30;
        zfp_struct.region_state.resize(9);
        zfp_struct.region_state.set_rate(64.0);
        zfp_struct.region_state[0] = 1.123;
        zfp_struct.region_state[1] = 5.432;
        zfp_struct.region_state[2] = 9.999;

        MPI_Send(&zfp_struct, 1, zfptype, 1, 123, MPI_COMM_WORLD);
    }
    else if (myrank == 1)
    {
        ZFPStruct zfp_struct_tmp;
        MPI_Recv(&zfp_struct, 1, zfptype, 0, 123, MPI_COMM_WORLD, &status);
        std::cout << zfp_struct.a << " " << zfp_struct.b << " " << zfp_struct.c << std::endl;
        std::cout << zfp_struct.region_state[0] << " " << zfp_struct.region_state[1] << " " << zfp_struct.region_state[2] << std::endl;
    }
    MPI_Finalize();
    return 0; 
}