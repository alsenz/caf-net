#include <future>
#include <chrono>
#include <thread>

#include "gtest/gtest.h"

#include "caf/all.hpp"

#include "dandelion/http-server.hpp"

TEST(ServerTest, ServerStartStop) {

    caf::actor_system_config cfg;
    cfg.load<caf::io::middleman>();
    caf::exec_main_init_meta_objects<caf::id_block::caf_net>();
    caf::core::init_global_meta_objects();
    caf::io::middleman::init_global_meta_objects();
    caf::actor_system system{cfg};

    auto server = as::net::http_server(system);

    //TODO this test doesn't work at all, just hangs. Need to resolve this!

    /*
    std::async(std::launch::async, [&server]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        server.stop();
    });

    server.run();
     */
}
