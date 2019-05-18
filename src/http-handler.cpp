#include "http-handler.hpp"

as::net::http_handler::http_handler(as::net::http_handler::mode parser_mode) : _mode(parser_mode),  _parser_settings(new http_parser_settings()), _parser(nullptr)  {
    http_parser_settings_init(_parser_settings.get());

    _parser_settings->on_body = &http_handler::on_body_cb;
    _parser_settings->on_header_field = &http_handler::on_header_field_cb;
    _parser_settings->on_header_value = &http_handler::on_header_value_cb;
    _parser_settings->on_headers_complete = &http_handler::on_headers_complete_cb;
    _parser_settings->on_message_begin = &http_handler::on_message_begin_cb;
    _parser_settings->on_message_complete = &http_handler::on_message_complete_cb;
    _parser_settings->on_url = &http_handler::on_url_cb;
    _parser_settings->on_status = &http_handler::on_status_cb;

    reset_http_parser();
}

void as::net::http_handler::handle_new_data_msg(const caf::io::new_data_msg &ndm) {
    if (_parser_invalidated) {
        reset_http_parser();
    }
    http_parser_execute(_parser.get(), _parser_settings.get(), ndm.buf.data(), ndm.buf.size());
}

int as::net::http_handler::on_headers_complete_impl() {
    if(_last_line_was_value) {
        dispatch_current_header();
    }
    _last_line_was_value = false;
    //Nothing. We expect to read the body as and when
    return 0;
}

int as::net::http_handler::on_message_complete_impl() {
    _parser_invalidated = true;
    on_message_complete();
    return 0;
}

int as::net::http_handler::on_url_impl(const char *p, size_t len) {
    _current_url.append(p, len);
    // Don't fire yet! In http, it'll be missing the host!
    return 0;
}

int as::net::http_handler::on_status_impl(unsigned int status_int) {
    on_status(int_to_status(status_int));
    return 0;
}

int as::net::http_handler::on_body_impl(const char *p, size_t len) {
    on_body(std::string_view(p, len));
    return 0;
}

int as::net::http_handler::on_header_field_impl(const char *p, size_t len) {
    if (_last_line_was_value) {
        dispatch_current_header();
    }
    _current_header_key.append(p, len);
    _last_line_was_value = false;
    return 0;
}

int as::net::http_handler::on_header_value_impl(const char *p, size_t len) {
    //Rely on default initialisation for string here...
    _current_header_value.append(p, len);
    _last_line_was_value = true;
    return 0;
}

void as::net::http_handler::dispatch_current_header() {
    on_header(_current_header_key, _current_header_value);
    if (_current_header_key == "Host" || _current_header_key == "host") {
        std::string full_url("http://");
        full_url.append(_current_header_value);
        full_url.append(_current_url);
        on_url(full_url); //Now we can fire with the full url!
    }
    _current_header_key.clear();
    _current_header_value.clear();
}

void as::net::http_handler::reset_http_parser() {
    _parser.reset(new http_parser());
    switch(_mode) {
        case mode::response:
            http_parser_init(_parser.get(), HTTP_RESPONSE);
            break;
        case mode::request:
            http_parser_init(_parser.get(), HTTP_REQUEST);
            break;
    }
    _parser->data = this;
    _parser_invalidated = false;
}
