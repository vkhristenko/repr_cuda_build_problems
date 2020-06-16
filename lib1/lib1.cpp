#include <iostream>
#include <cassert>

//#include "kernels.h"

extern "C" {
    void entryPoint() {
        std::cout << "hello from lib1\n";
  //      doEntryPoint();
    }
}
