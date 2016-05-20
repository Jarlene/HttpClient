//
// Created by Administrator on 2016/5/20.
//

#ifndef HTTPCLIENT_HTTPCLIENT_H
#define HTTPCLIENT_HTTPCLIENT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>


class HttpClient {

public:
    HttpClient();
    virtual ~HttpClient();
public:
    bool write();
    bool read();
    void close();
    int32_t get_listen_fd();

protected:
    bool create();
    void close(int32_t& fd);

private:
    int32_t _pipe_fd[2]; //_pipe_fd[0]:读, _pipe_fd[1]:写

};



#endif //HTTPCLIENT_HTTPCLIENT_H
