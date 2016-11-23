#include "hurricane/base/Library.h"

#ifndef WIN32
#include <dlfcn.h>
#else
#include <Windows.h>

extern HMODULE LibrarayHandle;
#endif

#include <string>
#include <iostream>

using namespace std;

std::string GetLibraryPath() {
#ifndef WIN32
    Dl_info dllInfo;

    int ret = dladdr((void*)(GetLibraryPath), &dllInfo);
    if ( !ret ) {
        std::cout << "[ERROR] Get Library Path failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string libraryFilePath = dllInfo.dli_fname;
    std::string::size_type pos = libraryFilePath.rfind('/');
#else
    const int MAX_FILE_NAME = 1000;

    char moduleFileName[MAX_FILE_NAME];
    GetModuleFileName(LibrarayHandle, moduleFileName, MAX_FILE_NAME);

    std::string libraryFilePath = moduleFileName;
    std::string::size_type pos = libraryFilePath.rfind('\\');
#endif

    if ( pos == std::string::npos ) {
        return ".";
    }

    return libraryFilePath.substr(0, pos);
}
