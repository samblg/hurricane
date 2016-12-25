#pragma once

#ifdef WIN32
#ifdef LOGGING_EXPORTS
#define LOGGING_API __declspec(dllexport)
#else
#define LOGGING_API __declspec(dllimport)
#endif
#else
#define LOGGING_API
#endif