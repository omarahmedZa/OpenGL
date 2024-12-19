#pragma once

#include <GL/glew.h>
#include <iostream>

using namespace std;

#ifdef NDEBUG
    #define ASSERT(x) ((void)0) // No-op in release builds
#else
    // Define a function to break into the debugger
    inline void debugBreak() {
        #if defined(__clang__) || defined(__GNUC__)
            __builtin_trap(); // Portable way to trigger a breakpoint
        #elif defined(_MSC_VER)
            __debugbreak(); // For MSVC (optional redundancy if cross-platform)
        #else
            raise(SIGTRAP); // Default fallback
        #endif
    }

    #define ASSERT(x)                                          \
        do {                                                   \
            if (!(x)) {                                        \
                std::cerr << "Assertion failed: " #x           \
                          << ", file " << __FILE__             \
                          << ", line " << __LINE__ << std::endl; \
                debugBreak();                                  \
                std::abort();                                  \
            }                                                  \
        } while (0)
#endif

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))



void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
