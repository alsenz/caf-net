#include <functional>

#include "url.h"

#include "common/hash-combine.hpp"

namespace gnt::dln {

    using url = Url::Url;

    template <class Inspector>
    typename Inspector::result_type inspect(Inspector &f, url &x) {
        auto get_port = [&x]() -> decltype(auto) { return x.port(); };
        auto set_port = [&x](int value) {
            x.setPort(value);
            return true;
        };
        auto get_host = [&x]() -> decltype(auto) { return x.host(); };
        auto set_host = [&x](std::string value) {
            x.setHost(std::move(value));
            return true;
        };
        auto get_path = [&x]() -> decltype(auto) { return x.path(); };
        auto set_path = [&x](std::string value) {
            x.setPath(std::move(value));
            return true;
        };
        auto get_params = [&x]() -> decltype(auto) { return x.params(); };
        auto set_params = [&x](std::string value) {
            x.setParams(std::move(value));
            return true;
        };
        auto get_query = [&x]() -> decltype(auto) { return x.query(); };
        auto set_query = [&x](std::string value) {
            x.setQuery(std::move(value));
            return true;
        };
        auto get_fragment = [&x]() -> decltype(auto) { return x.fragment(); };
        auto set_fragment = [&x](std::string value) {
            x.setFragment(std::move(value));
            return true;
        };
        auto get_userinfo = [&x]() -> decltype(auto) { return x.userinfo(); };
        auto set_userinfo = [&x](std::string value) {
            x.setUserinfo(std::move(value));
            return true;
        };
        return f.object(x).fields(
            f.field("port", get_port, set_port),
            f.field("host", get_host, set_host),
            f.field("path", get_path, set_path),
            f.field("params", get_params, set_params),
            f.field("query", get_query, set_query),
            f.field("fragment", get_fragment, set_fragment),
            f.field("userinfo", get_userinfo, set_userinfo)
        );
    }

} //ns gnt::dln

namespace std {

    template<>
    struct hash<gnt::dln::url> {
        std::size_t operator()(const gnt::dln::url &x) const {
            std::size_t seed = x.port()+7;
            hash_combine(seed, x.host(), x.path(), x.params(), x.query(), x.fragment(), x.userinfo());
            return seed;
        }
    };

} //ns std