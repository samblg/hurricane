/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

#pragma once

#include "logging/Logging.h"

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
        LOG(LOG_ERROR) << dlerror();
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
