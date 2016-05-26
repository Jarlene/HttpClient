//
// Created by Administrator on 2016/5/26.
//

#ifndef HTTPCLIENT_NETMANAGER_H
#define HTTPCLIENT_NETMANAGER_H


#include "EventCenter.h"


typedef void (*httpResponseCallback) (const char* url,
                                        int32_t code,
                                        char* text,
                                        int32_t len,
                                        void* arg);
typedef void (*normalCallback) (int32_t, const char*, int32_t, char*, int32_t, void*);
typedef void (*locationCallback) (int32_t, const char*, void*);
typedef void (*retryCallback) (int32_t, void*);

class NetManager {
public:
    NetManager();
    virtual ~NetManager();

public:
    virtual void init();
    virtual void unInit();
    virtual void setProxy(const char* proxy_addr, unsigned int proxy_port,
                          const char* proxy_user_name, const char* proxy_passwd);
    virtual void httpRequest(const char* url, int allow_retries, int timeout_secs, bool is_get,
                             char** headers, int header_num);
    virtual void setCallback(void(*callback)(const char*, int, char*, int, void*), void* arg);

private:
    EventCenter* eventCenter;
};


#endif //HTTPCLIENT_NETMANAGER_H
