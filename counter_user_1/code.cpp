#include "code.h"

#include <counter/Counter.h>

void set_counter_for_int_type_1( int c )
{
    Counter<int>::set_counter( c );
}

int get_counter_for_int_type_1( )
{
    return Counter<int>::get_counter( );
}