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
#include "mpi.h"
#include "zfparray1.h"
#include "zfparray2.h"
#include "zfparray3.h"

int main(int argc, char *argv[]){
    // Set up global variables
    int M = 5;
    int N = 3;
    int precision = 10;
    double min = -1000000;
    double max = 1000000;

    // Process specific variables
    MPI_Status status;
    int rank;
    int size;
    double* matrix_row = new double[N];
    double** matrix = new double*[M];
    double* vector = new double[N];
    double* result = new double[M];
    int jobs;

    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get current processes rank and number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check to make sure there are at least two processes
    if (size == 1){
        std::cout << "Requires at least 2 processes, please run with more than 1 process..." << std::endl;
        MPI_Finalize();
        return 1;
    }

	// Initialize Job Schedule
	int* job_schedule = new int[size-1];
    for (int k = 0; k < size-1; k++){
        job_schedule[k] = 0;
    }

    // have the first processor generate and send data
    if (rank == 0){
		std::cout << "Number of Processes: " << size << std::endl;

        // Initialize Randomization 
        //srand((unsigned)time(NULL));
        srand(0);

        // Create matrix
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
        // Create vector
        std::cout << "Vector: " << std::endl;
        for (int j = 0; j < N; j++){
            vector[j] = (max - min) * ((double)rand() / (double)RAND_MAX) + min;
            std::cout << std::setprecision(precision) << vector[j] << " " << std::endl;
        }

        // Determine how many rows each process will do
        int rows = M;
        int schedule_index = 0;
        while (rows != 0){
            job_schedule[schedule_index] = job_schedule[schedule_index] + 1;
            schedule_index++;
			if (schedule_index == size-1){
				schedule_index = 0;
			}
            rows = rows - 1;
        }
        
        // Tell each process how many rows they will be processing
        for (int k = 1; k < size; k++){
            MPI_Send(&job_schedule[k-1], 1, MPI_INT, k, 99, MPI_COMM_WORLD);
        }
    } else {
        // Determine how mnay rows this process will be processing
        MPI_Recv(&jobs, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Broadcast Vector out to all other processes
    MPI_Bcast(vector, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Have root send rows of the matrix off to corresponding processes as needed
    int results_index = 0;
    int current_matrix_row = 0;
    if (rank == 0){
        while(true){
            // Set number of jobs to recieve 
            int jobs_to_return = 0;

            // Send out matrix rows to all processes that need them
            for (int k = 1; k < size; k++){
                if (job_schedule[k-1] != 0){
                    // Send current matrix row
                    MPI_Send(matrix[current_matrix_row], N, MPI_DOUBLE, k, 50, MPI_COMM_WORLD);
                    current_matrix_row++;
                    // Decrement job schedule
                    job_schedule[k-1] = job_schedule[k-1] - 1;
                    // Increment jobs to recieve
                    jobs_to_return++;
                }
            }

            // Check to see if any jobs were sent off
            if (jobs_to_return == 0){
                break;
            }

            // Recieve jobs that are out and place result into result array
            int sender_rank = 1;
            for (int k = 0; k < jobs_to_return; k++){
                // Recieve result and store result
                MPI_Recv(&result[results_index], 1, MPI_DOUBLE, sender_rank, 25, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                results_index++;
                sender_rank++;
            }
        }
    // Have all other processes do the multiplication
    } else {
        for (int k = 0; k < jobs; k++){
            // Recieve Row
            MPI_Recv(matrix_row, N, MPI_DOUBLE, 0, 50, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Multiply 
            double result = 0;
            for (int j = 0; j < N; j++){
                result += (matrix_row[j] * vector[j]);
            }

            // Return Result
            MPI_Send(&result, 1, MPI_DOUBLE, 0, 25, MPI_COMM_WORLD);
        }
    }

    if (rank == 0){
        // Print the result
        std::cout << "Result: " << std::endl;
        for (int i = 0; i < M; i++){
            std::cout << result[i] << std::endl;
        }
    }




    // ZFP Section 


    // Set global variables
    double rate = 48.0;

    // Process specific variables
    zfp::array1<double> zmatrix_row(N, rate); 
    zfp::array2<double> zmatrix(M,N, rate);
    zfp::array1<double> zvector(N, rate);
    zfp::array1<double> zresult(M, rate);

    // Get current processes rank and number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check to make sure there are at least two processes
    if (size == 1){
        std::cout << "Requires at least 2 processes, please run with more than 1 process..." << std::endl;
        MPI_Finalize();
        return 1;
    }

	// Initialize Job Schedule
    for (int k = 0; k < size-1; k++){
        job_schedule[k] = 0;
    }

    // have the first processor generate and send data
    if (rank == 0){
		std::cout << "Number of Processes: " << size << std::endl;

        // Initialize Randomization 
        //srand((unsigned)time(NULL));
        srand(0);

        // Create matrix
        std::cout << "ZFP Matrix: " << std::endl;
        for (int i = 0; i < M; i++){
            for (int j = 0; j < N; j++){
                zmatrix[i * N + j] = matrix[i][j];
                zmatrix.flush_cache();
                std::cout << std::setprecision(precision) << zmatrix[i * N + j] << " ";
            }
            std::cout << std::endl;
        }
        // Create vector
        std::cout << "ZFP Vector: " << std::endl;
        for (int j = 0; j < N; j++){
            zvector[j] = vector[j];
            zvector.flush_cache();
            std::cout << std::setprecision(precision) << zvector[j] << " " << std::endl;
        }

        // Determine how many rows each process will do
        int rows = M;
        int schedule_index = 0;
        while (rows != 0){
            job_schedule[schedule_index] = job_schedule[schedule_index] + 1;
            schedule_index++;
			if (schedule_index == size-1){
				schedule_index = 0;
			}
            rows = rows - 1;
        }
        
        // Tell each process how many rows they will be processing
        for (int k = 1; k < size; k++){
            MPI_Send(&job_schedule[k-1], 1, MPI_INT, k, 99, MPI_COMM_WORLD);
        }
    } else {
        // Determine how mnay rows this process will be processing
        MPI_Recv(&jobs, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Move Zvector over to vector_buffer
    double* vector_buffer = new double[N];
    for (int j = 0; j < N; j++){
        vector_buffer[j] = zvector[j]; 
    }

    // Broadcast Vector Buffer out to all other processes
    MPI_Bcast(vector_buffer, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Move Vector Buffer back to zvector
    for (int j = 0; j < N; j++){
        zvector[j] = vector_buffer[j]; 
        zvector.flush_cache();
    }

    // Have root send rows of the matrix off to corresponding processes as needed
    results_index = 0;
    current_matrix_row = 0;
    double* matrix_row_buffer = new double[N];
    if (rank == 0){
        while(true){
            // Set number of jobs to recieve 
            int jobs_to_return = 0;

            // Send out matrix rows to all processes that need them
            for (int k = 1; k < size; k++){
                if (job_schedule[k-1] != 0){
                    // Copy matrix row to buffer
                    for (int j = 0; j < N; j++){
                        matrix_row_buffer[j] = zmatrix[current_matrix_row * N + j];
                    }
                    // Send current matrix row
                    MPI_Send(matrix_row_buffer, N, MPI_DOUBLE, k, 50, MPI_COMM_WORLD);
                    current_matrix_row++;
                    // Decrement job schedule
                    job_schedule[k-1] = job_schedule[k-1] - 1;
                    // Increment jobs to recieve
                    jobs_to_return++;
                }
            }

            // Check to see if any jobs were sent off
            if (jobs_to_return == 0){
                break;
            }

            // Recieve jobs that are out and place result into result array
            int sender_rank = 1;
            double tmp_result = 0;
            for (int k = 0; k < jobs_to_return; k++){
                // Recieve result and store result
                MPI_Recv(&tmp_result, 1, MPI_DOUBLE, sender_rank, 25, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                zresult[results_index] = tmp_result;
                zresult.flush_cache();
                results_index++;
                sender_rank++;
            }
        }
    // Have all other processes do the multiplication
    } else {
        for (int k = 0; k < jobs; k++){
            // Recieve Row
            MPI_Recv(matrix_row_buffer, N, MPI_DOUBLE, 0, 50, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // Copy buffer into zmatrix_row
            for (int j = 0; j < N; j++){
                zmatrix_row[j] = matrix_row_buffer[j];
                zmatrix_row.flush_cache();
            }

            // Multiply 
            double result = 0;
            for (int j = 0; j < N; j++){
                result += (zmatrix_row[j] * zvector[j]);
            }

            // Return Result
            MPI_Send(&result, 1, MPI_DOUBLE, 0, 25, MPI_COMM_WORLD);
        }
    }

    if (rank == 0){
        // Print the result
        std::cout << "ZFP Results: " << std::endl;
        for (int i = 0; i < M; i++){
            std::cout << zresult[i] << std::endl;
        }

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
        //std::cout << "Standard Time Take: " << standard_time_taken << std::endl;
        //std::cout << "ZFP Time Take: " << zfp_time_taken << std::endl;
        std::cout << "Maximum Absolute Difference: " << max_diff << std::endl;
        std::cout << "Root Mean Squared Error: " << rmse << std::endl;
        std::cout << "PSNR: " << psnr << std::endl;
    }

    MPI_Finalize();
    return 0;
}
