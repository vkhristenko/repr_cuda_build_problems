#include <iostream>
#include <cassert>

#include "kernels.h"

namespace lib2 {

__device__
void some_dev_func() {}

__global__
void kernel_test() { 
    printf("hello from lib2 kernel\n"); 
    some_dev_func();
}

void doEntryPoint() { 
    std::cout << "hello from lib2\n"; 
    auto check_error = [](auto code) {
        if (code != cudaSuccess) {
            std::cout << cudaGetErrorString(code) << std::endl;
            assert(false);
        }
    };

    kernel_test<<<1,1>>>();
    check_error(cudaGetLastError());
    cudaDeviceSynchronize();
}

}
