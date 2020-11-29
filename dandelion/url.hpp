#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <stdexcept>

#include "caf/all.hpp"
#include "caf/uri.hpp"

#include "common/hash-combine.hpp"

namespace gnt::dln {

    using url = caf::uri;
    using uri = caf::uri;

    uri parse(std::string_view uri_str);

    uri parse(const std::string &uri);

} //ns gnt::dln