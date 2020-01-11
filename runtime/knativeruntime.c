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
 * Utility functions to support the Knative C Runtime engine.
 */

#include <libhttp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "knativeruntime.h"
#include "httpstatus.h"

static struct lh_ctx_t *ctx;

char * knative_runtime_alloc(size_t size) {
    return malloc(size);
}

void knative_runtime_free(char * buffer) {
    free(buffer);
}

cb_response_t * knative_runtime_create_response(void) {
    return calloc(1, sizeof(cb_response_t));
}

/* Begin request handler for libhttp callback. Call user's initialization routine */
int begin_request(struct lh_ctx_t *ctx, struct lh_con_t *conn) {
    knative_serverless_callback.serverless_setup();

    return 0;
}

/* End request handler for libhttp callback. Call user's teardown routine */
void end_request(struct lh_ctx_t *ctx, const struct lh_con_t *conn, int reply_status_code) {
    knative_serverless_callback.serverless_teardown();
}

char * extract_body(struct lh_ctx_t *ctx, struct lh_con_t *conn) {
    const struct lh_rqi_t * request = httplib_get_request_info(conn);
    const char * bodySize = httplib_get_header(conn, "Content-Length");

    if (bodySize == NULL)
        return NULL;

    int size = atoi(bodySize);
    char * body = malloc(size);

    if (body == NULL) {
        printf("ERROR: Unable to alloc buffer for body\n");
        return NULL;
    }

    httplib_read(ctx, conn, body, size);

    return body;
}

kvpair_t ** extract_headers(struct lh_con_t *conn) {
    const struct lh_rqi_t * request = httplib_get_request_info(conn);

    if (request->num_headers == 0)
        return NULL;

    /* Provide an extra element to allow for null termination */
    kvpair_t ** headers = calloc(request->num_headers + 1, sizeof(kvpair_t *));
    if (headers == NULL) {
        printf("ERROR: Unable to alloc for headers\n");
        return NULL;
    }

    for (int i = 0; i < request->num_headers; i++) {
        kvpair_t * kv = malloc(sizeof(kvpair_t));
        if (kv == NULL) {
            printf("ERROR: Unable to alloc for headers\n");
            for (int j = 0; j < i; j++) {
                free(headers[j]);
            }
            free(headers);
            return NULL;
        }

        kv->key = (char *) request->http_headers[i].name;
        kv->value = (char *) request->http_headers[i].value;
        headers[i] = kv;
    }

    return headers;
}

kvpair_t ** extract_query_params(struct lh_con_t *conn) {
    const struct lh_rqi_t * request = httplib_get_request_info(conn);
    int param_count = 1;

    if (request->query_string == NULL)
        return NULL;

    char * query = strdup(request->query_string); /* duplicate string for tokenization */
    int query_length = strlen(query);

    for (int i = 0; i < query_length; i++) {
        if (query[i] == '&') {
            param_count++;
            query[i] = '\0';
        }
    }

    /* Provide an extra element to allow for null termination */
    kvpair_t ** params = calloc(param_count + 1, sizeof(kvpair_t *));
    if (params == NULL) {
        printf("ERROR: Unable to alloc for query params\n");
        free(query);
        return NULL;
    }

    for (int i = 0; i < param_count; i++) {
        kvpair_t * kv = malloc(sizeof(kvpair_t));
        if (kv == NULL) {
            printf("ERROR: Unable to alloc for query params\n");
            for (int j = 0; j < i; j++) {
                free(params[j]);
            }

            free(params);
            free(query);
            return NULL;
        } else {
            params[i] = kv;
        }
    }

    int start_token = 0;
    int mid_token = 0;
    int i = 0;
    int j = 0;
    int work = 1;

    while (work) {
        for (j; query[j] != '\0'; j++) {
            if (query[j] == '=') {
                mid_token = j;
                query[j] = '\0';
            }
        }

        int key_length = mid_token - start_token;
        int value_length = j - mid_token + 1;
        char *key = malloc(sizeof(char) * key_length);
        char *value = malloc(sizeof(char) * value_length);

        memcpy(key, query + start_token, key_length);
        memcpy(value, query + mid_token + 1, value_length);

        params[i]->key = key;
        params[i]->value = value;

        /* Bump incrementors */
        i++;
        j++;
        start_token = j;
        mid_token = j;

        if (j >= query_length) {
            /* Finished processing */
            work = 0;
        }
    }

    /* free the duplicated query string */
    free(query);

    return params;
}

void cleanup_pairs(int values_alloc, kvpair_t ** kv_array) {
    int len = 0;

    while (kv_array[len] != NULL)
        len++;

    for (int i = 0; i < len; i++) {
        if (values_alloc) {
            free(kv_array[i]->key);
            free(kv_array[i]->value);
        }

        free(kv_array[i]);
    }

    free(kv_array);
}

/* Call the serverless function, and parse the results */
int serverless_handler(struct lh_ctx_t *ctx, struct lh_con_t *conn, void *cbdata) {
    kvpair_t **headers = extract_headers(conn);
    kvpair_t **params = extract_query_params(conn);
    char * body = extract_body(ctx, conn);

    cb_response_t * response = knative_serverless_callback.serverless_func(params, headers, body);
    int bodySize = strlen(response->body);

    if (response->http_status == 0) {
        if (bodySize > 0)
            response->http_status = HTTP_STATUS_OK;
        else
            response->http_status = HTTP_STATUS_NO_CONTENT;
    }

    if (response->content_type == NULL) {
        response->content_type = "text/plain";
    }

    httplib_printf(ctx, conn,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"        // Always set Content-Length
        "\r\n"
        "%s",
        response->http_status,
        http_errtostr(response->http_status),
        response->content_type,
        bodySize, (char *) response->body);
    
    free(response);
    if (headers != NULL)
        cleanup_pairs(0, headers);

    if (body != NULL)
        free(body);

    if (params != NULL)
        cleanup_pairs(1, params);

    return 0;
}

/* Signal handler for proper cleanup */
void cleanup(int sig) {
    printf("Cleaning up...\n");
    httplib_stop(ctx);
    exit(0);
}

/* Register the serverless callback with the backend engine */
int main(int argc, char **argv) {
    struct lh_clb_t libhttp_callbacks;

    memset(&libhttp_callbacks, 0, sizeof(libhttp_callbacks));

    if (knative_serverless_callback.serverless_setup != NULL) {
        libhttp_callbacks.begin_request = begin_request;
    }

    if (knative_serverless_callback.serverless_teardown != NULL) {
        libhttp_callbacks.end_request = end_request;
    }

    ctx = httplib_start(&libhttp_callbacks, NULL, NULL);

    if (NULL == ctx)
        return 1;

    httplib_set_request_handler(ctx, "/", serverless_handler, NULL);

    if (signal(SIGINT, cleanup) == SIG_ERR) {
        printf("Failed to handle signal...\n");
        httplib_stop(ctx);
        exit(1);
    }

    if (signal(SIGTERM, cleanup) == SIG_ERR) {
        printf("Failed to handle signal...\n");
        httplib_stop(ctx);
        exit(1);
    }

    while (1)
        ;

    return 0;
}
