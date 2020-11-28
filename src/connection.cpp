#include "dandelion/connection.hpp"

namespace as::net {


    connection::connection(caf::actor_config &cfg, caf::io::connection_handle hdl, net::router_actor &router,
                           size_t max_request_body_heap_size)
        : connection_broker(cfg), http_handler(mode::request), _router(router), _max_req_body_heap(max_request_body_heap_size), _handle(hdl) {}

    connection::behavior_type connection::make_behavior() {
        this->configure_read(_handle, caf::io::receive_policy::at_most(1024));
        this->set_exit_handler([this](const caf::exit_msg &msg) { handle_exit(msg); });
        this->set_down_handler([this](const caf::down_msg &msg) { handle_down(msg); });
        return {
            [this](const caf::io::new_data_msg &ndm) { http_handler::handle_new_data_msg(ndm); },
            [this](const caf::io::connection_closed_msg &ccm) { handle_connection_closed(ccm); },
            [this](const net::response &resp) { handle_response(resp); }
        };
    }

    void connection::handle_connection_closed(const caf::io::connection_closed_msg &ccm) {
        this->quit();
    }

    void connection::handle_response(const net::response &resp) {
        std::ostringstream oss;
        resp.write(oss);
        const auto resp_str = oss.str(); //NOTE IMPROVEMENT: let's get rid of this extra copy somehow.

        LOG_TRACE_AOUT("connection") << "Writing back response: " << resp_str << std::endl;
        this->write(_handle, resp_str.length(), resp_str.c_str());
        this->flush(_handle);
        this->quit();
    }

    void connection::handle_exit(const caf::exit_msg &msg) {
        this->quit();
    }

    void connection::handle_down(const caf::down_msg &msg) {
        this->quit();
    }

    void connection::on_message_begin() {
        //Nothing. We reset the request in on_message_complete
    }

    void connection::on_message_complete() {
        net::request req;
        std::swap(req, _current_request); //Reset for another request potentially
        // Dispatch it off to the relevant actor handler.

        //TODO parameterize request timeout
        this->request(_router, std::chrono::seconds(29), req).then(
            [this](const net::response &resp) { //TODO: handle timeout
                handle_response(resp);
            });

    }

    void connection::on_url(const std::string &url) {
        _current_request.url(url);
    }

    void connection::on_header(const std::string &key, const std::string &value) {
        _current_request.set_header(key, value);
    }

    void connection::on_body(const std::string_view &body_data) {
        if (!_current_request.is_filebody()) {
            //First, check if the length is long enough to require us to convert.
            if (body_data.length() > _max_req_body_heap) {
                on_convert_request_body_to_file();
                // Continue below, to write to file!
                //...   \/
            } else {
                // We should just append to the body
                auto p_b = reinterpret_cast<const std::byte *>(body_data.data());
                _current_request.body().insert(std::begin(_current_request.body()), p_b, p_b + body_data.size());
            }
        }
        // Either we're a file reference, or we've just become one: write to the file!
        std::fwrite(body_data.data(), sizeof(char), body_data.size(), _current_request.filebody());
    }

    void connection::on_convert_request_body_to_file() {
        _current_request.filebody(std::tmpfile());
        std::fwrite(_current_request.body().data(), sizeof(std::byte), _current_request.body().size(),
                    _current_request.filebody());
        // Completely free up that body
        _current_request.body() = net::message::body_data_t();
    }


} //ns as::net
