#include "dandelion/request.hpp"

#include "common/variant-match.hpp"

namespace as::net {

    request::request() : _url(), _method(method_t::GET), _num_redirects_allowed(0) {}

    //TODO improve URL handling here - this doesn't seem to work if the scheme is missing!
    request::request(const std::string &req_url, method_t method) : _url(gnt::dln::parse(req_url)), _method(method), _num_redirects_allowed(0) {
        //TODO ensure that this is parsed right...
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

    std::string request::host() const {
        return _url.authority().host.apply<std::string>(std::match{
            [](const std::string &host) { return host; },
            [](const caf::ip_address &ip_addr) { return caf::to_string(ip_addr); }
        });
    }

    uint16_t request::port() const {
        auto p = _url.authority().port;
        if(p == 0) {
            return 80;
        }
        return p;
    }

    std::string_view request::path() const {
        return std::string_view(_url.path().data(), _url.path().size());
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

    std::string_view request::fragment() const {
        return std::string_view(_url.fragment().data(), _url.fragment().size());
    }

    std::string request::url() const {
        return caf::to_string(_url);
    }

    void request::url(const std::string &url) {
        _url = gnt::dln::parse(url);
        set_header("Host", host());
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
        std::string path = std::string(_url.path().data(), _url.path().size());
        if (path.empty() || path[path.length()] != '/') {
            path.append("/");
        }
        ostream << to_string(_method) << " " << path << " " << http_version << "\n";
        // We now need to write some extra headers if they're missing...
        auto full_headers = headers();
        if(!header("Host")) {
            full_headers["Host"] = host();
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

    gnt::dln::uri &request::uri_impl() {
        return _url;
    }

    const gnt::dln::uri &request::uri_impl() const {
        return _url;
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
