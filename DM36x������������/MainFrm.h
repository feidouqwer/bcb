//---------------------------------------------------------------------------

#ifndef MainFrmH
#define MainFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <winsock.h>
#include "UDPThread.h"
#include "TCPThread.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

#define SUPPORT_DEVICE_MAX  16
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *editKernel;
    TEdit *editFilesys;
    TButton *btnKernel;
    TButton *btnFilesys;
    TOpenDialog *openDlg;
    TButton *btnUpdate;
    TLabel *lblInfo;
    TLabel *lblClientNum;
    TGroupBox *GroupBox2;
    TLabel *Label3;
    TGroupBox *GroupBox3;
    TLabel *Label4;
    TComboBox *ComboBox1;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TTimer *TimerTCP;
    void __fastcall btnKernelClick(TObject *Sender);
    void __fastcall btnFilesysClick(TObject *Sender);
    void __fastcall btnUpdateClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall TimerTCPTimer(TObject *Sender);
private:	// User declarations
    net_struct netInfo;
    TUDPThread *udpThread;
    TTCPServer *tcpServerThread;

public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    __fastcall TForm1::~TForm1();
    int __fastcall AnalyNetInfo(net_struct *pNetInfo);
    int __fastcall IsMacRepeat(net_struct *pNetInfo);
    int __fastcall IsRandomRepeat(net_struct *pNetInfo);
    int __fastcall BeginToScan();
    int __fastcall ShowProgressBar(net_struct *pNetInfo);
    bool __fastcall GetHostIP(char *ipStr);
    int __fastcall GetMacFromIp(char *ipStr, char *mac);
    int clientDevNum;
    int finishDevNum;
    TProgressBar *progressBar[SUPPORT_DEVICE_MAX];
    TLabel *lblProgress[SUPPORT_DEVICE_MAX];
    TLabel *lblMac[SUPPORT_DEVICE_MAX];
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
