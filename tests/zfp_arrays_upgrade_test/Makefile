## Makefile for all zfp array examples

## COMPILER
CXX = g++
CXXFLAGS = -std=c++11 -g -Wall
MPICC = mpiCC

## FLAGS
LDFLAGS = -L ../../lib -fopenmp -lzfp -lm
INCLUDEFLAGS = -I ../../array -I ../../include -DZFP

all: upgrade

upgrade:
	$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) upgrade_zfp_test.cpp $(LDFLAGS) -o upgrade_zfp

clean:
	rm upgrade_zfp
