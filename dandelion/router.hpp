#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <optional>
#include <iostream>
#include <tuple>

#include "caf/all.hpp"
#include "r3.h"

#include "net-typeids.hpp"
#include "request.hpp"
#include "response.hpp"

namespace as::net {

    using router_actor = caf::typed_actor<
            caf::reacts_to<add_route_atom, std::string, caf::strong_actor_ptr>,
            caf::reacts_to<add_route_atom, method_t, std::string, caf::strong_actor_ptr>,
            caf::replies_to<request>::with<response>
            >;

class router : public router_actor::base {

    struct route_match {
        std::unordered_map<std::string, std::string> matched_slugs;
        caf::strong_actor_ptr target_actor;
    };

public:

    using r3node = R3Node;
    using r3route = R3Route;

    router(caf::actor_config &cfg, const std::string &server_name = "http-server");

    virtual ~router();

    behavior_type make_behavior() override;

protected:

    //Actor behavior
    void handle_add_route(add_route_atom, const std::string &route, caf::strong_actor_ptr target, method_t method = method_t::GET);

    //Actor behavior
    auto handle_request(class request req)  -> caf::result<net::response>;

private:

    struct match_entry_deleter {
        void operator() (match_entry *p) {
            match_entry_free(p);
        }
    };

    struct r3_tree_deleter {
        void operator() (r3node *p) {
            r3_tree_free(p);
        }
    };

    using target_holder = std::unique_ptr<caf::strong_actor_ptr>;
    using route_def = std::tuple<net::method_t, target_holder>;

    //! Parses url parameters and reinserts them into the request
    void parse_url_params(net::request &req);

    void insert_route(const std::string &path_template, method_t method, caf::strong_actor_ptr target);

    void compile_tree();

    std::optional<route_match> match_route(const std::string &path, method_t method = method_t::GET);

    std::string _server_name;
    std::unique_ptr<r3node, r3_tree_deleter> _r3_tree;
    std::unordered_map<std::string, route_def> _route_definitions;



};

} //ns as::net
