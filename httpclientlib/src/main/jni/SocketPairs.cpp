//
// Created by Jarlene on 2016/5/21.
//

#include "SocketPairs.h"


SocketPairs::SocketPairs() {
    if (!create()) {
        _pipe_fd[0] = -1;
        _pipe_fd[1] = -1;
    }
}

SocketPairs::~SocketPairs() {
    close();
}


bool SocketPairs::write() {
    if (_pipe_fd[1] == -1) {
        return false;
    }

    int32_t ret = send(_pipe_fd[1], "a", 1, 0);
    if (ret < 0) {
        return false;
    }

    return true;
}

bool SocketPairs::read() {
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

bool SocketPairs::create() {
    int32_t ret = socketpair(AF_UNIX, SOCK_STREAM, 0, _pipe_fd);
    if (ret < 0) {
        return false;
    }
    return true;
}

void SocketPairs::close() {
    close(_pipe_fd[0]);
    close(_pipe_fd[1]);
}

int32_t SocketPairs::get_listen_fd() {
    return _pipe_fd[0];
}

void SocketPairs::close(int32_t& fd) {
    close(fd);
    fd = -1;
}