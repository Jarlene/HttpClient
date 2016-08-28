//
// Created by Jarlene on 16/6/3.
//

#include "HttpClient.h"



HttpClient::HttpClient() {
    base = event_base_new();
}

HttpClient::~HttpClient() {
    event_base_free(base);
}

void HttpClient::printRequestInfo(struct evkeyvalq *header) {
    if (header == NULL) {
        LOGE("the header point is null");
        return;
    }
    struct evkeyval *first_node = header->tqh_first;
    while (first_node) {
        LOGD("key:%s  value:%s", first_node->key, first_node->value);
        first_node = first_node->next.tqe_next;
    }

}

void HttpClient::httpRequestPostCB(struct evhttp_request *req, void *arg) {
    struct HttpRequestPost *http_req_post = (struct HttpRequestPost *)arg;
    switch(req->response_code) {
        case HTTP_OK:
        {
            struct evbuffer* buf = evhttp_request_get_input_buffer(req);
            size_t len = evbuffer_get_length(buf);
            LOGD("print the head info:");
            printRequestInfo(req->output_headers);

            LOGD("len:%zu  body size:%zu", len, req->body_size);
            char *tmp = (char*) malloc(len+1);
            memcpy(tmp, evbuffer_pullup(buf, -1), len);
            tmp[len] = '\0';
            LOGD("print the body:");
            LOGD("HTML BODY:%s", tmp);
            *http_req_post->response = *tmp;
            free(tmp);

            event_base_loopexit(http_req_post->base, 0);
            break;
        }
        case HTTP_MOVEPERM:
            LOGD("the uri moved permanently");
            break;
        case HTTP_MOVETEMP:
        {
            const char *new_location = evhttp_find_header(req->input_headers, "Location");
            struct evhttp_uri *new_uri = evhttp_uri_parse(new_location);
            evhttp_uri_free(http_req_post->uri);
            http_req_post->uri = new_uri;
            startUrlRequest((struct HttpRequestGet *)http_req_post, REQUEST_POST_FLAG);
            break;
        }

        default:
            event_base_loopexit(http_req_post->base, 0);
            break;
    }
}

void HttpClient::httpRequestGetCB(struct evhttp_request *req, void *arg) {
    struct HttpRequestGet *http_req_get = (struct HttpRequestGet *)arg;
    switch(req->response_code)
    {
        case HTTP_OK:
        {
            struct evbuffer* buf = evhttp_request_get_input_buffer(req);
            size_t len = evbuffer_get_length(buf);
            LOGD("print the head info:");
            printRequestInfo(req->output_headers);

            LOGD("len:%zu  body size:%zu", len, req->body_size);
            char *tmp = (char*) malloc(len+1);
            memcpy(tmp, evbuffer_pullup(buf, -1), len);
            tmp[len] = '\0';
            LOGD("print the body:");
            LOGD("HTML BODY:%s", tmp);
            *http_req_get->response = *tmp;
            free(tmp);

            event_base_loopexit(http_req_get->base, 0);
            break;
        }
        case HTTP_MOVEPERM:
            LOGD("the uri moved permanently");
            break;
        case HTTP_MOVETEMP:
        {
            const char *new_location = evhttp_find_header(req->input_headers, "Location");
            struct evhttp_uri *new_uri = evhttp_uri_parse(new_location);
            evhttp_uri_free(http_req_get->uri);
            http_req_get->uri = new_uri;
            startUrlRequest(http_req_get, REQUEST_GET_FLAG);
            break;
        }

        default:
            event_base_loopexit(http_req_get->base, 0);
            break;
    }
}

