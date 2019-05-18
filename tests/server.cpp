#include <future>
#include <chrono>
#include <thread>

#include "gtest/gtest.h"

#include "caf/all.hpp"

#include "caf-net/http-server.hpp"

TEST(ServerTest, ServerStartStop) {

    caf::actor_system_config cfg;
    cfg.load<caf::io::middleman>();
    caf::actor_system system{cfg};

    auto server = as::net::http_server(system);

    std::async(std::launch::async, [&server]() {
        server.run();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    server.stop();
}
