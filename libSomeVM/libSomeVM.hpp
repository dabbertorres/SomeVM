#pragma once

#ifdef _WIN32
    #ifdef _WINDLL
        #define SVM_API __declspec(dllexport)
    #else
        #define SVM_API __declspec(dllimport)
    #endif
#else
    #define SVM_API
#endif
