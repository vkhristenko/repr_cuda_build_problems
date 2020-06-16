PROJECT_DIR=$(shell pwd)

CXXFLAGS=-fPIC -std=c++14 -I$(PROJECT_DIR)
LDFLAGS=-rdynamic

osname := $(shell uname -s)
LIB_EXTENSION = dylib
LDFLAGS += -ldl
ifeq ($(osname), Linux)
	LIB_EXTENSION = so
endif

all: driver lib1.$(LIB_EXTENSION) lib2.$(LIB_EXTENSION)

driver: driver.o
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

lib1.$(LIB_EXTENSION): $(PROJECT_DIR)/lib1/lib1.o $(PROJECT_DIR)/lib1/kernels_device.o $(PROJECT_DIR)/lib1/kernels.o
	g++ $(CXXFLAGS) $^ ${LDFLAGS} -shared -L/usr/local/cuda/lib64 -lcudart -o $@ 

lib2.$(LIB_EXTENSION): $(PROJECT_DIR)/lib2/lib2.o $(PROJECT_DIR)/lib2/kernels_device.o $(PROJECT_DIR)/lib2/kernels.o
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -shared -L/usr/local/cuda/lib64 -lcudart -o $@

%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@

$(PROJECT_DIR)/lib1/kernels_device.o: $(PROJECT_DIR)/lib1/kernels.o
	nvcc -arch=sm_61 -dlink --compiler-options '-std=c++14 -fPIC' -std=c++14 $^ -o $@

$(PROJECT_DIR)/lib2/kernels_device.o: $(PROJECT_DIR)/lib2/kernels.o $(PROJECT_DIR)/lib1/kernels.o
	nvcc -arch=sm_61 -dlink --compiler-options '-std=c++14 -fPIC' -std=c++14 $^ -o $@

%.o: %.cu
	nvcc -dc -std=c++14 -arch=sm_61 --compiler-options '-std=c++14 -fPIC' $< -o $@

clean:
	rm driver *.o *.$(LIB_EXTENSION) lib1/*.o lib2/*.o
