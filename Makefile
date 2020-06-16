PROJECT_DIR=$(shell pwd)

CXXFLAGS=-fPIC -std=c++14 -I$(PROJECT_DIR)
LDFLAGS=-rdynamic

osname := $(shell uname -s)
LIB_EXTENSION = dylib
LDFLAGS += -ldl
ifeq ($(osname), Linux)
	LIB_EXTENSION = so
endif

#all: test libplugins.$(LIB_EXTENSION)
all: driver lib1.$(LIB_EXTENSION) lib2.$(LIB_EXTENSION)

driver: driver.o
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

lib1.$(LIB_EXTENSION): $(PROJECT_DIR)/lib1/lib1.o
	nvcc -arch=sm_61 -dlink --compiler-options '-std=c++14 -fPIC' -lcudadevrt -std=c++14 $^ -o $(PROJECT_DIR)/lib1/lib1_device.o
	g++ $(CXXFLAGS) $(PROJECT_DIR)/lib1/lib1_device.o $^ ${LDFLAGS} -L/usr/local/cuda/lib64 -lcudart -shared -o $@

lib2.$(LIB_EXTENSION): $(PROJECT_DIR)/lib2/lib2.o
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -shared -o $@

#libplugins.$(LIB_EXTENSION): $(PROJECT_DIR)/TestPlugins/src/Plugin1.o $(PROJECT_DIR)/PluginManager/src/Manager.o
#	g++ $(CXXFLAGS) $^ $(LDFLAGS) -shared -o $@

#  3 all: $(objects)
#    4     nvcc -g -G -arch=sm_35 -dlink ${objects} -o device_stuff.o 
#	  5     g++  device_stuff.o ${objects} -L/usr/local/cuda/lib64/ -lcudart -o app
#	    6 
#		  7 %.o: %.cu
#		    8     nvcc -x cu -arch=sm_35 -I. -dc $< -o $@

%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@

%.o: %.cu
	nvcc -dc -std=c++14 -arch=sm_61 --compiler-options '-std=c++14 -fPIC' $< -o $@

clean:
	rm driver *.o *.$(LIB_EXTENSION) lib1/*.o lib2/*.o
