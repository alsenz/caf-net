#include "dandelion/request.hpp"

namespace as::net {

    request::request() : _url(""), _method(method_t::GET), _num_redirects_allowed(0) {}

    //TODO improve URL handling here - this doesn't seem to work if the scheme is missing!
    request::request(const std::string &req_url, method_t method) : _url(""), _method(method), _num_redirects_allowed(0) {
        url(req_url); //The method evaluate sets a few extra things up.
    }

    method_t request::method() const {
        return _method;
    }

    method_t &request::method() {
        return _method;
    }

    void request::method(method_t method) {
        _method = method;
    }

    const std::string &request::host() const {
        return _url.host();
    }

    unsigned int request::port() const {
        auto p = _url.port();
        if(p == 0) {
            return 80;
        }
        return p;
    }

    const std::string &request::path() const {
        return _url.path();
    }

    std::optional<std::string> request::param(const std::string &key) const {
        auto it = _params.find(key);
        if (it == std::end(_params)) {
            return {};
        }
        return it->second;
    }

    void request::add_param(const std::string &key, const std::string &value) {
        _params.insert(std::pair(key, value));
    }

    void request::emplace_param(const std::string &key, std::string &&value) {
        _params.emplace(key, std::move(value));
    }

    const std::string &request::query() const {
        return _url.query();
    }

    const std::string &request::fragment() const {
        return _url.fragment();
    }

    std::string request::url() const {
        return _url.str();
    }

    void request::url(const std::string &url) {
        _url.assign(Url::Url(url));
        set_header("Host", _url.host());
    }

    void request::url(std::string &&url) {
        _url.assign(Url::Url(std::move(url)));
        set_header("Host", _url.host());
    }

    std::unordered_map<std::string, std::string> &request::params() {
        return _params;
    }

    unsigned int request::num_redirects_allowed() const {
        return _num_redirects_allowed;
    }

    unsigned int &request::num_redirects_allowed() {
        return _num_redirects_allowed;
    }

    void request::num_redirects_allowed(unsigned int nr) {
        _num_redirects_allowed = nr;
    }

    void request::write(std::ostream &ostream, const std::string &http_version, const std::string &default_user_agent) const {
        std::string path = _url.path();
        if (path.empty() || path[path.length()] != '/') {
            path.append("/");
        }
        ostream << to_string(_method) << " " << path << " " << http_version << "\n";
        // We now need to write some extra headers if they're missing...
        auto full_headers = headers();
        if(!header("Host")) {
            full_headers["Host"] = _url.host();
        }
        if(!header("User-Agent")) {
            full_headers["User-Agent"] = default_user_agent;
        }
        if(!header("Accept")) {
            full_headers["Accept"] = "*/*";
        }
        for(auto kvp : full_headers) {
            ostream << kvp.first << ": " << kvp.second << "\n";
        }
        ostream << "\r\n";
    }


    std::string to_string(method_t m) {
        switch(m) {
            case method_t::GET:
                return "GET";
            case method_t::OPTIONS:
                return "OPTIONS";
            case method_t::HEAD:
                return "HEAD";
            case method_t::DELETE:
                return "DELETE";
            case method_t::POST:
                return "POST";
            case method_t::PUT:
                return "PUT";
        }
        return "GET";
    }
} //ns as::net
