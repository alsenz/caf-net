#include "dandelion/message.hpp"

namespace as::net {

    bool message::is_filebody() const {
        return _fb != nullptr;
    }

    std::FILE *message::filebody() const {
        return _fb;
    }

    void message::filebody(std::FILE *fb) {
        _fb = fb;
    }

    nlohmann::json message::json_body() const {
        return nlohmann::json::parse(body());
    }

    message::message() : _fb(nullptr) {}

    message::body_data_t &message::body() {
        return _body;
    }

    const message::body_data_t &message::body() const {
        return _body;
    }

    message::header_data &message::headers() {
        return _headers;
    }

    const message::header_data &message::headers() const {
        return _headers;
    }

    std::optional<std::string> message::header(const std::string &key) const {
        auto it = _headers.find(key);
        if(it == std::end(_headers)) {
            return {};
        }
        return it->second;
    }

    void message::set_header(const std::string &key, const std::string &value) {
        _headers[key] = value;
    }

    void message::emplace_header(const std::string &key, std::string &&value) {
        _headers.emplace(std::pair(key, std::move(value)));
    }

} //ns as::net
