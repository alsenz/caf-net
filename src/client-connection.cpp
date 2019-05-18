#include "caf-net/client-connection.hpp"

namespace as::net {

    client_connection::client_connection(caf::actor_config &cfg, const caf::io::connection_handle &hdl,
                                         const std::string &user_agent, size_t max_response_body_heap)
        : client_connection_broker(cfg), http_handler(mode::response), _handle(hdl), _user_agent(user_agent), _max_response_body_heap(max_response_body_heap), _rp() {}

    client_connection::behavior_type client_connection::make_behavior() {
        this->configure_read(_handle, caf::io::receive_policy::at_most(1024));
        this->set_exit_handler([this](const caf::exit_msg &msg) { handle_exit(msg); });
        this->set_down_handler([this](const caf::down_msg &msg) { handle_down(msg); });
        return {
            [this](const caf::io::new_data_msg &ndm) { http_handler::handle_new_data_msg(ndm); },
            [this](const caf::io::connection_closed_msg &ccm) { handle_connection_closed(ccm); },
            [this](const net::request &req) { return handle_request(req); }
        };
    }

    void client_connection::handle_exit(const caf::exit_msg &msg) {
            this->quit();
    }

    void client_connection::handle_down(const caf::down_msg &msg) {
            this->quit();
    }

    void client_connection::handle_connection_closed(const caf::io::connection_closed_msg &ccm) {
            _current_response = net::response(status_t::connection_closed_without_response);
            dispatch_current_response();
            this->quit();
    }

    auto client_connection::handle_request(const net::request &req) -> caf::result<net::response> {
        LOG_DEBUG_A("connection", "Handling request");
        std::ostringstream oss;
        req.write(oss, "HTTP/1.1", _user_agent);
        const std::string req_data = oss.str();
        LOG_TRACE_AOUT("connection") << "Writing request: " << req_data << std::endl;
        this->write(_handle, req_data.size(), req_data.data());
        this->flush(_handle);
        _rp = make_response_promise<net::response>();
        return _rp;
    }

    void client_connection::on_message_begin() {
        // Nothing to do
    }

    void client_connection::on_message_complete() {
        LOG_DEBUG_A("connection", "Message complete");
        //TODO we need to check for redirects! //TODO TODO phew what a loada work!
        dispatch_current_response();
    }

    void client_connection::on_status(net::status_t status) {
        LOG_DEBUG_AOUT("connection") << "Got status! " << to_string(status) << std::endl;
        _current_response.status(status);
    }

    void client_connection::on_header(const std::string &key, const std::string &value) {
        _current_response.set_header(key, value);
    }

    void client_connection::on_body(const std::string_view &body_data) {
        LOG_TRACE_A("connection", "Body received.");
        if (!_current_response.is_filebody()) {
            //First, check if the length is long enough to require us to convert.
            if (body_data.length() > _max_response_body_heap) {
                on_convert_request_body_to_file();
                // Continue below, to write to file!
                //...   \/
            } else {
                // We should just append to the body
                auto p_b = reinterpret_cast<const std::byte *>(body_data.data());
                _current_response.body().insert(std::begin(_current_response.body()), p_b, p_b + body_data.size());
                return;
            }
        }
        // Either we're a file reference, or we've just become one: write to the file!
        std::fwrite(body_data.data(), sizeof(char), body_data.size(), _current_response.filebody());
    }

    void client_connection::on_convert_request_body_to_file() {
        _current_response.filebody(std::tmpfile());
        std::fwrite(_current_response.body().data(), sizeof(std::byte), _current_response.body().size(),
                    _current_response.filebody());
        // Completely free up that body
        _current_response.body() = net::message::body_data_t();
    }

    void client_connection::dispatch_current_response() {
        _rp.deliver(_current_response); // This should be run on the same worker as the actor i.e. not from a concurrent thread.
    }


} //ns as::net
