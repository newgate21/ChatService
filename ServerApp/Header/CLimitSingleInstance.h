#include <windows.h>

class CLimitSingleInstance
{
protected:
    DWORD  m_dwLastError;
    HANDLE m_hMutex;

public:
    CLimitSingleInstance(LPCWSTR strMutexName)
    {
        m_hMutex = CreateMutex(NULL, FALSE, strMutexName);
        m_dwLastError = GetLastError();
    }

    ~CLimitSingleInstance()
    {
        if (m_hMutex)
        {
            CloseHandle(m_hMutex);
            m_hMutex = NULL;
        }
    }

    bool IsAnotherInstanceRunning()
    {
        return (m_dwLastError == ERROR_ALREADY_EXISTS);
    }

    void ShowErrorBox()
    {
        MessageBox(NULL, L"Another Instance is already running!", L"Error!", MB_ICONERROR | MB_OK);
    }
};