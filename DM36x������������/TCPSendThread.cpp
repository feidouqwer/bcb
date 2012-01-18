//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TCPSendThread.h"
#include "MainFrm.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TTCPSendThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TTCPSendThread::TTCPSendThread(bool CreateSuspended)
    : TThread(CreateSuspended)
{
    kernel = NULL;
    filesys = NULL;
    this->devNo = 0;
    totalSendLen = 0;
}
__fastcall TTCPSendThread::TTCPSendThread(bool CreateSuspended, SOCKET sock, int devNo, SOCKADDR_IN *pClientSockAddr)
    : TThread(CreateSuspended)
{
    kernel = NULL;
    filesys = NULL;
    sendSock = sock;
    this->devNo = devNo;
    memcpy(&clientSockAddr, pClientSockAddr, sizeof(clientSockAddr));
    totalSendLen = 0;
}
void __fastcall TTCPSendThread::OpenFilesysFile()
{
    filesys = fopen(Form1->editFilesys->Text.c_str(), "rb");
    if(filesys == NULL)
    {
        filesysSize = -1;
        return;
    }
    fseek(filesys, 0, SEEK_END);
    filesysSize = ftell(filesys);
    fseek(filesys, 0, SEEK_SET);
}
void __fastcall TTCPSendThread::OpenKernelFile()
{
    kernel = fopen(Form1->editKernel->Text.c_str(), "rb");
    if(kernel == NULL)
    {
        kernelSize = -1;
        return;
    }
    fseek(kernel, 0, SEEK_END);
    kernelSize = ftell(kernel);
    fseek(kernel, 0, SEEK_SET);
}
void __fastcall TTCPSendThread::UpdateLable()
{
    Form1->lblInfo->Tag += 1;
    Form1->lblInfo->Caption = "成功升级设备: " + IntToStr(Form1->lblInfo->Tag);
    Form1->Update();
}
void __fastcall TTCPSendThread::EnableButton()
{
    Form1->finishDevNum++;
    if(Form1->finishDevNum == Form1->clientDevNum)
    {
        Form1->TimerTCP->Enabled = false;
        Form1->btnUpdate->Enabled = true;
        Form1->btnKernel->Enabled = true;
        Form1->btnFilesys->Enabled = true;
        Form1->ComboBox1->Enabled = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TTCPSendThread::Execute()
{
    char buffer[256];
    unsigned char mac[32];
    char ipStr[32];
    cmd_info *pCmdInfo;

    //---- Place thread code here ----
    Synchronize(OpenKernelFile);
    Synchronize(OpenFilesysFile);
    totalSendLen = 0;
    strcpy(ipStr, inet_ntoa(clientSockAddr.sin_addr));
    if(Form1->GetMacFromIp(ipStr, mac))
    {
        char macStr[32];
        snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
                        mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
        Form1->lblMac[devNo]->Caption = macStr;
        Form1->lblMac[devNo]->Color = clBtnFace;
        Form1->lblMac[devNo]->Show();
    }
    while(!this->Terminated)
    {
        int recvLen = recv(sendSock, buffer, sizeof(buffer), 0);
        if(recvLen == SOCKET_ERROR)
        {
            Application->MessageBoxA("TCP接收数据错误", "TCP接收错误", MB_OK);
            Synchronize(EnableButton);
            Form1->lblMac[devNo]->Color = clRed;
            Form1->lblProgress[devNo]->Color = clRed;
            delete this;
            return;
        }
        pCmdInfo = (cmd_info*)buffer;
        if(pCmdInfo->magic != 0x12344321)
        {
            continue;
        }
        switch(pCmdInfo->cmd)
        {
            case CMD_REQUEST_FILE:
            {
                FILE *fp;
                int read_len;
                char file_buf[4 * 1024];

                if(strcmp(pCmdInfo->filename, "kernel") == 0)
                {
                    fp = kernel;
                    pCmdInfo->filelen = kernelSize;
                }
                else if(strcmp(pCmdInfo->filename, "filesys") == 0)
                {
                    fp = filesys;
                    pCmdInfo->filelen = filesysSize;
                }
                else
                {
                    ShowMessage("请求文件错误");
                    continue;
                }
                if(fp == NULL)
                {
                    ShowMessage("打开文件失败");
                    continue;
                }

                pCmdInfo->cmd =  CMD_RESPONSE;

                if(send(sendSock, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
                {
                    Application->MessageBoxA("发送文件信息错误", "错误", MB_OK | MB_ICONERROR);
                    closesocket(sendSock);
                    fclose(fp);
                    Synchronize(EnableButton);
                    Form1->lblMac[devNo]->Color = clRed;
                    Form1->lblProgress[devNo]->Color = clRed;
                    delete this;
                    return;
                }
                while((read_len = fread(file_buf, 1, sizeof(file_buf), fp)) > 0)
                {
                    if(send(sendSock, file_buf, read_len, 0) == SOCKET_ERROR)
                    {
                        Application->MessageBoxA("发送升级文件错误", "错误", MB_OK | MB_ICONERROR);
                        closesocket(sendSock);
                        fclose(fp);
                        Synchronize(EnableButton);
                        Form1->lblMac[devNo]->Color = clRed;
                        Form1->lblProgress[devNo]->Color = clRed;
                        delete this;
                        return;
                    }
                    totalSendLen += read_len;
                    Form1->progressBar[devNo]->Position = totalSendLen * 100 / (kernelSize + filesysSize);
                    Form1->lblProgress[devNo]->Caption = IntToStr(Form1->progressBar[devNo]->Position)
                                                            + "%";
                }
                fclose(fp);
                break;
            }
            case CMD_UPDATE_FILNISH:
            {
                Synchronize(UpdateLable);
                Synchronize(EnableButton);
                closesocket(sendSock);
                delete this;
                return;
            }
            defalt:
            {
                ShowMessage("Device command don't supoort");
                break;
            }
        }
    }
}
//---------------------------------------------------------------------------
