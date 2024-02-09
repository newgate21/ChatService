// CClientService.h
#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class CClientService {
public:
    using Callback = std::function<void(const std::string&, bool)>;

    CClientService(const std::string& serverAddress = "127.0.0.1");
    ~CClientService();

    void Connect();
    void Disconnect();
    void Send(const std::string& message);
    bool IsRunning() { return m_bIsRunning; };
    void SetCallback(Callback callback);

private:
    void ReceiveMessages(SOCKET socket);
    void Notify(const std::string& message, bool status = true);

private:
    std::string m_serverAddress;
    SOCKET m_clientSocket;
    bool m_bIsRunning;
    Callback m_callbackFunc;
};


