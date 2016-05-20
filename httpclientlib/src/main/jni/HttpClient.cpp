//
// Created by Administrator on 2016/5/20.
//

#include "HttpClient.h"

HttpClient::HttpClient() {
    if (!create()) {
        _pipe_fd[0] = -1;
        _pipe_fd[1] = -1;
    }
}

HttpClient::~HttpClient() {
    close();
}


bool HttpClient::write() {
    if (_pipe_fd[1] == -1) {
        return false;
    }

    int32_t ret = send(_pipe_fd[1], "a", 1, 0);
    if (ret < 0) {
        return false;
    }

    return true;
}

bool HttpClient::read() {
    if (_pipe_fd[1] == -1) {
        return false;
    }
    char buf[1024] = { 0 };
    if (recv(_pipe_fd[0], buf, 1024, 0) <= 0) {
        return false;
    } else {
        return true;
    }
}

bool HttpClient::create() {
    int32_t ret = socketpair(AF_UNIX, SOCK_STREAM, 0, _pipe_fd);
    if (ret < 0) {
        return false;
    }
    return true;
}

void HttpClient::close() {
    close(_pipe_fd[0]);
    close(_pipe_fd[1]);
}

int32_t HttpClient::get_listen_fd() {
    return _pipe_fd[0];
}

void HttpClient::close(int32_t& fd) {
    close(fd);
    fd = -1;
}