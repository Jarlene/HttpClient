//
// Created by Jarlene on 16/6/6.
//

#ifndef HTTPCLIENT_HTTPMANAGER_H
#define HTTPCLIENT_HTTPMANAGER_H

#include <string.h>
#include "HttpClient.h"
#include "base/Lock.h"


class HttpManager {

private:
    HttpManager();
public:
    virtual ~HttpManager();
private:
    static HttpManager* httpManager;
    static HttpClient* httpClient;
public:
    static HttpManager* getInstance();
public:
    char * doHttpGet(const char* url) const ;
    char * doHttpPost(const char* url) const ;

};


#endif //HTTPCLIENT_HTTPMANAGER_H
