#include <windows.h>
#include <iostream>
#include <Wtsapi32.h>
#pragma comment(lib, "Wtsapi32.lib")

SERVICE_STATUS        g_ServiceStatus = { 0 };
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;

SC_HANDLE scmHandle = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
SC_HANDLE serviceHandle = OpenService(scmHandle, L"Notepad_Service", DELETE);

void ServiceCtrlHandler(DWORD dwCtrl)
{
    switch (dwCtrl)
    {
    case SERVICE_CONTROL_STOP:
        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
        // Add cleanup code here

        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
        break;

    case SERVICE_CONTROL_PAUSE:
        g_ServiceStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

        g_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
        break;

    case SERVICE_CONTROL_CONTINUE:
        g_ServiceStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

        g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
        break;

    default:
        break;
    }
}

void RunNotepad()
{
    DWORD sessionId;
    HANDLE hToken;
    if (WTSQueryUserToken(WTSGetActiveConsoleSessionId(), &hToken) != 0)
    {
        STARTUPINFO si = { sizeof(STARTUPINFO) };
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        if (CreateProcessAsUser(hToken,
            L"C:\\Windows\\System32\\notepad.exe",
            NULL,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
        {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        CloseHandle(hToken);
    }
    else
    {
        // Handle error opening user token
    }
}

void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{

    g_StatusHandle = RegisterServiceCtrlHandler(L"Notepad_Service", ServiceCtrlHandler);
    if (g_StatusHandle == NULL)
        return;
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    //ServiceCtrlHandler(SERVICE_CONTROL_STOP);
    RunNotepad();

    // Close the handles
    CloseServiceHandle(serviceHandle);
    CloseServiceHandle(scmHandle);
}

int main()
{
    // Entry point for the service
    SERVICE_TABLE_ENTRY ServiceTable[] =
    {
        {(LPWSTR)L"Notepad_Service", (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };
    // Start the service control dispatcher
    StartServiceCtrlDispatcher(ServiceTable);
 
    return 0;
}