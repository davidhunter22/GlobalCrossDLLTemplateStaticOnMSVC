#pragma once

#if defined(COUNTER_USER_2_EXPORTS)
    #define COUNTER_USER_2_DECL_SPEC __declspec(dllexport)
#else
    #define COUNTER_USER_2_DECL_SPEC __declspec(dllimport)
#endif


// A couple of functions that get and set the value in a Counter<int>
COUNTER_USER_2_DECL_SPEC int  get_counter_for_int_type_2( );
COUNTER_USER_2_DECL_SPEC void set_counter_for_int_type_2( int );