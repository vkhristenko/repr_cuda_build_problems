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

    // open up lib 1 and check
    std::cout << "opening shared libs\n";
    void *handle1 = dlopen(libName1.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    const char* dlsym_error = nullptr;
    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot open lib: " << libName1 << " problem: " 
                  << dlsym_error << '\n';
        return 1;
    }
    if (!handle1) {
        std::cerr << "invalid handle for lib: " <<  libName1 << std::endl;
        return 1;
    }

    // open up lib 2 and check
    void *handle2 = dlopen(libName2.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot open lib: " << libName2 << " problem: "
                  << dlsym_error << std::endl;
        return 1;
    }
    if (!handle2) {
        std::cout << "invalid handle for lib: " << libName2 << std::endl;
        return 1;
    }

    // extract the symbols for the 1st
    using func = void(*)();
    func entryPoint1 = (func)dlsym(handle1, symbName.c_str());
    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot get symbols for lib: " << libName1 << " problem: " 
                  << dlsym_error << std::endl;
        return 1;
    }

    // extract the symbold for the 2nd
    func entryPoint2 = (func)dlsym(handle2, symbName.c_str());
    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot get symbols for lib: " << libName2 << " problem: " 
                  << dlsym_error << std::endl;
        return 1;
    }


    // run 1 and 2
    std::cout << "calling entry point 1\n";
    entryPoint1();
    std::cout << "calling entry point 2\n";
    entryPoint2();

    return 0;
}
