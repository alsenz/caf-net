#include "acceptor.hpp"

namespace as::net {


    acceptor::acceptor(caf::actor_config &cfg, caf::io::doorman_ptr &&doorman_ptr, net::router_actor &router,
                            std::size_t max_request_body_heap_size)
        : acceptor_broker(cfg), _router(router), _max_req_body_heap_sz(max_request_body_heap_size) {
        add_doorman(std::move(doorman_ptr));
    }

    acceptor::behavior_type acceptor::make_behavior() {
        this->set_exit_handler([this](const caf::exit_msg &msg) { handle_exit(msg); });
        return {
            [this](const caf::io::new_connection_msg &ncm) { handle_new_connection(ncm); },
            [this](const caf::io::acceptor_closed_msg &acm) { handle_acceptor_closed(acm); }
        };
    }

    void acceptor::handle_new_connection(const caf::io::new_connection_msg &ncm) {
        auto p_scribe = this->take(ncm.handle);
        caf::actor_config conn_config{this->context()};
        auto conn_worker = this->system().spawn_class<connection, caf::no_spawn_options>(conn_config, p_scribe->hdl(), _router, _max_req_body_heap_sz);
        auto conn_worker_impl = static_cast<connection *>(caf::actor_cast<abstract_actor*>(conn_worker));
        conn_worker_impl->move_scribe(std::move(p_scribe));
        conn_worker_impl->monitor(this);
    }

    void acceptor::handle_acceptor_closed(const caf::io::acceptor_closed_msg &acm) {
        this->quit();
    }

    void acceptor::handle_exit(const caf::exit_msg &msg) {
        this->quit();
    }


} //ns as::net
