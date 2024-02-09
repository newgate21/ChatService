// CServerService.h
#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <WinSock2.h>
#include "CLimitSingleInstance.h"

#pragma comment(lib, "ws2_32.lib")

class CServerService : public CLimitSingleInstance {
public:
    CServerService();
    ~CServerService();

    void Init();
    void Start();
    void Stop();

private:
    void HandleClient(SOCKET clientSocket);

private:
    SOCKET m_listeningSocket;
    std::vector<SOCKET> m_clients;
    std::mutex m_mutex;
    int m_iNumberOfConnections;
    bool m_bIsRunning;
};

