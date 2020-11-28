#include <functional>

#include "url.h"

#include "common/hash-combine.hpp"

namespace gnt::dln {

    using url = Url::Url;

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