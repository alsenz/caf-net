#pragma once

#include <chrono>

#include "caf/all.hpp"

#include "common/config.hpp"
#include "common/logger.hpp"

#include "request.hpp"
#include "response.hpp"
#include "client-connection.hpp"
#include "connection.hpp"

namespace as::net {

    using client_actor = caf::typed_actor<
            caf::replies_to<net::request>::with<net::response>
    >;

    class http_client : public client_actor::base {
    public:

        struct config {

            std::size_t max_response_body_head_mibs = MiB2b(16);
            std::string user_agent = "C++ Application Client";

            config() {}

            void configure(as::config_group &group) {
                group.add(user_agent, "controller-list-agent", "controller-list agent of HTTP client. Must be short, descriptive, and contain contact address");
                group.add(max_response_body_head_mibs, "max-body-heap-size", "the maximum size in memory of a response body, in MiB, before it is stored on disk");
            }

            constexpr const char *group_name() const {
                return "http-client";
            }
        };

        http_client(caf::actor_config &actor_cfg, const config &cfg);

        behavior_type make_behavior() override;

    protected:

        //TODO we should do handle_request for get_request and all that as well!

        auto handle_request(const net::request &req) -> caf::result<net::response> {
            caf::actor_system &sys = this->system();

            LOG_DEBUG_AOUT("test-cat") << "Opening connection to: "
                                       << req.host() << ":" << req.port() << std::endl;

            auto eptr = sys.middleman().backend().new_tcp_scribe(req.host(), req.port());
            if (!eptr) {
                const auto msg = sys.render(eptr.error());
                return net::response(status_t::network_connect_timeout_error, {{"error",  msg}});
            }

            //TODO where does the handle come from then???
            auto ptr = std::move(*eptr);
            caf::actor_config conn_cfg{&sys.middleman().backend()};
            auto client_worker = sys.spawn_class<client_connection, caf::no_spawn_options>(conn_cfg, ptr->hdl(), _cfg.user_agent, _cfg.max_response_body_head_mibs);

            auto client_worker_ptr = static_cast<caf::io::abstract_broker *>(caf::actor_cast<caf::abstract_actor *>(client_worker));
            client_worker_ptr->add_scribe(std::move(ptr));
            client_worker_ptr->monitor(this);

            //TODO actually this may work - it could just be a problem elsewhere...?
            //Note-- for some reason, delegate isn't working here... it's never picking up the response message... let's check if it's here or somewhere else...
            return this->delegate(client_worker, req);
            /*auto rp = make_response_promise<net::response>();
            this->request(client_worker, std::chrono::seconds(30), req).then([rp](const net::response resp) mutable {
                rp.deliver(resp);
            });
            return rp;*/
        }

        void handle_exit(const caf::exit_msg &msg);

    private:

        config _cfg;

    };


    auto spawn_client(caf::actor_system &home_system, const http_client::config &cfg)
    -> caf::infer_handle_from_class_t<http_client>;

    void stop_client(client_actor &client_worker);

}
