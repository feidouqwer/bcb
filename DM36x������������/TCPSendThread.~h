//---------------------------------------------------------------------------

#ifndef TCPSendThreadH
#define TCPSendThreadH
//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Classes.hpp>
#include <winsock.h>
//---------------------------------------------------------------------------
class TTCPSendThread : public TThread
{            
private:
    SOCKET sendSock;
    FILE *kernel;
    FILE *filesys;
    int kernelSize;
    int filesysSize;
    int totalSendLen;
    int devNo;
    SOCKADDR_IN clientSockAddr;
protected:
    void __fastcall Execute();
public:
    __fastcall TTCPSendThread(bool CreateSuspended);
    __fastcall TTCPSendThread(bool CreateSuspended, SOCKET sock, int devNo, SOCKADDR_IN *pClientSockAddr);
    void __fastcall TTCPSendThread::OpenKernelFile();
    void __fastcall TTCPSendThread::OpenFilesysFile();
    void __fastcall TTCPSendThread::UpdateLable();
    void __fastcall TTCPSendThread::EnableButton();
};
//---------------------------------------------------------------------------
#endif
