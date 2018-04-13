
VPATH = ../
BLAS_DIR := ${BLAS_DIR}
override CFLAGS += -Wall -I$(BLAS_DIR)/include
LDFLAGS=-L$(BLAS_DIR)/lib -Wl,-rpath,$(BLAS_DIR)/lib -lopenblas 

.PHONY : all clean

# Default rule
default : all 

all: benchmark_size

benchmark_size: benchmark_size.o device_model.o

clean:
	rm *.o benchmark_size

#device_model.o: ../device_model.c
