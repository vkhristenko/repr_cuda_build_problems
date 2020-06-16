#include <iostream>

__global__
void kernelt_test() { printf("hello from lib1 kernel\n"); }

extern "C" {
    void entryPoint() { 
        auto check_error = [](auto code) {
            if (code != cudaSuccess) {
                std::cout << cudaGetErrorString(code) << std::endl;
                assert(false);
            }
        };

        std::cout << "hello from lib1\n"; 
        kernel_test<<<1,1>>>();
        check_error(cudaGetLastError());
        cudaDeviceSynchronize();
    }
}
