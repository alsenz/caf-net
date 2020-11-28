#pragma once

#include <sstream>
#include <memory>
#include <cstdio>
#include <chrono>
#include <string>
#include <string_view>

#include "caf/all.hpp"
#include "caf/io/all.hpp"

#include "common/logger.hpp"

#include "router.hpp"
#include "response.hpp"
#include "http-handler.hpp"

namespace as::net {

    // Convert mbits to bytes
    constexpr std::size_t MiB2b(std::size_t mb) {
        return mb * 1024 * 1024;
    }

    typedef caf::io::typed_broker<
            caf::reacts_to<caf::io::new_data_msg>,
            caf::reacts_to<caf::io::connection_closed_msg>,
            caf::reacts_to<net::response>
    > connection_broker;

    class connection : public connection_broker, public http_handler {

    public:

        connection(caf::actor_config &cfg, caf::io::connection_handle hdl, net::router_actor &router,
                   std::size_t max_request_body_heap_size = MiB2b(16));

        behavior_type make_behavior() override;

    protected:

        void handle_connection_closed(const caf::io::connection_closed_msg &ccm);

        void handle_response(const net::response &resp);

        void handle_exit(const caf::exit_msg &msg);

        void handle_down(const caf::down_msg &msg);

        //Implementations \/ for those callbacks

        void on_message_begin() override;

        void on_message_complete() override;

        void on_url(const std::string &url) override;

        void on_header(const std::string &key, const std::string &value) override;

        void on_body(const std::string_view &body_data) override;

    protected:

        //TODO I'm not sure how we clean up (i.e. fclose) these tmpfiles... need to investigate this.
        //Special case callback: when the request is either to large or the mime type suggests it
        void on_convert_request_body_to_file();

    private:

        net::router_actor &_router;
        std::size_t _max_req_body_heap;

        caf::io::connection_handle _handle;

        net::request _current_request = net::request();

    };


} //ns as::net
