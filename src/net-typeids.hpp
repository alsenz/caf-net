#pragma once

#include "caf/all.hpp"

// Forward declare everything we're adding
namespace as::net {

    // Block out a silly custom type block
    constexpr caf::type_id_t custom_type_block_start = 18100; //Some sufficiently large far-away type block
    constexpr caf::type_id_t custom_type_block_end = custom_type_block_start + 32; //Leaves us 32 custom types
    constexpr caf::type_id_t custom_type_block_safe_max = custom_type_block_start + 999; // We promsie not to use more than a thosuand typeids.

    // Lots of forward declarations
    class request;
    enum class method_t;
    class response;
    class get_request;
    class put_request;
    class head_request;
    class post_request;
    class delete_request;
    class options_request;


} //ns as net

CAF_BEGIN_TYPE_ID_BLOCK(caf_net, as::net::custom_type_block_start)

    CAF_ADD_ATOM(caf_net, as::net, add_route_atom)
    CAF_ADD_TYPE_ID(caf_net, (as::net::request))
    CAF_ADD_TYPE_ID(caf_net, (as::net::response))
    CAF_ADD_TYPE_ID(caf_net, (as::net::method_t))
    CAF_ADD_TYPE_ID(caf_net, (as::net::get_request))
    CAF_ADD_TYPE_ID(caf_net, (as::net::post_request))
    CAF_ADD_TYPE_ID(caf_net, (as::net::put_request))
    CAF_ADD_TYPE_ID(caf_net, (as::net::head_request))
    CAF_ADD_TYPE_ID(caf_net, (as::net::delete_request))
    CAF_ADD_TYPE_ID(caf_net, (as::net::options_request))

CAF_END_TYPE_ID_BLOCK(caf_net)