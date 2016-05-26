//
// Created by Administrator on 2016/5/26.
//

#include "EventCenter.h"

EventCenter::EventCenter() {

}

EventCenter::~EventCenter() {

}

void EventCenter::init() {

}

void EventCenter::unInit() {

}


struct event_base *EventCenter::getEventBase() {
    return currentEventBase;
}

void EventCenter::addEvent(int32_t fd, void(*callback)(int, short, void*), void* arg) {

}

void EventCenter::removeEvent(int32_t fd) {

}

void EventCenter::removeEvent(struct event_base *base) {

}
