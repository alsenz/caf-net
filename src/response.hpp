#pragma once

#include <ostream>
#include <ctime>
#include <sstream>
#include <string_view>

#include "nlohmann/json.hpp"

#include "caf-net/net-typeids.hpp"
#include "caf-net/message.hpp"
#include "caf-net/status.hpp"

namespace as::net {

    class response : public message {
    public:

        response();

        response(status_t status);

        response(status_t status, const nlohmann::json &json);

        status_t status() const;

        //For inspection
        status_t &status();

        void status(status_t status);

        bool no_response_body();

        void append_body(const std::byte *data, std::size_t len);

        void append_body(const std::string &data);

        void write_json_body(const nlohmann::json &json);

        std::string_view body_view() const;

        void make_stock_response(const std::string &server_name = "http server", const std::string &extra_msg = "");

        void write(std::ostream &ostream, const std::string &http_version = "HTTP/1.1") const;

    private:

        status_t  _status;

    };

    template <class Inspector>
    typename Inspector::result_type inspect(Inspector& f, response& x) {
        return f(caf::meta::type_name("net::response"), x.status(), static_cast<message &>(x));
    }

} //ns as::net
