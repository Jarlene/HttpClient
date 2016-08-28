//
// Created by Jarlene on 16/6/3.
//

#ifndef HTTPCLIENT_HTTPCLIENT_H
#define HTTPCLIENT_HTTPCLIENT_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "include/event2/event.h"
#include "include/event2/http.h"
#include "include/event2/buffer.h"
};

#include "base/log.h"

#define LOG_TAG "CHttpClient"

#define HTTP_CONTENT_TYPE_URL_ENCODED   "application/x-www-form-urlencoded"
#define HTTP_CONTENT_TYPE_FORM_DATA     "multipart/form-data"
#define HTTP_CONTENT_TYPE_TEXT_PLAIN    "text/plain"

#define REQUEST_POST_FLAG  2
#define REQUEST_GET_FLAG   3

struct HttpRequestGet {
    struct evhttp_uri *uri;
    struct event_base *base;
    struct evhttp_connection *cn;
    struct evhttp_request *req;
    char * response;
};

struct HttpRequestPost {
    struct evhttp_uri *uri;
    struct event_base *base;
    struct evhttp_connection *cn;
    struct evhttp_request *req;
    char *content_type;
    char *post_data;
    char *response;
};

class HttpClient {

public:
    HttpClient();
    virtual ~HttpClient();

public:

    void* startHttpRequest(const char *url, int req_get_flag, const char *content_type, const char *data);
    void httpRequestFree(struct HttpRequestGet *http_req_get, int req_get_flag);

private:
    static inline void printRequestInfo(struct evkeyvalq *header);
    static void httpRequestPostCB(struct evhttp_request *req, void *arg);
    static void httpRequestGetCB(struct evhttp_request *req, void *arg);
    void* httpRequestNew(const char *url, int req_get_flag, const char *content_type, const char* data);
    static void startUrlRequest(struct HttpRequestGet *http_req, int req_get_flag);
private:
    struct event_base* base;
};


#endif //HTTPCLIENT_HTTPCLIENT_H
