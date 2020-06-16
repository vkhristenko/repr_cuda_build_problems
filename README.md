# Reproduce CUDA crash with Device side dependencies

## Running
```
git clone ...
cd repr_cuda_build_problems && make
./driver
```

the above sequence should produce:
```
[vkhriste@cmg-gpu1080 repr_cuda_build_problems]$ ./driver 
opening shared libs
calling entry point 1
hello from lib1
hello from lib1
hello from lib1 kernel
calling entry point 2
hello from lib2
hello from lib2
invalid device function
driver: /data/patatrack/vkhriste/repr_cuda_build_problems/lib2/kernels.cu:22: lib2::doEntryPoint()::<lambda(auto:1)> [with auto
```

## Requirements
 - `cuda` in `/usr/local/cuda` and `LD_LIBRARY_PATH` updated accordingly
