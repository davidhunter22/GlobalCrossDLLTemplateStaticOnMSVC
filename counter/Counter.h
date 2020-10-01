#pragma once

#include <global_instance/global_instance.h>

#if defined(COUNTER_EXPORTS)
    #define COUNTER_DECL_SPEC __declspec(dllexport)
#else
    #define COUNTER__DECL_SPEC __declspec(dllimport)
#endif

// If you comment this out the test that we our sharing the same value across DLLs will fail
#define GLOBALS_WORK_AROUND

// An example of a template type that has a member variable that depends on the type parameter T
template <typename T> struct  Counter
{
    #if defined(GLOBALS_WORK_AROUND)
    static T get_counter( )
    {
        // Get the global instance of the T type for the Counter<T> type 
        return get_global_instance<Counter<T>,T>( );
    }

    static void set_counter( T c )
    {
        get_global_instance<Counter<T>,T>( ) = c;
    }
    #else
    static T get_counter( )
    {
        return value;
    }

    static void set_counter( T c )
    {
        value = c;
    }

    static inline T value;
    #endif
};



