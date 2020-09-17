## Makefile for all zfp array examples

## COMPILER
CXX = g++
CXXFLAGS = -std=c++11
MPICC = mpiCC

## FLAGS
LDFLAGS = -L ../../git/zfp/lib -fopenmp -lzfp -lm
INCLUDEFLAGS = -I ../../git/zfp/array -I ../../git/zfp/include -DZFP

all: matvecmul mpi_matvecmul openmp_matvecmul mpi_datatype_example quality_test

matvecmul:
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) zfp_matvecmul.cpp $(LDFLAGS) -o matvecmul

mpi_matvecmul:
	$(MPICC) $(CXXFLAGS) $(INCLUDEFLAGS) zfp_mpi_matvecmul.cpp $(LDFLAGS) -o mpi_matvecmul

openmp_matvecmul:
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) zfp_openmp_matvecmul.cpp $(LDFLAGS) -o openmp_matvecmul

mpi_datatype_example:
	$(MPICC) $(CXXFLAGS) $(INCLUDEFLAGS) zfp_mpi_datatype_example.cpp $(LDFLAGS) -o mpi_datatype_example

quality_test:
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) zfp_quality_test.cpp $(LDFLAGS) -o quality_test

clean:
	rm matvecmul
	rm mpi_matvecmul
	rm openmp_matvecmul
	rm mpi_datatype_example
	rm quality_test
