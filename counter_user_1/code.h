#pragma once

#if defined(COUNTER_USER_1_EXPORTS)
    #define COUNTER_USER_1_DECLSPEC __declspec(dllexport)
#else
    #define COUNTER_USER_1_DECLSPEC __declspec(dllimport)
#endif

// A couple of functions that get and set the value in a Counter<int>
COUNTER_USER_1_DECLSPEC int  get_counter_for_int_type_1( );
COUNTER_USER_1_DECLSPEC void set_counter_for_int_type_1( int );
