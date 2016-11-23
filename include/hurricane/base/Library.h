#pragma once

#include <string>
#include <iostream>

#ifdef WIN32

#include <Windows.h>

typedef HMODULE LibraryHandle;

inline LibraryHandle HurricaneLibraryLoad(const std::string& name) {
    std::string path = name + ".dll";
    return LoadLibraryA(path.c_str());
}

#define HurricaneLibraryFree FreeLibrary

inline DWORD HurricaneGetLibraryError() {
    return GetLastError();
}

#else

#include <dlfcn.h>

typedef void* LibraryHandle;
inline LibraryHandle HurricaneLibraryLoad(const std::string& name) {
    std::string path("lib");
    path += name + ".so";
    LibraryHandle handle = dlopen(path.c_str(), RTLD_NOW);
    if ( !handle ) {
        std::cerr << dlerror() << std::endl;
        exit(EXIT_FAILURE);
    }

    return handle;
}
#define HurricaneLibraryFree dlclose

#define HurricaneGetLibraryError() dlerror()

#endif

#ifdef __cplusplus
#include <string>

template <class Function>
Function HurricaneLibraryGetSymbol(LibraryHandle libraryHandle, const std::string& libraryName) {
#ifdef WIN32
    return reinterpret_cast<Function>(GetProcAddress(libraryHandle, libraryName.c_str()));
#else
    return reinterpret_cast<Function>(dlsym(libraryHandle, libraryName.c_str()));
#endif
}

std::string GetLibraryPath();

#endif // __cplusplus
