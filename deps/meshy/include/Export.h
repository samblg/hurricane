#pragma once

#ifdef WIN32
#ifdef MESHY_EXPORTS
#define MESHY_API __declspec(dllexport)
#else
#define MESHY_API __declspec(dllimport)
#endif
#else
#define MESHY_API
#endif