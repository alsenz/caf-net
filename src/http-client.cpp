#include "http-client.hpp"

namespace as::net {


    auto spawn_client(caf::actor_system &home_system,
                            const http_client::config &cfg) -> caf::infer_handle_from_class_t<http_client> {
        return home_system.spawn<http_client, caf::no_spawn_options>(cfg);
    }

    void stop_client(client_actor &client_worker) {
        caf::anon_send_exit(client_worker, caf::exit_reason::user_shutdown);
    }


    http_client::http_client(caf::actor_config &actor_cfg, const http_client::config &cfg) : client_actor::base(actor_cfg), _cfg(cfg) {}

    http_client::behavior_type http_client::make_behavior() {
        this->set_exit_handler([this](const caf::exit_msg &msg) { handle_exit(msg); });
        return {
            [this](const net::request &req) { return handle_request(req); }
        };
    }

    void http_client::handle_exit(const caf::exit_msg &msg) {
        this->quit();
    }
} //ns as::net