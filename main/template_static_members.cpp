
#include <counter/Counter.h>
#include <counter_user_1/code.h>
#include <counter_user_2/code.h>

#include <cassert>

int main( int, char** )
{
    // These name are obviously different so they will have distinct type_index values
    auto n1 { typeid(int).name( ) };
    auto n2 { typeid(Counter<int>).name( ) };

    // This will get you a default constructed instance of an int which may have any value
    auto value = get_counter_for_int_type_1( );

    set_counter_for_int_type_1( 99 );

    // This gets the value for Counter<int> in a different DLL to the one its was set in which shows they shared the same Counter<int> value
    value = get_counter_for_int_type_2( );

    assert( value == 99 );

    return 0;
}