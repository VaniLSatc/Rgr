#pragma once

#ifdef _WIN32
    #include <windows.h>
    #define LIB_HANDLE HMODULE
    #define LOAD_LIB(name) LoadLibraryA(name)
    #define GET_PROC(handle, name) GetProcAddress(handle, name)
    #define FREE_LIB(handle) FreeLibrary(handle)
#else
    #include <dlfcn.h>
    #define LIB_HANDLE void*
    #define LOAD_LIB(name) dlopen(name, RTLD_LAZY)
    #define GET_PROC(handle, name) dlsym(handle, name)
    #define FREE_LIB(handle) dlclose(handle)
#endif
