#include "dandelion/url.hpp"

namespace gnt::dln {

    uri parse(std::string_view uri_str) {
        auto res = caf::make_uri(uri_str);
        if(!res) {
            throw std::invalid_argument(caf::to_string(res.error()));
        } else {
            return std::move(res.value());
        }
    }

    uri parse(const std::string &uri) {
        return parse(std::string_view(uri));
    }

} // ns gnt::dln