#include "caf-net/http-server.hpp"

namespace as::net {


    http_server::http_server(caf::actor_system &system, const http_server::config &cfg) : _system(system), _cfg(cfg) {
        _router = _system.spawn<net::router>();
    }

    http_server::~http_server() {
        stop();
    }

    void http_server::run() {
        _acceptor = spawn_server();
        _system.await_all_actors_done(); //TODO it would make more sense to become a worker thread... not sure how to sort that though..
    }

    void http_server::stop() {
        caf::anon_send_exit(_acceptor, caf::exit_reason::kill);
        caf::anon_send_exit(_router, caf::exit_reason::kill);
    }

    caf::strong_actor_ptr http_server::spawn_server() {
        // True: reuse addr on the port.
        auto eptr = _system.middleman().backend().new_tcp_doorman(_cfg.port, nullptr, true);
        if(!eptr) {
            throw std::runtime_error(_system.render(eptr.error()));
        }
        caf::actor_config actor_cfg{&_system.middleman().backend()};
        return caf::actor_cast<caf::strong_actor_ptr >(
            _system.spawn_class<net::acceptor, caf::no_spawn_options>(actor_cfg, std::move(*eptr), _router, MiB2b(_cfg.max_body_heap_size)));
    }
} //ns as::net
