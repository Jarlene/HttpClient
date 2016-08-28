//
// Created by Jarlene on 16/6/6.
//

#include "HttpManager.h"

static CLock cLock;

HttpManager::HttpManager() {
    httpClient = new HttpClient();
}

HttpManager::~HttpManager() {
    delete httpClient;
    delete httpManager;
}

HttpManager* HttpManager::getInstance() {
    if (httpManager == NULL) {
        cLock.Lock();
        if (httpManager == NULL) {
            httpManager = new HttpManager();
        }
        cLock.Unlock();
    }
    return httpManager;
}

char *HttpManager::doHttpGet(const char *url) const {
    HttpClient* httpClient = new HttpClient();
    struct HttpRequestGet* httpRequestGet = (HttpRequestGet*) httpClient->startHttpRequest(url, REQUEST_GET_FLAG,
                                                                         HTTP_CONTENT_TYPE_URL_ENCODED,
                                                                         NULL);
    char * result = httpRequestGet->response;
    httpClient->httpRequestFree(httpRequestGet, REQUEST_GET_FLAG);
    delete httpClient;
    return result;
}

char *HttpManager::doHttpPost(const char *url) const {
    HttpClient* httpClient = new HttpClient();
    struct HttpRequestGet* httpRequestPost = (HttpRequestGet*) httpClient->startHttpRequest(url, REQUEST_POST_FLAG,
                                                                         HTTP_CONTENT_TYPE_URL_ENCODED,
                                                                         NULL);
    char * result = httpRequestPost->response;
    httpClient->httpRequestFree(httpRequestPost, REQUEST_GET_FLAG);
    delete httpClient;
    return result;
}







