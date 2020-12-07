#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <stdexcept>

#include "caf/all.hpp"
#include "caf/uri.hpp"
#include "caf/detail/append_percent_encoded.hpp"

#include "common/hash-combine.hpp"

namespace gnt::dln {

    using url = caf::uri;
    using uri = caf::uri;

    uri parse(std::string_view uri_str);

    uri parse(const std::string &uri);

    // Missing "to_string" for the host type...
    std::string to_string(const uri::host_type &host);

} //ns gnt::dln