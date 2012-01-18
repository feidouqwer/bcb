//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    WSADATA wsd;

    if (WSAStartup(MAKEWORD(2,2),&wsd)!=0)
	{
		return;//ʧ��
	}
}
__fastcall TForm1::~TForm1()
{
    WSACleanup();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnKernelClick(TObject *Sender)
{
    this->openDlg->FileName = "uImage";
    if(openDlg->Execute())
    {
        this->editKernel->Text = this->openDlg->FileName;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnFilesysClick(TObject *Sender)
{
    this->openDlg->FileName = "Filesys.cramfs";
    if(openDlg->Execute())
    {
        this->editFilesys->Text = this->openDlg->FileName;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    this->editKernel->Text = ExtractFilePath(Application->ExeName) + "uImage_ipnc_DM365";
    this->editFilesys->Text = ExtractFilePath(Application->ExeName)+ "cramfsImage_ipnc_DM365";
    for(int i=0; i<SUPPORT_DEVICE_MAX; i++)
    {
        this->progressBar[i] = new TProgressBar(GroupBox2);
        this->progressBar[i]->Width = 100;
        this->progressBar[i]->Height = 18;
        this->progressBar[i]->Parent = GroupBox2;
        this->progressBar[i]->Position = 0;
        this->progressBar[i]->Smooth = true;
        this->progressBar[i]->Position = 0;

        this->lblProgress[i] = new TLabel(this->progressBar[i]);
        this->lblProgress[i]->AutoSize = false;
        this->lblProgress[i]->Width = 25;
        this->lblProgress[i]->Height = 12;
        this->lblProgress[i]->Parent = this->progressBar[i];
        this->lblProgress[i]->Left = 40;
        this->lblProgress[i]->Top = 2;
        this->lblProgress[i]->Transparent = true;
        //this->lblProgress[i]->Caption = "100%";

        this->lblMac[i] = new TLabel(GroupBox2);
        this->lblMac[i]->Width = 100;
        this->lblMac[i]->Height = 15;
        this->lblMac[i]->Parent = GroupBox2;
        this->lblMac[i]->AutoSize = false;
        //this->lblMac[i]->Caption = "12:34:56:78:90:ab";

        this->progressBar[i]->Left = this->lblMac[i]->Left + this->lblMac[i]->Width + 5;
        this->progressBar[i]->Top = 10 + (this->progressBar[i]->Height + 5) * (i);
        this->lblMac[i]->Left = 5;
        this->lblMac[i]->Top = this->progressBar[i]->Top;

        this->lblMac[i]->Hide();
        this->progressBar[i]->Hide();
        this->lblProgress[i]->Hide();
    }
    char ipStr[128];
    if(GetHostIP(ipStr))
    {
        AnsiString str = ipStr;
        lblInfo->Caption = "������IP:" + str;
        this->udpThread = new TUDPThread(0);
        this->tcpServerThread = new TTCPServer(0);
    }
    else
    {
        this->btnKernel->Enabled = false;
        this->btnFilesys->Enabled = false;
        this->btnUpdate->Enabled = false;
        this->ComboBox1->Enabled = false;
        this->lblInfo->Caption = "������192.168.1.*�������ַ";
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
    if(this->udpThread != NULL)
    {        
        TerminateThread((void*)this->udpThread->Handle, 0);
        delete this->udpThread;
    }
    if(this->tcpServerThread != NULL)
    {
        TerminateThread((void*)this->tcpServerThread->Handle, 0);
        delete this->tcpServerThread;
    }
    for(int i=0; i<SUPPORT_DEVICE_MAX; i++)
    {   
        if(this->lblProgress[i] != NULL)
        {
            delete this->lblProgress[i];
            this->lblProgress[i] = NULL;
        }
        if(this->progressBar[i] != NULL)
        {
            delete this->progressBar[i];
            this->progressBar[i] = NULL;
        }
        if(this->lblMac[i] != NULL)
        { 
            delete this->lblMac[i];
            this->lblMac[i] = NULL;
        }
    }
}
//---------------------------------------------------------------------------
int __fastcall TForm1::ShowProgressBar(net_struct *pNetInfo)
{
    if(pNetInfo->dev_num > sizeof(this->progressBar)/sizeof(TProgressBar *))
    {
        ShowMessage("���ܴ�����ô��Ľ�����");
        return false;
    }
    for(int i=0; i<sizeof(this->progressBar)/sizeof(TProgressBar *); i++)
    {
        //if(this->progressBar[i]->Visible == false)
        //    break;
        this->progressBar[i]->Hide();
        this->lblProgress[i]->Hide();
        this->lblMac[i]->Hide();
    }
    for(int i=0; i<pNetInfo->dev_num; i++)
    {
        this->progressBar[i]->Position = 0;
        this->lblProgress[i]->Color = clBtnFace;
        this->lblMac[i]->Color = clBtnFace;
        this->progressBar[i]->Show();
        this->lblProgress[i]->Show();
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnUpdateClick(TObject *Sender)
{
    FILE *kernel, *filesys;
    char ipStr[128];

    if(!this->GetHostIP(ipStr))
        return;
    this->lblInfo->Tag = 0;

    if(this->editKernel->Text == "" || this->editFilesys->Text == "")
    {
        Application->MessageBoxA("��ѡ���ں˺��ļ�ϵͳ��·��!!!", "����", MB_OK);
        return;
    }

    kernel = fopen(this->editKernel->Text.c_str(), "rb");
    filesys = fopen(this->editFilesys->Text.c_str(), "rb");
    if(kernel == NULL || filesys == NULL)
    {
        Application->MessageBoxA("�ں��ļ������ļ�ϵͳ�ļ�·������", "�ļ�·������", MB_OK | MB_ICONERROR);
        if(kernel != NULL)
            fclose(kernel);
        if(filesys != NULL)
            fclose(filesys);
        return;
    }
    
    this->btnKernel->Enabled = false;
    this->btnFilesys->Enabled = false;
    this->btnUpdate->Enabled = false;
    this->ComboBox1->Enabled = false;

    int ret;
    int tryNum = 0;
    int clientNum;
    this->lblClientNum->Caption = "";
    this->lblInfo->Caption = "";
    this->Update();
    do
    {
        this->udpThread->ResetClientNum();
        this->udpThread->Resume();
        BeginToScan();
        clientNum = this->udpThread->GetClientNum();
        this->udpThread->Suspend();
        this->lblClientNum->Caption = "ɨ�赽�豸������" + IntToStr(clientNum);
        this->Update();
        if(clientNum == 0)
        {
            ret = false;
            break;
        }
        else if(clientNum != this->ComboBox1->ItemIndex + 1)
        {
            AnsiString str;
            str = "ѡ����豸����(" + IntToStr(this->ComboBox1->ItemIndex + 1)
                    + ")��ʵ�ʽ���������豸����(" + IntToStr(clientNum) + ")������,�Ƿ��������?";
            if(Application->MessageBoxA(str.c_str(),
                    "�豸��������", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
            {
                ret = false;
                break;
            }
        }
        this->udpThread->GetNetInfo(&netInfo);
        netInfo.dev_num = clientNum;
        netInfo.server_addr.S_un.S_addr = inet_addr(ipStr);
        netInfo.server_port = TCP_SERVER_PORT;
        int analyStatus = this->AnalyNetInfo(&netInfo);
        if(analyStatus == -1)
        {
            ret = false;
            break;
        }
        else if(analyStatus == 0)
        {
            // IP��ַ��random���ظ�,��Ҫ���½���ɨ��
            this->lblInfo->Caption = "IP��ַ��random���ظ�,���½���ɨ��.";
            this->Update();
            continue;
        }
        else
        {
            ret = true;
            break;
        }
    }while(tryNum++ < 5);

    if(ret == true)
    {
        this->ShowProgressBar(&netInfo);
        this->tcpServerThread->ResetSetNo();
        this->lblInfo->Caption = "ɨ��ɹ�,����������...";
        this->Update();
        this->clientDevNum = clientNum;
        this->finishDevNum = 0;
        this->udpThread->SendBroadCastNetInfo(&netInfo);
        this->TimerTCP->Enabled = true;
        //�ɹ�ɨ�赽���豸,������Ҫ�ȴ����豸����������ٴ�ʹ�ܰ���
        // ���߳�ʱ60s����Ȼ���豸û�������ɹ�������ʹ�ܰ���
    }
    else
    {
        if(tryNum >= 5)
        {
            this->lblInfo->Caption = "ɨ��5��,����IP��MAC�����ظ�!!!";
        }
        this->btnKernel->Enabled = true;
        this->btnFilesys->Enabled = true;
        this->btnUpdate->Enabled = true;
        this->ComboBox1->Enabled = true;
    }
}
//---------------------------------------------------------------------------
int __fastcall TForm1::IsMacRepeat(net_struct *pNetInfo)
{
    for(int i=0; i<pNetInfo->dev_num; i++)
    {
        for(int j=i+1; j<pNetInfo->dev_num; j++)
        {
            if(memcmp(pNetInfo->node[i].mac, pNetInfo->node[j].mac,
                    sizeof(pNetInfo->node[i].mac)) == 0)
            {
                return 1;   // MAC repeat
            }
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
int __fastcall TForm1::IsRandomRepeat(net_struct *pNetInfo)
{
    for(int i=0; i<pNetInfo->dev_num; i++)
    {
        for(int j=i+1; j<pNetInfo->dev_num; j++)
        {
            if(pNetInfo->node[i].random == pNetInfo->node[j].random)
            {
                return 1;   // MAC repeat
            }
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
int __fastcall TForm1::AnalyNetInfo(net_struct *pNetInfo)
{
    if(pNetInfo == NULL)
        return -1;
    int macRepeat = IsMacRepeat(pNetInfo);
    int randomRepeat = IsRandomRepeat(pNetInfo);
    if(macRepeat && randomRepeat)
    {
        // MAC��random���ظ�,������Ҫ���·��͹㲥���ٽ���ɨ��
        return 0;
    }
    else if(macRepeat && randomRepeat == 0)
    {
        const unsigned char mac[6] = {0x00, 0x05, 0x3B, 0xF7, 0x33, 0x11};
        for(int i=0; i<pNetInfo->dev_num; i++)
        {
            memcpy(pNetInfo->node[i].mac, mac, sizeof(pNetInfo->node[i].mac));
            pNetInfo->node[i].mac[5] += i;
        }
        pNetInfo->use_which_compare = 1; //�豸����random���бȶ���ѡ���Լ���MAC��ַ
    }
    else
    {
        pNetInfo->use_which_compare = 0; // �豸����MAC���бȶ�
    }
    return 1;
}
//---------------------------------------------------------------------------
int __fastcall TForm1::BeginToScan()
{
    if(this->udpThread->SendBroadCastPacket() == SOCKET_ERROR)
    {
         this->lblInfo->Caption = "����UDP�㲥��ʧ��";
         return false;
    }
    Sleep(3000);

    return true;
}
bool __fastcall TForm1::GetHostIP(char *ipStr)
{
    struct hostent *P_Host;
    char Computer_Name[128] = "";

    gethostname(Computer_Name, sizeof(Computer_Name));//��ȡָ�������������
    if(Computer_Name[0] == '\0')
    {
        MessageBox(NULL, "����������ϣ��޷����������������ϵ�������Ա","��ʾ",MB_ICONASTERISK);
        return false;
    }

    P_Host = gethostbyname(Computer_Name);
    if(P_Host == NULL)
    {
        MessageBox(NULL, "����������ϣ��޷���ȡIP������ϵ����Ա","��ʾ",MB_ICONASTERISK);
        return false;
    }
    char **pptr = P_Host->h_addr_list;
    for(; *pptr != NULL; pptr++)
    {
        //ShowMessage(inet_ntoa(*(in_addr *)*pptr));
        if(strncmp(inet_ntoa(*(in_addr *)*pptr), "192.168.1.", strlen("192.168.1.")) == 0)
        {
            strcpy(ipStr, inet_ntoa(*((in_addr *)P_Host->h_addr)));
            return true;
        }
    }
    return false;
}
int __fastcall TForm1::GetMacFromIp(char *ipStr, char *mac)
{
    if(ipStr == NULL || mac == NULL)
        return false;
    for(int i=0; i<sizeof(netInfo.dev_num); i++)
    {
        if(strcmp(inet_ntoa(netInfo.node[i].ip_addr), ipStr) == 0)
        {
            memcpy(mac, netInfo.node[i].mac, sizeof(netInfo.node[i].mac));
            return true;
        }
    }
    return false;
}

void __fastcall TForm1::TimerTCPTimer(TObject *Sender)
{
    this->TimerTCP->Enabled = false;
    this->btnKernel->Enabled = true;
    this->btnFilesys->Enabled = true;
    this->btnUpdate->Enabled = true;
    this->ComboBox1->Enabled = true;
    this->lblInfo->Caption = "�豸������ʱ,����������!!!";
}
//---------------------------------------------------------------------------

