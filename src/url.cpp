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

    std::string to_string(const uri::host_type &host) {
        std::string str;
        auto f = caf::detail::make_overload(
            [&](const caf::ip_address& addr) {
                if (addr.embeds_v4()) {
                    str += to_string(addr);
                } else {
                    str += '[';
                    str += to_string(addr);
                    str += ']';
                }
            },
            [&](const std::string& host) {
                caf::detail::append_percent_encoded(str, host);
            });
        visit(f, host);
        return str;
    }

} // ns gnt::dln