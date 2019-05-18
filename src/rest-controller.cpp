#include "caf-net/rest-controller.hpp"

namespace as::net {


    rest_controller::rest_controller(caf::actor_config &cfg) : rest_controller_actor::base(cfg) {}

    rest_controller::behavior_type rest_controller::make_behavior() {
        return {
            [this](const net::get_request &req) { return handle_get(req); },
            [this](const net::post_request &req) { return handle_post(req); },
            [this](const net::head_request &req) { return handle_head(req); },
            [this](const net::put_request &req) { return handle_put(req); },
            [this](const net::delete_request &req) { return handle_delete(req); },
            [this](const net::options_request &req) { return handle_options(req); },
        };
    }

    rest_controller::response_result rest_controller::handle_get(const net::get_request &req) {
        return net::response(status_t::method_not_allowed);
    }

    rest_controller::response_result rest_controller::handle_post(const net::post_request &req) {
        return net::response(status_t::method_not_allowed);
    }

    rest_controller::response_result rest_controller::handle_head(const net::head_request &req) {
        return net::response(status_t::method_not_allowed);
    }

    rest_controller::response_result rest_controller::handle_put(const net::put_request &req) {
        return net::response(status_t::method_not_allowed);
    }

    rest_controller::response_result rest_controller::handle_delete(const net::delete_request &req) {
        return net::response(status_t::method_not_allowed);
    }

    rest_controller::response_result rest_controller::handle_options(const net::options_request &req) {
        return net::response(status_t::method_not_allowed);
    }
}
