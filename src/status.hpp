#pragma once

/* NOTICE: Some material in this file is based on best/http/status.hpp, by Vinnie Falco.
 * Here is the license for that work:
 * //
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//
*/

#include <string>

namespace as::net {

    enum class status_t : unsigned {
        continue_ = 100,
        switching_protocols = 101,
        processing = 102,

        ok = 200,
        created = 201,
        accepted = 202,
        non_authoritative_information = 203,
        no_content = 204,
        reset_content = 205,
        partial_content = 206,
        multi_status = 207,
        already_reported = 208,
        im_used = 226,

        multiple_choices = 300,
        moved_permanently = 301,
        found = 302,
        see_other = 303,
        not_modified = 304,
        use_proxy = 305,
        temporary_redirect = 307,
        permanent_redirect = 308,

        bad_request = 400,
        unauthorized = 401,
        payment_required = 402,
        forbidden = 403,
        not_found = 404,
        method_not_allowed = 405,
        not_acceptable = 406,
        proxy_authentication_required = 407,
        request_timeout = 408,
        conflict = 409,
        gone = 410,
        length_required = 411,
        precondition_failed = 412,
        payload_too_large = 413,
        uri_too_long = 414,
        unsupported_media_type = 415,
        range_not_satisfiable = 416,
        expectation_failed = 417,
        misdirected_request = 421,
        unprocessable_entity = 422,
        locked = 423,
        failed_dependency = 424,
        upgrade_required = 426,
        precondition_required = 428,
        too_many_requests = 429,
        request_header_fields_too_large = 431,
        connection_closed_without_response = 444,
        unavailable_for_legal_reasons = 451,
        client_closed_request = 499,

        internal_server_error = 500,
        not_implemented = 501,
        bad_gateway = 502,
        service_unavailable = 503,
        gateway_timeout = 504,
        http_version_not_supported = 505,
        variant_also_negotiates = 506,
        insufficient_storage = 507,
        loop_detected = 508,
        not_extended = 510,
        network_authentication_required = 511,
        network_connect_timeout_error = 599
    };

    std::string to_string(status_t status);

    status_t int_to_status(unsigned int i);


} //ns as::net