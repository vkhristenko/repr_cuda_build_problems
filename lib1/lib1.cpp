#include <iostream>
#include <cassert>

#include "kernels.h"

extern "C" {
    void entryPoint() {
        doEntryPoint();
    }
}
