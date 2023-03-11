// SystemLab1cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "SystemLab1cpp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

vector <HANDLE> hEvents = {};
HANDLE hThreadEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ThreadEvent"));
HANDLE hStartEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("StartEvent"));
HANDLE hStopEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("StopEvent"));
HANDLE hConfirmEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ConfirmEvent"));
HANDLE hCloseEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("CloseEvent"));
HANDLE hControlEvents[3] = { hStartEvent, hStopEvent, hCloseEvent };
CRITICAL_SECTION cs;

void SecuredOutput(const string& output, CRITICAL_SECTION& cs)
{
    EnterCriticalSection(&cs);
    cout << output << endl;
    LeaveCriticalSection(&cs);
}

DWORD WINAPI Thread(LPVOID lpParameter)
{
    int i = int(lpParameter);
    SecuredOutput(to_string(i), cs);
    SetEvent(hThreadEvent);
    WaitForSingleObject(hEvents[i-1], INFINITE);
    SecuredOutput("thread " + to_string(i) + " done", cs);
    hEvents.pop_back();
    return 0;
}

void start()
{
    InitializeCriticalSection(&cs);
    int i = 1;
    while (true)
    {
        int n = WaitForMultipleObjects(3, hControlEvents, FALSE, INFINITE) - WAIT_OBJECT_0;
        switch (n)
        {
        case 0:
            hEvents.push_back(CreateEvent(NULL, FALSE, FALSE, NULL));
            CreateThread(NULL, 0, Thread, (PVOID)(i), 0, NULL);
            WaitForSingleObject(hThreadEvent, INFINITE);
            i++;
            break;
        case 1:
            if (i != 1)
            {
                --i;
                SetEvent(hEvents[i - 1]);
            } 
            break;

        case 2:
            hEvents.clear();
            ExitProcess(1);
        }
        SetEvent(hConfirmEvent);
    }
}

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            start();
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
