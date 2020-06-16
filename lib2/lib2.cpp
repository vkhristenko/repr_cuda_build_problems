#include <iostream>

extern "C" {
    void entryPoint() { std::cout << "hello from lib2\n"; }
}
