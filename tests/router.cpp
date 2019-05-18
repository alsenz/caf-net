#include <iostream>

#include "gtest/gtest.h"

#include "src/caf-net/router.hpp"

#include "caf/all.hpp"
#include "caf/io/all.hpp"


using simple_rest_behavior = caf::typed_actor<caf::replies_to<as::net::request>::with<as::net::response>>;

simple_rest_behavior::behavior_type simple_endpoint_behavior(simple_rest_behavior::base *self) {
    return {
            [](as::net::request &req) {
                auto resp = as::net::response(as::net::status_t::ok);
                resp.append_body("testing 1234");
                return resp;
            }
    };
}

TEST(RouterTests, TestSimpleRouting) {

    caf::actor_system_config cfg;
    caf::actor_system system{cfg};
    auto router = system.spawn<as::net::router>();
    auto router_fn = caf::make_function_view(router);
    auto simple_actor = system.spawn(simple_endpoint_behavior);
    auto simple_actor_ptr = caf::actor_cast<caf::strong_actor_ptr>(simple_actor);

    //Syncronously send an 'add'
    router_fn(as::net::add_route_atom::value, "/widgets/bobbins", simple_actor_ptr);

    auto req1 = as::net::request("http://www.example.com/widgets/bobbins");
    auto req2 = as::net::request("http://www.example.com/wodgets/bobbins");
    auto req3 = as::net::request("http://www.example.com/widgets/snark");

    EXPECT_EQ(as::net::status_t::ok, router_fn(req1).value().status());
    EXPECT_EQ("testing 1234", router_fn(req1).value().body_view());
    EXPECT_EQ(as::net::status_t::not_found, router_fn(req2).value().status());
    EXPECT_EQ(as::net::status_t::not_found, router_fn(req3).value().status());

    //Add a route for snark
    router_fn(as::net::add_route_atom::value, "/widgets/snark", simple_actor_ptr);

    EXPECT_EQ(as::net::status_t::ok, router_fn(req3).value().status());
    //And this one is still found
    EXPECT_EQ(as::net::status_t::ok, router_fn(req1).value().status());
    //But this one isn't
    EXPECT_EQ(as::net::status_t::not_found, router_fn(req2).value().status());
}

simple_rest_behavior::behavior_type param_echo_endpoint(simple_rest_behavior::base *self) {
    return {
            [](as::net::request &req) {
                auto resp = as::net::response(as::net::status_t::ok);
                const auto req_param = req.param("name");
                if(!req_param) {
                    resp.append_body("no-param");
                } else {
                    resp.append_body(req_param.value());
                }
                return resp;
            }
    };
}

TEST(RouterTests, TestParameterRouting) {

    caf::actor_system_config cfg;
    caf::actor_system system{cfg};
    auto router = system.spawn<as::net::router>();
    auto router_fn = caf::make_function_view(router);
    auto simple_endpoint_ptr = caf::actor_cast<caf::strong_actor_ptr >(system.spawn(simple_endpoint_behavior));
    auto echo_param_endpoint_ptr = caf::actor_cast<caf::strong_actor_ptr >(system.spawn(param_echo_endpoint));

    router_fn(as::net::add_route_atom::value, "/widgets/list", simple_endpoint_ptr);
    router_fn(as::net::add_route_atom::value, "/widgets/{name}/info", echo_param_endpoint_ptr);
    router_fn(as::net::add_route_atom::value, "/widgets/info", echo_param_endpoint_ptr);

    auto req1 = as::net::request("http://www.example.com/widgets/list");
    auto req2 = as::net::request("http://www.example.com/widgets/kerbobbins/info");
    auto req3 = as::net::request("http://www.example.com/widgets/info?name=in-query-name");
    auto req4 = as::net::request("http://www.example.com/widgets/info");

    EXPECT_EQ("testing 1234", router_fn(req1).value().body_view());
    EXPECT_EQ("kerbobbins", router_fn(req2).value().body_view());
    EXPECT_EQ("in-query-name", router_fn(req3).value().body_view());
    EXPECT_EQ("no-param", router_fn(req4).value().body_view());
}