void HttpClient::startUrlRequest(struct HttpRequestGet *http_req, int req_get_flag) {
    if (http_req->cn) {
        evhttp_connection_free(http_req->cn);
    }

    int port = evhttp_uri_get_port(http_req->uri);
    http_req->cn = evhttp_connection_base_new(http_req->base,
                                              NULL,
                                              evhttp_uri_get_host(http_req->uri),
                                              (port == -1 ? 80 : port));

    /**
     * Request will be released by evhttp connection
     * See info of evhttp_make_request()
     */
    if (req_get_flag == REQUEST_POST_FLAG) {
        http_req->req = evhttp_request_new(httpRequestPostCB, http_req);
    } else if (req_get_flag ==  REQUEST_GET_FLAG) {
        http_req->req = evhttp_request_new(httpRequestGetCB, http_req);
    }

    if (req_get_flag == REQUEST_POST_FLAG) {
        const char *path = evhttp_uri_get_path(http_req->uri);
        evhttp_make_request(http_req->cn, http_req->req, EVHTTP_REQ_POST,
                            path ? path : "/");
        /** Set the post data */
        struct HttpRequestPost *http_req_post = (struct HttpRequestPost *)http_req;
        evbuffer_add(http_req_post->req->output_buffer, http_req_post->post_data, strlen(http_req_post->post_data));
        evhttp_add_header(http_req_post->req->output_headers, "Content-Type", http_req_post->content_type);
    } else if (req_get_flag == REQUEST_GET_FLAG) {
        const char *query = evhttp_uri_get_query(http_req->uri);
        const char *path = evhttp_uri_get_path(http_req->uri);
        size_t len = (query ? strlen(query) : 0) + (path ? strlen(path) : 0) + 1;
        char *path_query = NULL;
        if (len > 1) {
            path_query = (char*) calloc(len, sizeof(char));
            sprintf(path_query, "%s?%s", path, query);
        }
        evhttp_make_request(http_req->cn, http_req->req, EVHTTP_REQ_GET,
                            path_query ? path_query: "/");
    }
    /** Set the header properties */
    evhttp_add_header(http_req->req->output_headers, "Host", evhttp_uri_get_host(http_req->uri));
}

void* HttpClient::startHttpRequest(const char *url, int req_get_flag,
                      const char *content_type, const char *data) {
    struct HttpRequestGet *http_req_get = (struct HttpRequestGet*) httpRequestNew(url, req_get_flag, content_type, data);
    startUrlRequest(http_req_get, req_get_flag);
    return http_req_get;
}



void HttpClient::httpRequestFree(struct HttpRequestGet *http_req_get, int req_get_flag) {
    evhttp_connection_free(http_req_get->cn);
    evhttp_uri_free(http_req_get->uri);
    if (req_get_flag == REQUEST_GET_FLAG) {
        free(http_req_get);
    } else if(req_get_flag == REQUEST_POST_FLAG) {
        struct HttpRequestPost *http_req_post = (struct HttpRequestPost*)http_req_get;
        if (http_req_post->content_type) {
            free(http_req_post->content_type);
        }
        if (http_req_post->post_data) {
            free(http_req_post->post_data);
        }
        free(http_req_post);
    }
    http_req_get = NULL;
}

void *HttpClient::httpRequestNew(const char *url, int req_get_flag, const char *content_type,
                                 const char *data) {
    int len = 0;
    if (req_get_flag == REQUEST_GET_FLAG) {
        len = sizeof(struct HttpRequestGet);
    } else if(req_get_flag == REQUEST_POST_FLAG) {
        len = sizeof(struct HttpRequestPost);
    }

    struct HttpRequestGet *http_req_get = (struct HttpRequestGet*)calloc(1, len);
    http_req_get->uri = evhttp_uri_parse(url);

    http_req_get->base = base;

    if (req_get_flag == REQUEST_POST_FLAG) {
        struct HttpRequestPost *http_req_post = (struct HttpRequestPost *)http_req_get;
        if (content_type == NULL) {
            content_type = HTTP_CONTENT_TYPE_URL_ENCODED;
        }
        http_req_post->content_type = strdup(content_type);

        if (data == NULL) {
            http_req_post->post_data = NULL;
        } else {
            http_req_post->post_data = strdup(data);
        }
    }

    return http_req_get;
}
