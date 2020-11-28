#include "dandelion/response.hpp"


namespace as::net {


    response::response() : _status(status_t::internal_server_error) {}

    response::response(status_t status) : _status(status) {}

    status_t response::status() const {
        return _status;
    }

    status_t &response::status() {
        return _status;
    }

    void response::status(status_t status) {
        _status = status;
    }

    bool response::no_response_body() {
        return body().empty();
    }

    void response::append_body(const std::byte *data, std::size_t len) {
        body().insert(std::end(body()), data, data+len);
    }

    void response::append_body(const std::string &data) {
        append_body(reinterpret_cast<const std::byte *>(data.c_str()), data.size());
    }

    std::string_view response::body_view() const {
        return std::string_view(reinterpret_cast<const char *>(body().data()), body().size()); //sizeof byte = sizeof char
    }

    void response::make_stock_response(const std::string &server_name, const std::string &extra_msg) {
        body().reserve(200); //Wild guess
        append_body("<h1>");
        append_body(std::to_string(static_cast<unsigned int>(_status)));
        append_body("</h1>");
        append_body("<h2>");
        append_body(to_string(_status));
        append_body("</h2>");
        append_body("<br/><h4>");
        append_body(extra_msg);
        append_body("</h4>");
        append_body("<br/><br/><br/><br/><div style='position:absolute; bottom: 4em; width: 100%'><hr/>");
        append_body(server_name);
        append_body("</div>");
        emplace_header("Content-Type", "text/html");
    }

    void response::write(std::ostream &ostream, const std::string &http_version) const {
        ostream << http_version << " " << static_cast<unsigned int>(_status) << " " << to_string(_status) << "\n";

        for(const auto &kvp : headers()) {
            ostream << kvp.first << ": " << kvp.second << "\n";
        }

        if(!header("Date")) {
            std::time_t now = std::time(nullptr);
            std::stringstream ss;
            ss << std::asctime(std::gmtime(&now));
            ostream << "Date" << ": " << ss.str() << "\n";
        } else {
            ostream << "\n";
        }

        ostream << "\r"; //\n In the date.
        ostream.write(reinterpret_cast<const char *>(body().data()), body().size());
    }

    void response::write_json_body(const nlohmann::json &json) {
        append_body(json.dump());
        emplace_header("Content-Type", "application/json");
    }

    response::response(status_t status, const nlohmann::json &json) : response(status) {
        write_json_body(json);
    }

} //ns as::net
