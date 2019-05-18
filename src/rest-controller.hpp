#pragma once

#include "caf/all.hpp"

#include "request.hpp"
#include "response.hpp"


namespace as::net {

    // This is just a helper class for people who are likely to want to write REST controller-style classes to handle requests.
    // It is quite acceptable not to use this class, and just to spin up actors as usual with your own inheritance hierarchy, as per the examples.

    using rest_controller_actor = caf::typed_actor<
    caf::replies_to<net::get_request>::with<net::response>,
    caf::replies_to<net::post_request>::with<net::response>,
            caf::replies_to<net::head_request>::with<net::response>,
            caf::replies_to<net::put_request>::with<net::response>,
            caf::replies_to<net::delete_request>::with<net::response>,
            caf::replies_to<net::options_request>::with<net::response>
    >;

    class rest_controller : public rest_controller_actor::base {

    public:

        using response_result = caf::result<net::response>;

        rest_controller(caf::actor_config &cfg);

        behavior_type make_behavior() override;

    protected:

        virtual response_result handle_get(const net::get_request &req);

        virtual response_result handle_post(const net::post_request &req);

        virtual response_result handle_head(const net::head_request &req);

        virtual response_result handle_put(const net::put_request &req);

        virtual caf::result<net::response> handle_delete(const net::delete_request &req);

        virtual caf::result<net::response> handle_options(const net::options_request &req);

    };

} //ns as::net