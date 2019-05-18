#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <utility>

#include "caf/all.hpp"
#include "caf/io/all.hpp"

#include "src/common/config.hpp"

#include "router.hpp"
#include "acceptor.hpp"

namespace as::net {

    class http_server {
    public:

        struct config {
            config() {}
            std::string hostname = "localhost";
            unsigned int port = 8018;
            std::string name = "as-http-server";
            std::size_t max_body_heap_size = 16;

            void configure(as::config_group &group) {
                group.add(hostname, "hostname", "server hostname");
                group.add(port, "port", "server port");
                group.add(name, "name", "server name");
                group.add(max_body_heap_size, "max-body-heap-size", "the maximum size in memory of a request body, in MiB, before it is stored on disk");
            }

            constexpr const char *group_name() const {
                return "http-server";
            }
        };

        http_server(caf::actor_system &system, const http_server::config &cfg = http_server::config());

        virtual ~http_server();

        template <class C, caf::spawn_options Os = caf::no_spawn_options, class... Ts>
        auto spawn_on_route(const std::string &route, Ts&&... xs) -> caf::infer_handle_from_class_t<C> {
            auto worker = _system.spawn<C, Os, Ts...>(xs...);
            caf::anon_send(_router, net::add_route_atom::value, route, caf::actor_cast<caf::strong_actor_ptr>(worker));
            static_cast<router *>(caf::actor_cast<caf::abstract_actor *>(_router))->link_to(worker);
            return worker;
        }

        template <caf::spawn_options Os = caf::no_spawn_options, class F, class... Ts>
        auto spawn_on_route(F fun, const std::string &route, Ts&&... xs) -> caf::infer_handle_from_fun_t<F> {
            auto worker = _system.spawn<Os, F, Ts...>(fun, std::move(xs)...);
            caf::anon_send(_router, net::add_route_atom::value, route, caf::actor_cast<caf::strong_actor_ptr>(worker));
            static_cast<router *>(caf::actor_cast<caf::abstract_actor *>(_router))->link_to(worker);
            return worker;
        }

        void run();

        void stop();

    private:

        caf::strong_actor_ptr spawn_server();

        caf::actor_system &_system;
        config _cfg;

        router_actor _router;
        caf::strong_actor_ptr _acceptor;



    };

} //ns as::net
