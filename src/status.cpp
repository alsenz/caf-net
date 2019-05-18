#include "status.hpp"

namespace as::net {


    std::string to_string(status_t status) {
        switch(status)
        {
            // 1xx
            case status_t::continue_:                             return "Continue";
            case status_t::switching_protocols:                   return "Switching Protocols";
            case status_t::processing:                            return "Processing";

                // 2xx
            case status_t::ok:                                    return "OK";
            case status_t::created:                               return "Created";
            case status_t::accepted:                              return "Accepted";
            case status_t::non_authoritative_information:         return "Non-Authoritative Information";
            case status_t::no_content:                            return "No Content";
            case status_t::reset_content:                         return "Reset Content";
            case status_t::partial_content:                       return "Partial Content";
            case status_t::multi_status:                          return "Multi-Status";
            case status_t::already_reported:                      return "Already Reported";
            case status_t::im_used:                               return "IM Used";

                // 3xx
            case status_t::multiple_choices:                      return "Multiple Choices";
            case status_t::moved_permanently:                     return "Moved Permanently";
            case status_t::found:                                 return "Found";
            case status_t::see_other:                             return "See Other";
            case status_t::not_modified:                          return "Not Modified";
            case status_t::use_proxy:                             return "Use Proxy";
            case status_t::temporary_redirect:                    return "Temporary Redirect";
            case status_t::permanent_redirect:                    return "Permanent Redirect";

                // 4xx
            case status_t::bad_request:                           return "Bad Request";
            case status_t::unauthorized:                          return "Unauthorized";
            case status_t::payment_required:                      return "Payment Required";
            case status_t::forbidden:                             return "Forbidden";
            case status_t::not_found:                             return "Not Found";
            case status_t::method_not_allowed:                    return "Method Not Allowed";
            case status_t::not_acceptable:                        return "Not Acceptable";
            case status_t::proxy_authentication_required:         return "Proxy Authentication Required";
            case status_t::request_timeout:                       return "Request Timeout";
            case status_t::conflict:                              return "Conflict";
            case status_t::gone:                                  return "Gone";
            case status_t::length_required:                       return "Length Required";
            case status_t::precondition_failed:                   return "Precondition Failed";
            case status_t::payload_too_large:                     return "Payload Too Large";
            case status_t::uri_too_long:                          return "URI Too Long";
            case status_t::unsupported_media_type:                return "Unsupported Media Type";
            case status_t::range_not_satisfiable:                 return "Range Not Satisfiable";
            case status_t::expectation_failed:                    return "Expectation Failed";
            case status_t::misdirected_request:                   return "Misdirected Request";
            case status_t::unprocessable_entity:                  return "Unprocessable Entity";
            case status_t::locked:                                return "Locked";
            case status_t::failed_dependency:                     return "Failed Dependency";
            case status_t::upgrade_required:                      return "Upgrade Required";
            case status_t::precondition_required:                 return "Precondition Required";
            case status_t::too_many_requests:                     return "Too Many Requests";
            case status_t::request_header_fields_too_large:       return "Request Header Fields Too Large";
            case status_t::connection_closed_without_response:    return "Connection Closed Without Response";
            case status_t::unavailable_for_legal_reasons:         return "Unavailable For Legal Reasons";
            case status_t::client_closed_request:                 return "Client Closed Request";
                // 5xx
            case status_t::internal_server_error:                 return "Internal Server Error";
            case status_t::not_implemented:                       return "Not Implemented";
            case status_t::bad_gateway:                           return "Bad Gateway";
            case status_t::service_unavailable:                   return "Service Unavailable";
            case status_t::gateway_timeout:                       return "Gateway Timeout";
            case status_t::http_version_not_supported:            return "HTTP Version Not Supported";
            case status_t::variant_also_negotiates:               return "Variant Also Negotiates";
            case status_t::insufficient_storage:                  return "Insufficient Storage";
            case status_t::loop_detected:                         return "Loop Detected";
            case status_t::not_extended:                          return "Not Extended";
            case status_t::network_authentication_required:       return "Network Authentication Required";
            case status_t::network_connect_timeout_error:         return "Network Connect Timeout Error";

            default:
                break;
        }
        return "<unknown-status>";
    }

    status_t int_to_status(unsigned int i) {

        switch(i) {
            case 100: return status_t::continue_;
            case 101: return status_t::switching_protocols;
            case 102: return status_t::processing;

            case 200: return status_t::ok;
            case 201: return status_t::created;
            case 202: return status_t::accepted;
            case 203: return status_t::non_authoritative_information;
            case 204: return status_t::no_content;
            case 205: return status_t::reset_content;
            case 206: return status_t::partial_content;
            case 207: return status_t::multi_status;
            case 208: return status_t::already_reported;
            case 226: return status_t::im_used;

            case 300: return status_t::multiple_choices;
            case 301: return status_t::moved_permanently;
            case 302: return status_t::found;
            case 303: return status_t::see_other;
            case 304: return status_t::not_modified;
            case 305: return status_t::use_proxy;
            case 307: return status_t::temporary_redirect;
            case 308: return status_t::permanent_redirect;

            case 400: return status_t::bad_request;
            case 401: return status_t::unauthorized;
            case 402: return status_t::payment_required;
            case 403: return status_t::forbidden;
            case 404: return status_t::not_found;
            case 405: return status_t::method_not_allowed;
            case 406: return status_t::not_acceptable;
            case 407: return status_t::proxy_authentication_required;
            case 408: return status_t::request_timeout;
            case 409: return status_t::conflict;
            case 410: return status_t::gone;
            case 411: return status_t::length_required;
            case 412: return status_t::precondition_failed;
            case 413: return status_t::payload_too_large;
            case 414: return status_t::uri_too_long;
            case 415: return status_t::unsupported_media_type;
            case 416: return status_t::range_not_satisfiable;
            case 417: return status_t::expectation_failed;
            case 421: return status_t::misdirected_request;
            case 422: return status_t::unprocessable_entity;
            case 423: return status_t::locked;
            case 424: return status_t::failed_dependency;
            case 426: return status_t::upgrade_required;
            case 428: return status_t::precondition_required;
            case 429: return status_t::too_many_requests;
            case 431: return status_t::request_header_fields_too_large;
            case 444: return status_t::connection_closed_without_response;
            case 451: return status_t::unavailable_for_legal_reasons;
            case 499: return status_t::client_closed_request;

            case 500: return status_t::internal_server_error;
            case 501: return status_t::not_implemented;
            case 502: return status_t::bad_gateway;
            case 503: return status_t::service_unavailable;
            case 504: return status_t::gateway_timeout;
            case 505: return status_t::http_version_not_supported;
            case 506: return status_t::variant_also_negotiates;
            case 507: return status_t::insufficient_storage;
            case 508: return status_t::loop_detected;
            case 510: return status_t::not_extended;
            case 511: return status_t::network_authentication_required;
            case 599: return status_t::network_connect_timeout_error;

            default:
                return status_t::internal_server_error; //Something pretty bad must have gone wrong up top!
        };
    }
} //ns as::net