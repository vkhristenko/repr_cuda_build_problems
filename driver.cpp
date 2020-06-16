#include <cstdlib>
#include <dlfcn.h>

#include <iostream>

int main(int argc, char** argv) {
    // lib names
#ifdef __APPLE__
    std::string libName1 = "lib1.dylib";
    std::string libName2 = "lib2.dylib";
#elif __linux__
    char *cwd = std::getenv("PWD");
    std::string libName1 = std::string(cwd) + std::string{"/"} + 
        std::string{"lib1.so"};
    std::string libName2 = std::string(cwd) + std::string{"/"} + 
        std::string{"lib2.so"};
#endif
    std::string symbName = "entryPoint";

    // open up libs and get a hold of the entry point symbols
    void *handle1 = dlopen(libName1.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    void *handle2 = dlopen(libName2.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    using func = void(*)();
    func entryPoint1 = (func)dlsym(handle1, symbName.c_str());
    func entryPoint2 = (func)dlsym(handle2, symbName.c_str());

    // run 1 and 2
    entryPoint1();
    entryPoint2();

    return 0;
}
