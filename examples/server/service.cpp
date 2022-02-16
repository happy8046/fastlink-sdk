#include "service.h"

#include <tchar.h>
#include <string>
#include <Windows.h>

SERVICE_STATUS g_status;
SERVICE_STATUS_HANDLE g_status_handle;

const static auto service_name = _T("fp_demo_svr");

service_func g_on_start;
service_func g_on_stop;
service_func g_run;

bool report_status(DWORD current_state, DWORD win32_exit_code, DWORD wait_hint)
{
    if (current_state == SERVICE_START_PENDING)
        g_status.dwControlsAccepted = 0;
    else
        g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

    g_status.dwCurrentState = current_state;
    g_status.dwWin32ExitCode = win32_exit_code;
    g_status.dwWaitHint = wait_hint;

    if ((current_state == SERVICE_RUNNING) || (current_state == SERVICE_STOPPED))
        g_status.dwCheckPoint = 0;
    else
        g_status.dwCheckPoint++;

    return SetServiceStatus(g_status_handle, &g_status) == TRUE;
}

void WINAPI service_control_handler(DWORD ctrl_code)
{
    switch (ctrl_code)
    {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        report_status(SERVICE_STOP_PENDING, NO_ERROR, 0);
        g_on_stop();
        break;
    default:
        break;
    }
}

void WINAPI service_main(DWORD argc, LPTSTR* argv)
{
    g_status_handle = ::RegisterServiceCtrlHandler(service_name, &service_control_handler);
    g_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_status.dwServiceSpecificExitCode = 0;
    report_status(SERVICE_START_PENDING, NO_ERROR, 0);
    g_on_start();
    report_status(SERVICE_RUNNING, NO_ERROR, 0);
    g_run();
    report_status(SERVICE_STOPPED, NO_ERROR, 0);
}

int run_as_service(service_func on_start, service_func run, service_func on_stop)
{
    g_on_start = on_start;
    g_on_stop = on_stop;
    g_run = run;
    //运行服务
    SERVICE_TABLE_ENTRY dispatch_table[] =
    {
        { (TCHAR*)service_name, (LPSERVICE_MAIN_FUNCTION)service_main },
        { NULL, NULL}
    };
    ::StartServiceCtrlDispatcher(dispatch_table);
    return 0;
}