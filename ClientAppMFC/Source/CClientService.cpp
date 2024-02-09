// CClientService.cpp
#include "pch.h"
#include "CClientService.h"

//*********************************************************
//*
//*********************************************************
CClientService::CClientService(const std::string& serverAddress) : 
    m_clientSocket(INVALID_SOCKET), m_bIsRunning(false), m_serverAddress(serverAddress) {
}

//*********************************************************
//*
//*********************************************************
CClientService::~CClientService() {
    Disconnect();
}

//*********************************************************
//*
//*********************************************************
void CClientService::Connect() {
    // Disconnect previous created socket
    Disconnect();

    // Initialize Winsock
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        m_bIsRunning = false;
        Notify("Error initializing winsock");
        return;
    }

    // Create a socket
    m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_clientSocket == INVALID_SOCKET) {
        m_bIsRunning = false;
        Notify("Error creating socket");
        return;
    }

    // Set up the server address structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, m_serverAddress.c_str(), &hint.sin_addr);

    // Set the socket option for reuse address
    int enable = 1;
    if (setsockopt(m_clientSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(int)) == SOCKET_ERROR) {
        m_bIsRunning = false;
        closesocket(m_clientSocket);
        Notify("Error setting socket option");
        return;
    }

    // Connect to the server
    if (connect(m_clientSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        m_bIsRunning = false;
        Notify("Can't connect to server");
        return;
    }

    m_bIsRunning = true;
    Notify("Connected to server");

    // Start receiving messages in a separate thread
    std::thread(&CClientService::ReceiveMessages, this, m_clientSocket).detach();
}

//*********************************************************
//*
//*********************************************************
void CClientService::Disconnect() {
    m_bIsRunning = false;
    if (m_clientSocket != INVALID_SOCKET) {
        if (closesocket(m_clientSocket) == SOCKET_ERROR) {
            Notify("Error closing socket");
        }
        m_clientSocket = INVALID_SOCKET;
        WSACleanup();
    }
}

//*********************************************************
//*
//*********************************************************
void CClientService::Send(const std::string& message) {
    if (m_clientSocket != INVALID_SOCKET)
        send(m_clientSocket, message.c_str(), message.size(), 0);
}

//*********************************************************
//*
//*********************************************************
void CClientService::ReceiveMessages(SOCKET socket) {
    // Receive and notify messages
    char buffer[1024];
    int bytesReceived;

    while (m_bIsRunning) {
        bytesReceived = recv(socket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            m_bIsRunning = false;
            Notify("Server disconnected");
        }
        else
        { 
            std::string message(buffer, 0, bytesReceived);
            Notify(message, false);
        }
    }
}

//*********************************************************
//*
//*********************************************************
void CClientService::Notify(const std::string& message, bool status)
{
    // Notify the UI
    if (m_callbackFunc) {
        m_callbackFunc(message, status);
    }
}

//*********************************************************
//*
//*********************************************************
void CClientService::SetCallback(Callback callback) {
    m_callbackFunc = callback;
}
