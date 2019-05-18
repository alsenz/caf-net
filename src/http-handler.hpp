#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "http_parser.h"

#include "caf/io/all.hpp"

#include "caf-net/status.hpp"

namespace as::net {

    class http_handler {
    public:

        //Basically a C++-ey standin for the node http mode.
        enum class mode {request, response};

        http_handler(mode parser_mode);

    protected:

        //Callbacks for the http parser... these need to be static so they can be C callback... \/

        static int on_message_begin_cb(http_parser *parser) {
            reinterpret_cast<http_handler *>(parser->data)->on_message_begin();
            return 0;
        }

        static int on_headers_complete_cb(http_parser *parser) {
            return reinterpret_cast<http_handler *>(parser->data)->on_headers_complete_impl();
        }

        static int on_message_complete_cb(http_parser *parser) {
            return reinterpret_cast<http_handler *>(parser->data)->on_message_complete_impl();
        }

        static int on_header_field_cb(http_parser *parser, const char *p, size_t len) {
            return reinterpret_cast<http_handler *>(parser->data)->on_header_field_impl(p, len);
        }

        static int on_header_value_cb(http_parser *parser, const char *p, size_t len) {
            return reinterpret_cast<http_handler *>(parser->data)->on_header_value_impl(p, len);
        }

        static int on_body_cb(http_parser *parser, const char *p, size_t len) {
            return reinterpret_cast<http_handler *>(parser->data)->on_body_impl(p, len);
        }

        static int on_url_cb(http_parser *parser, const char *p, size_t len) {
            return reinterpret_cast<http_handler *>(parser->data)->on_url_impl(p, len);
        }

        static int on_status_cb(http_parser *parser, const char *p, size_t len) {
            return reinterpret_cast<http_handler *>(parser->data)->on_status_impl(parser->status_code);
        }

        //Virtual implementation functions
        // implementor overrides

        virtual void on_body(const std::string_view &body_data) = 0;

        virtual void on_url(const std::string &url) {} // Optional -- only relevant to requests

        virtual void on_status(net::status_t status) {} //Optional --only relevant to responses

        virtual void on_header(const std::string &key, const std::string &value) = 0;

        virtual void on_message_begin() = 0;

        virtual void on_message_complete() = 0;


        // A simple handler function that both are likely to want to use

        void handle_new_data_msg(const caf::io::new_data_msg &ndm);

    private:

        // Implementations
        int on_headers_complete_impl();

        int on_message_complete_impl();

        int on_url_impl(const char *p, size_t len);

        int on_status_impl(unsigned int status_int);

        int on_body_impl(const char *p, size_t len);

        int on_header_field_impl(const char *p, size_t len);

        int on_header_value_impl(const char *p, size_t len);

        void dispatch_current_header();


    private:

        void reset_http_parser();

        mode _mode;
        std::unique_ptr<http_parser_settings> _parser_settings;
        std::unique_ptr<http_parser> _parser;

        bool _parser_invalidated = false;

        bool _last_line_was_value = false;
        std::string _current_header_key = "";
        std::string _current_header_value = "";
        std::string _current_url = "";

    };


} //ns as::net
