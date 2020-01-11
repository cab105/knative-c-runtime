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

#ifndef _KNATIVE_RUNTIME_H
#define _KNATIVE_RUNTIME_H

/*
 * Generic key/value pair struct for managing things like query parameters and headers
 */
typedef struct {
    char * key;
    char * value;
} kvpair_t;

/*
 * What kind of response should be sent back to the caller? When NULL is returned,
 * Assume an http_status code of 200, content_type of text/plain, and empty body.
 */
typedef struct {
    int http_status;
    char * content_type;
    void * body;
} cb_response_t;

/* Geneate the response struct to pass for the return */
cb_response_t * knative_runtime_create_response(void);

/*
 * The callback mechanism for invoking the serverless function.  serverless_setup
 * and serverless_teardown are called immediately before and after the invoking
 * of the function, and intended for buffer allocation/freeing operations.
 */
typedef struct {
    cb_response_t * (*serverless_func)(kvpair_t **query_params, kvpair_t **headers, char *body);
    void (*serverless_setup)(void);
    void (*serverless_teardown)(void);
} serverless_cb_t;

/* The callback struct must be treated as a global variable and be defined by the user */
extern serverless_cb_t knative_serverless_callback;

char * knative_runtime_malloc(size_t size);

void knative_runtime_free(char * buffer);

#endif /* _KNATIVE_RUNTIME_H */
