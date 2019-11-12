/*
 * Locks the workstation and puts the displays to sleep
 */
 
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")

#include <iostream>
#include <windows.h>

using std::cout;
using std::cerr;
using std::endl;

void usleep(__int64 usec) 
{ 
    HANDLE timer; 
    LARGE_INTEGER ft; 

    ft.QuadPart = -(10*usec);

    timer = CreateWaitableTimer(NULL, TRUE, NULL); 
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
    WaitForSingleObject(timer, INFINITE); 
    CloseHandle(timer); 
}

LPTSTR GetLastErrorFormatted()
{
    LPTSTR msg;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), 0, (LPTSTR) &msg, 0, nullptr);          
    return msg;
}

bool DesktopUnlocked()
{
    HDESK hDesktop = OpenDesktop("Default", 0, false, DESKTOP_SWITCHDESKTOP);
    if(hDesktop!=0)
    {
        if(SwitchDesktop(hDesktop))
        {
            CloseDesktop(hDesktop);
            return true;
        }
        else
            CloseDesktop(hDesktop);
    }
    else
    {
        LPTSTR error = GetLastErrorFormatted();
        MessageBox(nullptr, error, "Error", MB_OK);
        delete[] error;
    }
    
    return false;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Lock and wait for lock to kick in
    int cnt = 0;
    LockWorkStation();
    while(DesktopUnlocked())
    {
        usleep(500000);
        cnt ++;
        if(cnt>=10)
        {
            MessageBox(nullptr, "Failed to lock workstation, aborting", "Error", MB_OK);
            return 0;
        }
    }
    
    // Power off the display
    SendMessageW(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
    
    return 0;
}
