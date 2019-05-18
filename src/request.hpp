#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <type_traits>
#include <cstdio>
#include <ostream>

#include "nlohmann/json.hpp"
#include "url.h"
#include "r3.h"

#include "caf-net/message.hpp"

namespace as::net {

    class request : public message {
    public:

        enum class method_t : int {GET = METHOD_GET, PUT = METHOD_PUT, POST = METHOD_POST, HEAD = METHOD_HEAD, DELETE = METHOD_DELETE, OPTIONS = METHOD_OPTIONS};

        request();

        request(const std::string &url, method_t method = method_t::GET);

        method_t method() const;

        //! Useful ref for serialisation
        method_t &method();

        void method(method_t method);

        const std::string &host() const;

        unsigned int port() const;

        const std::string &path() const;

        //Get a param. Note-- request does NOT parse param_str itself, the controller-list of this class must add_param's accordingly.
        //(In a typical net usage case, this is done by net::router, and includes params from the path)
        std::optional<std::string> param(const std::string &key) const;

        void add_param(const std::string &key, const std::string &value);

        void emplace_param(const std::string &key, std::string &&value);

        const std::string &query() const;

        const std::string &fragment() const;

        std::string url() const;

        // Note that this will also set the host header
        void url(const std::string &url);

        // Note that this will also set the host header.
        void url(std::string &&url);

        //! useful for serialisation
        std::unordered_map<std::string, std::string> &params();

        unsigned int num_redirects_allowed() const;

        // Useful for parsing
        unsigned int &num_redirects_allowed();

        void num_redirects_allowed(unsigned int nr);

        void write(std::ostream &ostream, const std::string &http_version = "HTTP/1.1", const std::string &default_user_agent = "C++ based client") const;

    private:

        Url::Url _url;
        std::unordered_map<std::string, std::string> _params;
        method_t _method;
        unsigned int _num_redirects_allowed; //Used by the client
    };


    std::string to_string(request::method_t m);

    //NOTE & WARNING: If the request is a filebody request, the file descriptor will be lost through serialisation.
    //So it's vital that requests that may be filebody requests are only routed locally, and that filebodies are handled close
    // ... to the server in the actor system.

    //For inspection, we'll need to split save and load since Url is a rather complex class
    template <class Inspector>
    typename std::enable_if<Inspector::reads_state,
            typename Inspector::result_type>::type
    inspect(Inspector& f, request& x) {
        return f(caf::meta::type_name("net::request"), x.url(), x.params(), x.method());
    }

    template <class Inspector>
    typename std::enable_if<Inspector::writes_state,
            typename Inspector::result_type>::type
    inspect(Inspector& f, request& x) {
        std::string url;
        auto res = f(caf::meta::type_name("net::request"), url, x.params(), x.method());
        x.url(url);
        return res;
    }

    class get_request : public request {
    public:
        static constexpr method_t method = method_t::GET;
        using request::request;
        get_request(const request &req) : request(req) {}
        get_request(request &&req) : request(std::move(req)) {}
    };

    template <class Inspector>
    typename Inspector::result_type inspect(Inspector& f, get_request& x) {
        return f(caf::meta::type_name("net::get_request"), static_cast<request &>(x));
    }

    class head_request : public request {
    public:
        static constexpr method_t method = method_t::HEAD;
        using request::request;
        head_request(const request &req) : request(req) {}
        head_request(request &&req) : request(std::move(req)) {}
    };

    template <class Inspector>
    typename Inspector::result_type inspect(Inspector& f, head_request& x) {
        return f(caf::meta::type_name("net::head_request"), static_cast<request &>(x));
    }

    class put_request : public request {
    public:
        static constexpr method_t method = method_t::PUT;
        using request::request;
        put_request(const request &req) : request(req) {}
        put_request(request &&req) : request(std::move(req)) {}
    };

    template <class Inspector>
    typename Inspector::result_type inspect(Inspector& f, put_request& x) {
        return f(caf::meta::type_name("net::put_request"), static_cast<request &>(x));
    }

    class post_request : public request {
    public:
        static constexpr method_t method = method_t::POST;
        using request::request;
        post_request(const request &req) : request(req) {}
        post_request(request &&req) : request(std::move(req)) {}
    };

    template <class Inspector>
    typename Inspector::result_type inspect(Inspector& f, post_request& x) {
        return f(caf::meta::type_name("net::post_request"), static_cast<request &>(x));
    }

    class delete_request : public request {
    public:
        static constexpr method_t method = method_t::DELETE;
        using request::request;
        delete_request(const request &req) : request(req) {}
        delete_request(request &&req) : request(std::move(req)) {}
    };

    template <class Inspector>
    typename Inspector::result_type inspect(Inspector& f, delete_request& x) {
        return f(caf::meta::type_name("net::delete_request"), static_cast<request &>(x));
    }

    class options_request : public request {
    public:
        static constexpr method_t method = method_t::OPTIONS;
        using request::request;
        options_request(const request &req) : request(req) {}
        options_request(request &&req) : request(std::move(req)) {}
    };

    template <class Inspector>
    typename Inspector::result_type inspect(Inspector& f, options_request& x) {
        return f(caf::meta::type_name("net::options_request"), static_cast<request &>(x));
    }


} //ns as::net
