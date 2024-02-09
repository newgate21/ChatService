// main.cpp
#include "CServerService.h"

int main() {
    CServerService server;
    if (server.IsAnotherInstanceRunning())
    {
        server.ShowErrorBox();
        return 0;
    }

    server.Init();
    server.Start(); // Blocking

    std::getchar();
    return 0;
}