#include "router.hpp"

namespace as::net {

    router::router(caf::actor_config &cfg, const std::string &server_name) : router_actor::base(cfg), _server_name(server_name), _r3_tree(nullptr) {}

    router::~router() {}

    router_actor::behavior_type  router::make_behavior() {
        return {
                [this](add_route_atom atom, const std::string &route, caf::strong_actor_ptr sap) { handle_add_route(atom, route, sap); },
                [this](add_route_atom atom, request::method_t method, const std::string &route, caf::strong_actor_ptr sap) { handle_add_route(atom, route, sap, method); },
                [this](class request req) { return handle_request(req); }
        };
    }

    void router::handle_add_route(add_route_atom, const std::string &route, caf::strong_actor_ptr target,
                                  request::method_t method) {
        insert_route(route, method, target);
    }

    auto router::handle_request(class request req) -> caf::result<net::response> {
        if(!_r3_tree) {
            net::response resp = net::response(status_t::internal_server_error);
            resp.make_stock_response(_server_name, "No Routes Configured");
            return resp;
        }
        auto rp = make_response_promise<net::response>();
        auto matched = match_route(req.path(), req.method());
        parse_url_params(req);
        if(!matched) {
            net::response resp = net::response(status_t::not_found);
            resp.make_stock_response(_server_name);
            rp.deliver(resp);

        } else {
            // Write the match params into the request
            // Nb: this comes *second* to prevent a path-based-auth bypass attack
            auto &slug_map = matched.value().matched_slugs;
            req.params().insert(std::make_move_iterator(std::begin(slug_map)), std::make_move_iterator(std::end(slug_map)));

            auto target_actor = caf::actor_cast<caf::actor>(matched.value().target_actor);
            auto cb = [rp, this](net::response resp) mutable {
                if(resp.no_response_body()) {
                    resp.make_stock_response(_server_name);
                }
                rp.deliver(resp);
            };

            auto final_error_fallback = [this, rp] (const caf::error &err) mutable {
                std::cout << "Warning: unhandled error: " << system().render(err) << std::endl;
                rp.deliver(net::response(net::status_t::internal_server_error));
            };

            //TODO make timeout configurable, and consistent with the timeout in the server...
            //TODO ensure that if we dispatch to something that can't handle it, we error nicely...
            switch(req.method()) {
                case net::request::method_t::GET:
                    this->request(target_actor, std::chrono::seconds(29), net::get_request(req)).then(cb, final_error_fallback);
                    break;
                case net::request::method_t::DELETE:
                    this->request(target_actor, std::chrono::seconds(29), net::delete_request(req)).then(cb, final_error_fallback);
                    break;
                case net::request::method_t::HEAD:
                    this->request(target_actor, std::chrono::seconds(29), net::head_request(req)).then(cb, final_error_fallback);
                    break;
                case net::request::method_t::OPTIONS:
                    this->request(target_actor, std::chrono::seconds(29), net::options_request(req)).then(cb, final_error_fallback);
                    break;
                case net::request::method_t::POST:
                    this->request(target_actor, std::chrono::seconds(29), net::post_request(req)).then(cb, final_error_fallback);
                    break;
                case net::request::method_t::PUT:
                    this->request(target_actor, std::chrono::seconds(29), net::put_request(req)).then(cb, final_error_fallback);
                    break;
            }

        }
        return rp;
    }

    void router::parse_url_params(net::request &req) {
        //NB: this will need to be reviewed as it will probably need to be quite performant.
        const auto &req_query = req.query();
        enum class state {writing_key, writing_value};
        state current_state = state::writing_key;
        std::string current_key;
        current_key.reserve(10);
        std::string current_value;
        current_value.reserve(10);
        for(const char c : req_query) {
            switch(c) {
                case '&':
                    req.params().insert(std::make_pair(current_key, current_value));
                    current_key.clear();
                    current_value.clear();
                    current_state = state::writing_key;
                    break;
                case '=':
                    current_state = state::writing_value;
                    break;
                default:
                    switch(current_state) {
                        case state::writing_key:
                            current_key.append(1, c);
                            break;
                        case state::writing_value:
                            current_value.append(1, c);
                            break;
                    }
            }
        }
        if(current_key.length()) { //Last one won't have an '&'.
            req.params().insert(std::make_pair(current_key, current_value));
        }
    }

    void router::insert_route(const std::string &path_template, request::method_t method, caf::strong_actor_ptr target) {
        target_holder holder(new caf::strong_actor_ptr(std::move(target))); //Create a copy ... little laborious but needed to work with the clib!
        _route_definitions.emplace(path_template, std::make_tuple(method, std::move(holder)));
        compile_tree();
    }

    void router::compile_tree() {
        _r3_tree.reset(r3_tree_create(_route_definitions.size()));
        for(const auto &def : _route_definitions) {
            //Need to make a new copy of the string for C lib.
            char *c_str_path = new char[def.first.length()];
            strncpy(c_str_path, def.first.c_str(), def.first.length());
            auto target_method = std::get<0>(def.second);
            auto &target_wrapper = std::get<1>(def.second);
            //We can just cast the enum because they're set up to be compatible
            r3_tree_insert_routel(_r3_tree.get(), static_cast<int>(target_method), c_str_path, def.first.length(), target_wrapper.get());
        }
        char *errstr;
        r3_tree_compile(_r3_tree.get(), &errstr);
        if(errstr!=nullptr) {
            std::stringstream ss;
            ss << "Unable in router to compile tree, due to: ";
            ss << errstr;
            throw std::runtime_error(ss.str());
        }
    }

    std::optional<router::route_match> router::match_route(const std::string &path, request::method_t method) {
        int r3_method = static_cast<int>(method); //This works because enum compatibility
        std::unique_ptr<match_entry, match_entry_deleter> match_entry_ptr(match_entry_createl(path.c_str(), path.length()));
        match_entry_ptr->request_method = r3_method;
        //Note: I think this is a ptr to something managed by the tree, so we don't need to delete it
        r3route *matched_route_ptr = r3_tree_match_route(_r3_tree.get(), match_entry_ptr.get());
        if(!matched_route_ptr || !matched_route_ptr->data) {
            return {};
        }
        if (match_entry_ptr->vars.tokens.size != match_entry_ptr->vars.slugs.size) {
            return {};
        }
        route_match response_match;
        //has base char* and len unsigned int
        for (unsigned int i = 0; i < match_entry_ptr->vars.tokens.size; i++) {
            auto param_key = std::string(match_entry_ptr->vars.slugs.entries[i].base, match_entry_ptr->vars.slugs.entries[i].len);
            auto value_key = std::string(match_entry_ptr->vars.tokens.entries[i].base, match_entry_ptr->vars.tokens.entries[i].len);
            response_match.matched_slugs.emplace(std::move(param_key), std::move(value_key));
        }
        //Copy the strong actor ptr in...
        response_match.target_actor = *(reinterpret_cast<caf::strong_actor_ptr  *>(matched_route_ptr->data));
        return response_match;
    }

} //ns as::net