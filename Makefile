PROJECT_DIR=$(shell pwd)

CXXFLAGS=-fPIC -std=c++11 -I$(PROJECT_DIR)
LDFLAGS=-rdynamic

osname := $(shell uname -s)
LIB_EXTENSION = dylib
LDFLAGS += -ldl
ifeq ($(osname), Linux)
	LIB_EXTENSION = so
endif

#all: test libplugins.$(LIB_EXTENSION)
all: driver lib1.$(LIB_EXTENSION) lib2.$(LIB_EXTENSION)

driver: driver.o #$(PROJECT_DIR)/PluginManager/src/Manager.o
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

lib1.$(LIB_EXTENSION): $(PROJECT_DIR)/lib1/lib1.o
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -shared -o $@

lib2.$(LIB_EXTENSION): $(PROJECT_DIR)/lib2/lib2.o
	g++ $(CXXFLAGS) $^ $(LDFLAGS) -shared -o $@

#libplugins.$(LIB_EXTENSION): $(PROJECT_DIR)/TestPlugins/src/Plugin1.o $(PROJECT_DIR)/PluginManager/src/Manager.o
#	g++ $(CXXFLAGS) $^ $(LDFLAGS) -shared -o $@

%.o: %.cc
	g++ $(CXXFLAGS) -c $< -o $@

%.o: %.cu
	nvcc -x cu -arch=sm_61 -c $< -o $@

clean:
	rm driver *.o *.$(LIB_EXTENSION)
