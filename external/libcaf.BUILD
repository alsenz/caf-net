#TODO would be good to split this up a bit to get caf to build under bazel!

genrule(
    name = "libcaf_genrule",
    srcs = glob(["**/*"]),
    outs = ["core_export.hpp", "build_config.hpp", "io_export.hpp", "openssl_export.hpp"],
    cmd = "cmake external/libcaf -DBUILD_SHARED_LIBS=OFF -DCAF_LOG_LEVEL=INFO && cp caf/detail/*.hpp $(@D)"
)

cc_library(
    name = "libcaf_build_config",
    hdrs = ["build_config.hpp", "core_export.hpp", "io_export.hpp", "openssl_export.hpp"],
    #strip_include_prefix = "external/libcaf"
    include_prefix = "caf/detail"
)


cc_library(
    name = "libcaf_core",
    srcs = [  "libcaf_core/src/abstract_actor.cpp",
              "libcaf_core/src/abstract_channel.cpp",
              "libcaf_core/src/abstract_group.cpp",
              "libcaf_core/src/actor.cpp",
              "libcaf_core/src/actor_addr.cpp",
              "libcaf_core/src/actor_clock.cpp",
              "libcaf_core/src/actor_companion.cpp",
              "libcaf_core/src/actor_config.cpp",
              "libcaf_core/src/actor_control_block.cpp",
              "libcaf_core/src/actor_ostream.cpp",
              "libcaf_core/src/actor_pool.cpp",
              "libcaf_core/src/actor_profiler.cpp",
              "libcaf_core/src/actor_proxy.cpp",
              "libcaf_core/src/actor_registry.cpp",
              "libcaf_core/src/actor_system.cpp",
              "libcaf_core/src/actor_system_config.cpp",
              "libcaf_core/src/attachable.cpp",
              "libcaf_core/src/behavior.cpp",
              "libcaf_core/src/binary_deserializer.cpp",
              "libcaf_core/src/binary_serializer.cpp",
              "libcaf_core/src/blocking_actor.cpp",
              "libcaf_core/src/complexity_based_credit_controller.cpp",
              "libcaf_core/src/config_option.cpp",
              "libcaf_core/src/config_option_adder.cpp",
              "libcaf_core/src/config_option_set.cpp",
              "libcaf_core/src/config_value.cpp",
              "libcaf_core/src/credit_controller.cpp",
              "libcaf_core/src/decorator/sequencer.cpp",
              "libcaf_core/src/default_attachable.cpp",
              "libcaf_core/src/deserializer.cpp",
              "libcaf_core/src/detail/abstract_worker.cpp",
              "libcaf_core/src/detail/abstract_worker_hub.cpp",
              "libcaf_core/src/detail/append_percent_encoded.cpp",
              "libcaf_core/src/detail/behavior_impl.cpp",
              "libcaf_core/src/detail/behavior_stack.cpp",
              "libcaf_core/src/detail/blocking_behavior.cpp",
              "libcaf_core/src/detail/get_mac_addresses.cpp",
              "libcaf_core/src/detail/get_process_id.cpp",
              "libcaf_core/src/detail/get_root_uuid.cpp",
              "libcaf_core/src/detail/ini_consumer.cpp",
              "libcaf_core/src/detail/invoke_result_visitor.cpp",
              "libcaf_core/src/detail/message_builder_element.cpp",
              "libcaf_core/src/detail/message_data.cpp",
              "libcaf_core/src/detail/meta_object.cpp",
              "libcaf_core/src/detail/parse.cpp",
              "libcaf_core/src/detail/parser/chars.cpp",
              "libcaf_core/src/detail/pretty_type_name.cpp",
              "libcaf_core/src/detail/private_thread.cpp",
              "libcaf_core/src/detail/ripemd_160.cpp",
              "libcaf_core/src/detail/serialized_size.cpp",
              "libcaf_core/src/detail/set_thread_name.cpp",
              "libcaf_core/src/detail/shared_spinlock.cpp",
              "libcaf_core/src/detail/simple_actor_clock.cpp",
              "libcaf_core/src/detail/stringification_inspector.cpp",
              "libcaf_core/src/detail/sync_request_bouncer.cpp",
              "libcaf_core/src/detail/test_actor_clock.cpp",
              "libcaf_core/src/detail/thread_safe_actor_clock.cpp",
              "libcaf_core/src/detail/tick_emitter.cpp",
              "libcaf_core/src/detail/type_id_list_builder.cpp",
              "libcaf_core/src/detail/uri_impl.cpp",
              "libcaf_core/src/downstream_manager.cpp",
              "libcaf_core/src/downstream_manager_base.cpp",
              "libcaf_core/src/error.cpp",
              "libcaf_core/src/event_based_actor.cpp",
              "libcaf_core/src/execution_unit.cpp",
              "libcaf_core/src/exit_reason_strings.cpp",
              "libcaf_core/src/forwarding_actor_proxy.cpp",
              "libcaf_core/src/group.cpp",
              "libcaf_core/src/group_manager.cpp",
              "libcaf_core/src/group_module.cpp",
              "libcaf_core/src/inbound_path.cpp",
              "libcaf_core/src/init_global_meta_objects.cpp",
              "libcaf_core/src/intrusive/inbox_result_strings.cpp",
              "libcaf_core/src/intrusive/task_result_strings.cpp",
              "libcaf_core/src/invoke_msg_result_strings.cpp",
              "libcaf_core/src/ipv4_address.cpp",
              "libcaf_core/src/ipv4_endpoint.cpp",
              "libcaf_core/src/ipv4_subnet.cpp",
              "libcaf_core/src/ipv6_address.cpp",
              "libcaf_core/src/ipv6_endpoint.cpp",
              "libcaf_core/src/ipv6_subnet.cpp",
              "libcaf_core/src/local_actor.cpp",
              "libcaf_core/src/logger.cpp",
              "libcaf_core/src/mailbox_element.cpp",
              "libcaf_core/src/make_config_option.cpp",
              "libcaf_core/src/memory_managed.cpp",
              "libcaf_core/src/message.cpp",
              "libcaf_core/src/message_builder.cpp",
              "libcaf_core/src/message_handler.cpp",
              "libcaf_core/src/message_priority_strings.cpp",
              "libcaf_core/src/monitorable_actor.cpp",
              "libcaf_core/src/node_id.cpp",
              "libcaf_core/src/outbound_path.cpp",
              "libcaf_core/src/pec_strings.cpp",
              "libcaf_core/src/policy/downstream_messages.cpp",
              "libcaf_core/src/policy/unprofiled.cpp",
              "libcaf_core/src/policy/work_sharing.cpp",
              "libcaf_core/src/policy/work_stealing.cpp",
              "libcaf_core/src/proxy_registry.cpp",
              "libcaf_core/src/raise_error.cpp",
              "libcaf_core/src/ref_counted.cpp",
              "libcaf_core/src/replies_to.cpp",
              "libcaf_core/src/response_promise.cpp",
              "libcaf_core/src/resumable.cpp",
              "libcaf_core/src/scheduled_actor.cpp",
              "libcaf_core/src/scheduler/abstract_coordinator.cpp",
              "libcaf_core/src/scheduler/test_coordinator.cpp",
              "libcaf_core/src/scoped_actor.cpp",
              "libcaf_core/src/scoped_execution_unit.cpp",
              "libcaf_core/src/sec_strings.cpp",
              "libcaf_core/src/serializer.cpp",
              "libcaf_core/src/settings.cpp",
              "libcaf_core/src/size_based_credit_controller.cpp",
              "libcaf_core/src/skip.cpp",
              "libcaf_core/src/stream_aborter.cpp",
              "libcaf_core/src/stream_manager.cpp",
              "libcaf_core/src/stream_priority_strings.cpp",
              "libcaf_core/src/string_algorithms.cpp",
              "libcaf_core/src/string_view.cpp",
              "libcaf_core/src/term.cpp",
              "libcaf_core/src/test_credit_controller.cpp",
              "libcaf_core/src/thread_hook.cpp",
              "libcaf_core/src/timestamp.cpp",
              "libcaf_core/src/tracing_data.cpp",
              "libcaf_core/src/tracing_data_factory.cpp",
              "libcaf_core/src/type_id_list.cpp",
              "libcaf_core/src/uri.cpp",
              "libcaf_core/src/uri_builder.cpp",
              "libcaf_core/src/uuid.cpp"],
    hdrs = glob(["libcaf_core/caf/**/*.hpp"]),
    copts = ["-std=c++17", "-Wextra", "-Wall", "-pedantic", "-ftemplate-depth=512", "-ftemplate-backtrace-limit=0", "-Wno-missing-field-initializers", "-pthread", "-fPIC"],
    strip_include_prefix = "libcaf_core",
    visibility = ["//visibility:public"],
    deps = [":libcaf_build_config"]
)

