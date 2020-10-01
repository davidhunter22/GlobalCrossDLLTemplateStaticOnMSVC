#pragma once

#include <any>
#include <typeinfo>
#include <typeindex>

#if defined(GLOBAL_INSTANCE_EXPORTS)
    #define GLOBAL_INSTANCE_DECL_SPEC __declspec(dllexport)
#else
    #define GLOBAL_INSTANCE_DECL_SPEC __declspec(dllimport)
#endif

// A globally visible function that does not require us to know the type T
// Users don't normally call this directly they call the template version
GLOBAL_INSTANCE_DECL_SPEC std::any& get_global_instance( std::type_index type_index );

// A template version that does the correct casting and set an initial default value
// T : The type used for the lookup using it's type_index
// U : The stored type, there is one global instance of the U type for each T type
template <typename T, typename U> U& get_global_instance( )
{
    auto& instance { get_global_instance( typeid(T) ) };

    // If this is the first time we have tried to get this global instance the std::nay returned will be empty
    // So we make sure the std::any contains a default constructed instance. There are other way that this could
    // work but this is good enough as an example.
    if( instance.has_value( ) == false )
    {
        instance = U { };
    }

    return std::any_cast<U&>( instance );
}




