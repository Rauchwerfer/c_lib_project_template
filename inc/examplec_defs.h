#ifndef EXAMPLEC_DEFS_H
#define EXAMPLEC_DEFS_H

// Detect platform
#if defined _WIN32 || defined __CYGWIN__
    #define EXAMPLEC_HELPER_DLL_IMPORT __declspec(dllimport)
    #define EXAMPLEC_HELPER_DLL_EXPORT __declspec(dllexport)
    #define EXAMPLEC_HELPER_DLL_LOCAL
#else
    #ifdef __GNUC__
        #if __GNUC__ >= 4
            #define EXAMPLEC_HELPER_DLL_IMPORT __attribute__((visibility("default")))
            #define EXAMPLEC_HELPER_DLL_EXPORT __attribute__((visibility("default")))
            #define EXAMPLEC_HELPER_DLL_LOCAL __attribute__((visibility("hidden")))
        #else
            #define EXAMPLEC_HELPER_DLL_IMPORT
            #define EXAMPLEC_HELPER_DLL_EXPORT
            #define EXAMPLEC_HELPER_DLL_LOCAL
        #endif
    #else
        #error UNSUPPORTED COMPILER
    #endif
#endif

#ifdef EXAMPLEC_STATIC_DEFINE
  #define EXAMPLEC_API
  #define EXAMPLEC_NO_EXPORT
#else
  #ifdef EXAMPLEC_EXPORTS  // Defined when building the library
    #define EXAMPLEC_API EXAMPLEC_HELPER_DLL_EXPORT
  #else
    #define EXAMPLEC_API EXAMPLEC_HELPER_DLL_IMPORT
  #endif
  #define EXAMPLEC_NO_EXPORT EXAMPLEC_HELPER_DLL_LOCAL
#endif

#endif // EXAMPLEC_DEFS_H