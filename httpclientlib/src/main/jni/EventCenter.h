//
// Created by Administrator on 2016/5/26.
//

#ifndef HTTPCLIENT_EVENTCENTER_H
#define HTTPCLIENT_EVENTCENTER_H

#include <stdint.h>
#include <vector>
#include "base/thread.h"
#include "libevent/http-internal.h"


class EventCenter : public CThread{
public:
    EventCenter();
    virtual ~EventCenter();

public:
    void init();
    void unInit();
    struct event_base* getEventBase();
    void addEvent(int32_t fd, void(*callback)(int, short, void*), void* arg);
    void removeEvent(int32_t fd);
    void removeEvent(struct event_base* base);

private:
    struct event_base* currentEventBase;
    std::vector<struct event*> eventVect;
    CLock eventLock;

};


#endif //HTTPCLIENT_EVENTCENTER_H
