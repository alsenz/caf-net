#pragma once

#include "caf/all.hpp"
#include "caf/io/all.hpp"

#include "router.hpp"
#include "connection.hpp"

namespace as::net {

    typedef caf::io::typed_broker<
            caf::reacts_to<caf::io::new_connection_msg>,
            caf::reacts_to<caf::io::acceptor_closed_msg>
    > acceptor_broker;

class acceptor : public acceptor_broker {
    public:

        acceptor(caf::actor_config &cfg, caf::io::doorman_ptr &&doorman_ptr, net::router_actor &router, std::size_t max_request_body_heap_size = MiB2b(16));

        behavior_type make_behavior() override;

    protected:

        void handle_new_connection(const caf::io::new_connection_msg &ncm);

        void handle_acceptor_closed(const caf::io::acceptor_closed_msg &acm);

        void handle_exit(const caf::exit_msg &msg);

    private:

        net::router_actor &_router;
        std::size_t _max_req_body_heap_sz;
    };


}