cc_library(
    name = "libcaf_io",
    srcs = [  "libcaf_io/src/detail/socket_guard.cpp",
              "libcaf_io/src/io/abstract_broker.cpp",
              "libcaf_io/src/io/basp/header.cpp",
              "libcaf_io/src/io/basp/instance.cpp",
              "libcaf_io/src/io/basp/message_queue.cpp",
              "libcaf_io/src/io/basp/message_type_strings.cpp",
              "libcaf_io/src/io/basp/routing_table.cpp",
              "libcaf_io/src/io/basp/worker.cpp",
              "libcaf_io/src/io/basp_broker.cpp",
              "libcaf_io/src/io/broker.cpp",
              "libcaf_io/src/io/connection_helper.cpp",
              "libcaf_io/src/io/datagram_servant.cpp",
              "libcaf_io/src/io/doorman.cpp",
              "libcaf_io/src/io/middleman.cpp",
              "libcaf_io/src/io/middleman_actor.cpp",
              "libcaf_io/src/io/middleman_actor_impl.cpp",
              "libcaf_io/src/io/network/acceptor.cpp",
              "libcaf_io/src/io/network/acceptor_manager.cpp",
              "libcaf_io/src/io/network/datagram_handler.cpp",
              "libcaf_io/src/io/network/datagram_manager.cpp",
              "libcaf_io/src/io/network/datagram_servant_impl.cpp",
              "libcaf_io/src/io/network/default_multiplexer.cpp",
              "libcaf_io/src/io/network/doorman_impl.cpp",
              "libcaf_io/src/io/network/event_handler.cpp",
              "libcaf_io/src/io/network/interfaces.cpp",
              "libcaf_io/src/io/network/ip_endpoint.cpp",
              "libcaf_io/src/io/network/manager.cpp",
              "libcaf_io/src/io/network/multiplexer.cpp",
              "libcaf_io/src/io/network/native_socket.cpp",
              "libcaf_io/src/io/network/operation_strings.cpp",
              "libcaf_io/src/io/network/pipe_reader.cpp",
              "libcaf_io/src/io/network/protocol.cpp",
              "libcaf_io/src/io/network/receive_buffer.cpp",
              "libcaf_io/src/io/network/scribe_impl.cpp",
              "libcaf_io/src/io/network/stream.cpp",
              "libcaf_io/src/io/network/stream_manager.cpp",
              "libcaf_io/src/io/network/test_multiplexer.cpp",
              "libcaf_io/src/io/scribe.cpp",
              "libcaf_io/src/policy/tcp.cpp",
              "libcaf_io/src/policy/udp.cpp"],
    hdrs = glob(["libcaf_io/caf/**/*.hpp"]),
    strip_include_prefix = "libcaf_io",
    visibility = ["//visibility:public"],
    copts = ["-std=c++17", "-Wextra", "-Wall", "-pedantic", "-ftemplate-depth=512", "-ftemplate-backtrace-limit=0", "-Wno-missing-field-initializers", "-pthread", "-fPIC"],
    deps = [":libcaf_build_config", ":libcaf_core"]
)

cc_library(
    name = "libcaf",
    visibility = ["//visibility:public"],
    deps = [":libcaf_io", ":libcaf_core"]
)
