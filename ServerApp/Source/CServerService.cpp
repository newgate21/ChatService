// CServerService.cpp
#include "CServerService.h"

//*********************************************************
//*
//*********************************************************
CServerService::CServerService() :
    CLimitSingleInstance(L"ServerAppName"), 
    m_listeningSocket(INVALID_SOCKET), 
    m_iNumberOfConnections(0),
    m_bIsRunning(true) {

}

//*********************************************************
//*
//*********************************************************
CServerService::~CServerService() {
    Stop();
}

//*********************************************************
//*
//*********************************************************
void CServerService::Init() {
    // Initialize Winsock
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        std::cerr << "Error initializing winsock" << std::endl;
        m_bIsRunning = false;
        return;
    }

    // Create a socket
    m_listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listeningSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        m_bIsRunning = false;
        return;
    }

    // Set up the server address structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    // Bind the socket
    if (bind(m_listeningSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket" << std::endl;
        m_bIsRunning = false;
        return;
    }

    // Listen on the socket
    if (listen(m_listeningSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket" << std::endl;
        m_bIsRunning = false;
        return;
    }
}

//*********************************************************
//*
//*********************************************************
void CServerService::Start() {
    std::cout << "Server started, waiting for connections..." << std::endl;

    while (m_bIsRunning) {
        SOCKET clientSocket = accept(m_listeningSocket, nullptr, nullptr);

        std::lock_guard<std::mutex> lock(m_mutex);
        m_clients.push_back(clientSocket);

        m_iNumberOfConnections++;
        std::cout << "Client connected!" << std::endl;
        std::cout << "Current number of Clients: " << m_iNumberOfConnections << std::endl;

        std::thread(&CServerService::HandleClient, this, clientSocket).detach();
    }
}

//*********************************************************
//*
//*********************************************************
void CServerService::Stop() {
    closesocket(m_listeningSocket);
    WSACleanup();
}

//*********************************************************
//*
//*********************************************************
void CServerService::HandleClient(SOCKET clientSocket) {
    // Receive and broadcast messages
    char buffer[1024];
    int bytesReceived;

    while (m_bIsRunning) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::lock_guard<std::mutex> lock(m_mutex);
            closesocket(clientSocket);
            m_clients.erase(std::remove(m_clients.begin(), m_clients.end(), clientSocket), m_clients.end());

            m_iNumberOfConnections--;
            std::cout << "Client disconnected!" << std::endl;
            std::cout << "Current number of Clients: " << m_iNumberOfConnections << std::endl;
            break;
        }

        std::string message(buffer, 0, bytesReceived);

        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& client : m_clients) {
            if (client != clientSocket) {
                send(client, message.c_str(), message.size(), 0);
            }
        }
    }
}
