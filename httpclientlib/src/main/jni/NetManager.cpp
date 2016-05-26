//
// Created by Administrator on 2016/5/26.
//

#include "NetManager.h"

NetManager::NetManager() {
    eventCenter = new EventCenter();
}

NetManager::~NetManager() {
    delete eventCenter;
}

void NetManager::init() {
    eventCenter->init();
}

void NetManager::unInit() {
    eventCenter->unInit();
}

void NetManager::setProxy(const char *proxy_addr, unsigned int proxy_port,
                          const char *proxy_user_name, const char *proxy_passwd) {

}

void NetManager::httpRequest(const char *url, int allow_retries, int timeout_secs, bool is_get,
                             char **headers, int header_num) {

}


void NetManager::setCallback(void (*callback)(const char *, int, char *, int, void *), void *arg) {

}
