#include <iostream>

extern "C" {
    void entryPoint() { std::cout << "hello from lib1\n"; }
}
