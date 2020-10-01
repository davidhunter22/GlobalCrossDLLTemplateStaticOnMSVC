#include "global_instance.h"

#include <unordered_map>
#include <stdexcept>

std::any& get_global_instance( std::type_index type_index )
{
    static std::unordered_map<std::type_index,std::any> global_instance_cache;

    return global_instance_cache[ type_index ];
}
