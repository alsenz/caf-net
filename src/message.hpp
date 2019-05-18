#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <optional>
#include <functional>

#include "nlohmann/json.hpp"

#include "caf/all.hpp"

namespace as::net {

    //Base class for requests and responses
    class message {
    public:

        message();

        //Note- this excludes some clever or mmapey support for very large bodies, but in my experience,
        //when building an API server, it's never worth trying to engineer in specific support-- the right
        //design is to have a file server for file uploads, and then have the request connect to it
        using body_data_t = std::vector<std::byte>;

        using header_data = std::unordered_map<std::string, std::string>;

        body_data_t &body();

        const body_data_t  &body() const;

        header_data &headers();

        const header_data &headers() const;

        std::optional<std::string> header(const std::string &key) const;

        void set_header(const std::string &key, const std::string &value);

        void emplace_header(const std::string &key, std::string &&value);

        bool is_filebody() const;

        std::FILE *filebody() const;

        void filebody(std::FILE *fb);

        //! Un-clever json parsing -- will throw if it's not json
        nlohmann::json json_body() const;

    private:

        header_data _headers;
        body_data_t _body;

        std::FILE *_fb;

    };

    template <class Inspector>
    typename Inspector::result_type inspect(Inspector& f, message& x) {
        return f(caf::meta::type_name("net::message"), x.headers(), x.body());
    }

} //ns as net
