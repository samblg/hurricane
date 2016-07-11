/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

#include "IoLoop.h"
#include "kqueue/kqueueloop.h"
#include "eventqueue.h"
#include <functional>

KQueueLoop::KQueueLoop(const std::string& host, int port) :
        _host(host), _port(port)
{
}

void KQueueLoop::Run() {
    std::thread listenThread(std::bind(&KQueueLoop::ServerListenThread, this));
    listenThread.detach();
}

void KQueueLoop::ServerListenThread()
{
    int32_t sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == sock)
    {
        std::cerr << "socket() failed:" << errno << std::endl;
        return;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = inet_addr(_host.c_str());
    if (bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1)
    {
        std::cerr << "bind() failed:" << errno << std::endl;
        return;
    }

    if (listen(sock, 5) == -1)
    {
        std::cerr << "listen() failed:" << errno << std::endl;
        return;
    }

    // Register socket describer
    int32_t kq = kqueue();
    int32_t registrar = _RegisterKQueue(kq, sock);
    if (0 != registrar)
    {
        std::cerr << "Register listener to kqueue failed.\n";
        return;
    }

    // Enter loop
    struct kevent events[MAX_EVENT_COUNT];
    while (true)
    {
        int ret = kevent(kq, NULL, 0, events, MAX_EVENT_COUNT, NULL);
        if (ret == -1)
        {
            std::cerr << "kevent failed!\n";
            continue;
        }

        _HandleEvent(kq, sock, events, ret);
    }
}

int32_t KQueueLoop::_RegisterKQueue(int32_t kq, int32_t fd)
{
    struct kevent changes[1];
    EV_SET(&changes[0], fd, EVFILT_READ, EV_ADD, 0, 0, NULL);

    int ret = kevent(kq, changes, 1, NULL, 0, NULL);
    if (-1 == ret)
        return -1;

    return 0;

}

void KQueueLoop::_HandleEvent(int32_t kq, int32_t fd, struct kevent* events, int32_t nevents)
{
    for (int i = 0; i < nevents; i++)
    {
        uint64_t sock = events[i].ident;
        int64_t data = events[i].data;

        if (sock == fd)
            _Accept(kq, fd, data);
        else
            _Receive(sock, data);
    }
}

void KQueueLoop::_Accept(int32_t kq, int32_t fd, int32_t connSize)
{
    for (int i = 0; i < connSize; i++)
    {
        int client = accept(fd, NULL, NULL);
        if (client == -1)
        {
            std::cerr << "Accept failed.\n";
            continue;
        }

        KQueueConnectionPtr connection = std::make_shared<KQueueConnection>(client);
		_connections[client] = connection;
		std::cout << "New connection" << std::endl;

        if (!_RegisterKQueue(kq, client))
        {
            std::cerr << "Register client failed.\n";
            return;
        }
    }
}

void KQueueLoop::_Read(int32_t sock, uint32_t availBytes)
{
    char buf[MAX_RECV_BUFF];
    int64_t bytes = recv(sock, buf, availBytes, 0);
    if (bytes == 0 || bytes == -1)
    {
        _connections.erase(sock);
        Close(sock);

        std::cerr << "client Close or recv failed.\n";
        return;
    }

    // Write buf to the receive queue.
    _Enqueue(_connections[sock], buf, bytes);
}

void KQueueLoop::_Enqueue(KQueueConnectionPtr connection, const char* buf, int64_t bytes)
{
    if ( _dataSink ) {
        _dataSink->OnDataIndication(connection.get(), buf, bytes);
    }
}

void KQueueLoop::SetDataSink(DataSink* dataSink) {
    _dataSink = dataSink;
}
