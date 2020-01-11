/** 
 * Copyright (c) 2020 Atelier Solutions. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **/

#include "httpstatus.h"

const char * http_errtostr(int status) {
    switch (status) {
        case HTTP_STATUS_CONTINUE:
            return "Continue";
        case HTTP_STATUS_OK:
            return "OK";
        case HTTP_STATUS_CREATED:
            return "Created";
        case HTTP_STATUS_ACCEPTED:
            return "Accepted";
        case HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION:
            return "Non-Authoritative Information";
        case HTTP_STATUS_NO_CONTENT:
            return "No Content";
        case HTTP_STATUS_RESET_CONNECTION:
            return "Reset Connection";
        case HTTP_STATUS_PARTIAL_CONTENT:
            return "Partial Content";

        case HTTP_STATUS_MULTIPLE_CHOICES:
            return "Multiple Choices";
        case HTTP_STATUS_MOVED_PERMANENTLY:
            return "Moved Permanently";
        case HTTP_STATUS_FOUND:
            return "Found";
        case HTTP_STATUS_SEE_OTHER:
            return "See Other";
        case HTTP_STATUS_NOT_MODIFED:
            return "Not Modified";
        case HTTP_STATUS_TEMPORARY_REDIRECT:
            return "Temporary Redirect";
        case HTTP_STATUS_PERMANENT_REDIRECT:
            return "Permanent Redirect";

        case HTTP_STATUS_BAD_REQUEST:
            return "Bad Request";
        case HTTP_STATUS_UNAUTHORIZED:
            return "Unauthorized";
        case HTTP_STATUS_PAYMENT_REQUIRED:
            return "Payment Required";
        case HTTP_STATUS_FORBIDDEN:
            return "Forbidden";
        case HTTP_STATUS_NOT_FOUND:
            return "Not Found";
        case HTTP_STATUS_METHOD_NOT_ALLOWED:
            return "Method Not Allowed";
        case HTTP_STATUS_NOT_ACCEPTABLE:
            return "Not Acceptable";
        case HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED:
            return "Proxy Authentication Required";
        case HTTP_STATUS_REQUEST_TIMEOUT:
            return "Request Timeout";
        case HTTP_STATUS_CONFLICT:
            return "Conflict";
        case HTTP_STATUS_GONE:
            return "Gone";
        case HTTP_STATUS_LENGTH_REQUIRED:
            return "Length Required";
        case HTTP_STATUS_PRECONDITION_FAILED:
            return "Precondition Failed";
        case HTTP_STATUS_PAYLOAD_TOO_LARGE:
            return "Payload Too Large";
        case HTTP_STATUS_URI_TOO_LONG:
            return "URI Too Long";
        case HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE:
            return "Unsupported Media Type";
        case HTTP_STATUS_RANGE_NOT_SATISFIABLE:
            return "Range Not Satisfiable";
        case HTTP_STATUS_EXPECTATION_FAILED:
            return "Expectation Failed";
        case HTTP_STATUS_MISDIRECTED_REQUEST:
            return "Misdirected Request";
        case HTTP_STATUS_UNPROCESSABLE_ENTITY:
            return "Unprocessable Entity";
        case HTTP_STATUS_TOO_EARLY:
            return "Too Early";
        case HTTP_STATUS_UPGRADE_REQUIRED:
            return "Upgrade Required";
        case HTTP_STATUS_PRECONDITION_REQUIRED:
            return "Precondition Required";
        case HTTP_STATUS_TOO_MANY_REQUESTS:
            return "Too Many Requests";
        case HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE:
            return "Request Header Fields Too Large";
        case HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS:
            return "Unavailable for Legal Reasons";

        case HTTP_STATUS_INTERNAL_SERVER_ERROR:
            return "Internal Server Error";
        case HTTP_STATUS_NOT_IMPLEMENTED:
            return "Not Implemented";
        case HTTP_STATUS_BAD_GATEWAY:
            return "Bad Gateway";
        case HTTP_STATUS_SERVICE_UNAVAILABLE:
            return "Service Unavailable";
        case HTTP_STATUS_GATEWAY_TIMEOUT:
            return "Gateway Timeout";
        case HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED:
            return "HTTP Version Not Supported";
        case HTTP_STATUS_VARIANT_ALSO_NEGOTIATES:
            return "Variant Also Negotiates";
        case HTTP_STATUS_NOT_EXTENDED:
            return "Not Extended";
        case HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED:
            return "Network Authentication Required";
        default:
            return "Unknown";
    }
}