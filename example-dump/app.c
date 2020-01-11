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

/**
 * Sample function to dump incoming requests.  One thing to note is that the
 * constructor/destructor are not thread-safe so care must be made when using
 * globals for each request.
 **/

#include <stdio.h>
#include <stdlib.h>
#include "knativeruntime.h"
#include "httpstatus.h"

#define BUFFERSIZE 4096

/* The sample function */
cb_response_t * dump_func(kvpair_t **query_params, kvpair_t **headers, char *body) {
    cb_response_t *response = knative_runtime_create_response();
    char buffer[BUFFERSIZE];
    long len = 0;

    len = snprintf(buffer, BUFFERSIZE, "Headers:\n");
    int i = 0;

    while (headers[i] != NULL) {
        len += snprintf(buffer + len, BUFFERSIZE, "\t%s -> %s\n", headers[i]->key, headers[i]->value);
        i++;
    }

    if (query_params != NULL) {
        i = 0; // Reset i back to 0 for the query parameters
        len += snprintf(buffer + len, BUFFERSIZE, "\nQuery parameters:\n");
        while(query_params[i] != NULL) {
            len += snprintf(buffer + len, BUFFERSIZE, "\t%s -> %s\n", query_params[i]->key, query_params[i]->value);
            i++;
        }
    }

    len += snprintf(buffer + len, BUFFERSIZE, "\nBody:\n%s\n", body);

    response->body = buffer;
}

/* Required to bind the function to the runtime library */
serverless_cb_t knative_serverless_callback = {
    dump_func, /* serverless function */
    NULL, /* A per-connection constructor */
    NULL /* A per-connection destructor */
};
