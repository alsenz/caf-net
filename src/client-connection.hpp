#pragma once

#include <sstream>
#include <cstdio>
#include <chrono>
#include <string>
#include <string_view>

#include "caf/all.hpp"
#include "caf/io/all.hpp"

#include "common/logger.hpp"

#include "caf-net/http-handler.hpp"
#include "caf-net/connection.hpp" //For MiB2b
#include "caf-net/request.hpp"
#include "caf-net/response.hpp"


namespace as::net {

    using client_connection_broker = caf::io::typed_broker<
            caf::reacts_to<caf::io::new_data_msg>,
            caf::reacts_to<caf::io::connection_closed_msg>,
            caf::replies_to<net::request>::with<net::response>
    >;

    class client_connection : public client_connection_broker, public http_handler {

    public:

        client_connection(caf::actor_config &cfg, const caf::io::connection_handle &hdl, const std::string &user_agent, size_t max_response_body_heap = MiB2b(16)); //_rp is invalid!

        behavior_type make_behavior() override;

    protected:

        void handle_exit(const caf::exit_msg &msg);

        void handle_down(const caf::down_msg &msg);

        void handle_connection_closed(const caf::io::connection_closed_msg &ccm);

        auto handle_request(const net::request &req) -> caf::result<net::response>;

        void on_message_begin() override;

        void on_message_complete() override;

        void on_status(net::status_t status) override;

        void on_header(const std::string &key, const std::string &value) override;

        //NOTE-- this is essentially a bit of a dupe of the version in connection.hpp, just for responses!
        void on_body(const std::string_view &body_data) override;

    private:

        //Special case callback: when the request is either to large or the mime type suggests it
        void on_convert_request_body_to_file();

        void dispatch_current_response();

        caf::io::connection_handle _handle;
        std::string _user_agent;
        std::size_t _max_response_body_heap;

        bool dispatched = false;
        net::response _current_response = net::response();

        caf::response_promise _rp;

    };

} //ns as::net
