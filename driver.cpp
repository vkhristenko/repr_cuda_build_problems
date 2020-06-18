#include <cstdlib>
#include <dlfcn.h>

#include <iostream>
#include <sstream>

struct Exception {
    explicit Exception() {}
};
        
using func_ptr = void(*)();

void* open_lib(std::string const& libName) {
    void *handle = dlopen(libName.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    const char* dlsym_error = nullptr;
    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot open lib: " << libName << " problem: " 
                  << dlsym_error << '\n';
        throw Exception{}; 
    }
    if (!handle) {
        std::cerr << "invalid handle for lib: " <<  libName << std::endl;
        throw Exception{};
    }

    return handle;
}

func_ptr get_symbol(
        void* handle, std::string const& libName, std::string const& symbName) {
    func_ptr entryPoint = (func_ptr)dlsym(handle, symbName.c_str());
    char const* dlsym_error = nullptr;
    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot get symbols for lib: " << libName << " problem: " 
                  << dlsym_error << std::endl;
        throw Exception{};
    }

    return entryPoint;
}

int main(int argc, char** argv) {
    // make sure lib bits are provided
    if (argc < 2) {
        std::cout << "run with ./driver lib_bits\n" ;
        return 1;
    }

    // 0 for no libs, 1 for load 1, 2 for load 2, 3 for load both
    std::stringstream strS; strS << argv[1];
    uint32_t lib_bits; strS >> lib_bits;
    bool load1 = lib_bits & 0x1;
    bool load2 = (lib_bits >> 1) & 0x1;
    if (lib_bits==0) { std::cout << "No Libraries will be loaded\n"; return 1; }
    if (load1 && load2)
        std::cout << "Loading lib1 then lib2\n";
    else if (load1)
        std::cout << "Loading lib1\n";
    else if (load2)
        std::cout << "Loading lib2\n";

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

    try {
        // open libs
        void *handle1, *handle2;
        if (load1) handle1 = open_lib(libName1);
        if (load2) handle2 = open_lib(libName2);

        // extract symbols
        func_ptr entryPoint1;
        func_ptr entryPoint2;
        if (load1) entryPoint1 = get_symbol(handle1, libName1, symbName);
        if (load2) entryPoint2 = get_symbol(handle2, libName2, symbName);

        // run 1 and 2
        if (load1) {
            std::cout << "calling entry point 1\n";
            entryPoint1();
        }
        if (load2) {
            std::cout << "calling entry point 2\n";
            entryPoint2();
        }
    } catch (Exception const& exc) {
        std::cerr << "Exiting...\n";
        return 1;
    }

    return 0;
}
