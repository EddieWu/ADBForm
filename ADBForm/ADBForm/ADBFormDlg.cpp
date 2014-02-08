// ADBFormDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ADBForm.h"
#include "ADBFormDlg.h"
//#include "Test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CADBFormDlg �Ի���




CADBFormDlg::CADBFormDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CADBFormDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CADBFormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSGSHOW, Cot_msginfoshow);
}

BEGIN_MESSAGE_MAP(CADBFormDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CADBFormDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CADBFormDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CADBFormDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CADBFormDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CADBFormDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CADBFormDlg ��Ϣ�������

BOOL CADBFormDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	memset(PWD,0,MAX_PATH);
	::GetCurrentDirectory(MAX_PATH,PWD);
	//AfxMessageBox(PWD);

	UnDoStrBuff = "";

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CADBFormDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CADBFormDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CADBFormDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CADBFormDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char *TestCommand  = "E:\\Project\\06.Acer\\DroidConn\\MoreADB\\adb\\adb shell";
    char *TestCommand2 = TEST_COMMAND;//"F:\\BenKyoU\\CloudWu\\CCPP\\CCpp\\Debug\\CCpp.exe";
	RunObj = new CPipeRun(TestCommand2,true);
	//char str[4096] = {0};
    RunObj->PreparePipe();
	RunObj->RunProgress();
	/*
	while (RunObj->UpdateOutStr(str)==E_READ_INFO_CONTINUE)
	{
		SetDlgItemText(IDC_EDIT1,str);
		UpdateWindow();
	}
	*/
	/*
	CString MSG_Buffer = "";
	GetDlgItemText(IDC_EDIT_MSGSHOW,MSG_Buffer);
	RunObj->UpdateOutStr(str);
	MSG_Buffer.AppendFormat("%s",str);
	SetDlgItemText(IDC_EDIT_MSGSHOW,MSG_Buffer);
	UpdateWindow();
	RunObj->Close();
	*/
	hGetInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)LogInfoShow,this,NULL,&dGetInfoHandleID);
	/*
	DWORD   PreparePipe(void);
	DWORD   RunProgress(void);
	DWORD   UpdateOutStr(char oStr[4096]);
	DWORD   Close(void);
	CString GetRunInfo(void);
	*/
	//CString teststr = RunObj->GetRunInfo();
	//CTest tt;
	//= new CPipeRun(TestCommand,true);
	//OnOK();
}


DWORD CADBFormDlg::LogInfoShow(LPVOID lpParam)
{
	CADBFormDlg *pThis = (CADBFormDlg *)lpParam;

	return pThis->LogInfoShowFunc(0, 0);
}

DWORD CADBFormDlg::LogInfoShowFunc(WPARAM wPamam, LPARAM lParam)
{
	char str[40960] = {0};
	//CString MSG_Buffer = "";
	//SetDlgItemText(IDC_EDIT_MSGSHOW,"TEST...");
	//GetDlgItemText(IDC_EDIT_MSGSHOW,MSG_Buffer);
	UnDoStrBuff = "";
	RunUIChange(true);
	while(E_READ_INFO_ERR != RunObj->UpdateOutStr(str))
	{
		//MSG_Buffer.AppendFormat("%s",str);
		//SetDlgItemText(IDC_EDIT_MSGSHOW,MSG_Buffer);
		//((CEdit *)GetDlgItem(IDC_EDIT_MSGSHOW))->SetSel(MSG_Buffer.GetLength(),MSG_Buffer.GetLength());
		//Cot_msginfoshow
		//m_EdtLog.SetSel(sTmp.GetLength(),sTmp.GetLength());
		//UpdateWindow();
		UpdateLogInfo(-1,str);
		UnDoStrBuff.AppendFormat("%s",str);
	}
	//AfxMessageBox(UnDoStrBuff);
	AnalyseStr(0,TestFlag);
	RunObj->Close();
	delete RunObj;
	RunObj = NULL;
	RunUIChange(false);
	CloseHandle(hGetInfoHandle);
	//RunObj = NULL;

	//
	return 0;
}

void CADBFormDlg::RunUIChange(bool RunFlag)
{
	//if (RunFlag)  //running
	((CButton *)GetDlgItem(IDOK))->EnableWindow(!RunFlag);
	((CButton *)GetDlgItem(IDC_BUTTON1))->EnableWindow(!RunFlag);
	((CButton *)GetDlgItem(IDC_BUTTON2))->EnableWindow(!RunFlag);
	((CButton *)GetDlgItem(IDC_BUTTON3))->EnableWindow(!RunFlag);
	((CButton *)GetDlgItem(IDC_BUTTON4))->EnableWindow(!RunFlag);
}

DWORD CADBFormDlg::DetectDevices(void)
{
	TestFlag = THREAD_DETECT;
	char DetectCMD[100] = {0};
	//sprintf_s(DetectCMD,"%s%s",PWD,CMD_ADB_SHELL);
	sprintf_s(DetectCMD,"%s%s",PWD,CMD_ADB_DEVICES);
	UpdateLogInfo(-1,"Detect devices:\r\n");
	UpdateLogInfo(-1,DetectCMD);
	UpdateLogInfo(-1,"\r\n");
	//debug
	//AfxMessageBox(DetectCMD);
	//
	RunObj = new CPipeRun(DetectCMD,true);
	RunObj->PreparePipe();
	RunObj->RunProgress();

	hGetInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)LogInfoShow,this,NULL,&dGetInfoHandleID);

	return 0;
}

void CADBFormDlg::UpdateLogInfo(DWORD retcode,CString loginfo)
{
	CString MSG_Buffer = "";
	GetDlgItemText(IDC_EDIT_MSGSHOW,MSG_Buffer);
	if (loginfo.GetLength()==0)
	{
		MSG_Buffer.AppendFormat("RETURN CODE[%d]",retcode);
	}
	else
	{
		MSG_Buffer.AppendFormat("%s",loginfo);
		//SetDlgItemText(IDC_EDIT_MSGSHOW,MSG_Buffer);
		Cot_msginfoshow.SetWindowText(MSG_Buffer);
		Cot_msginfoshow.SetSel(MSG_Buffer.GetLength(),MSG_Buffer.GetLength());
	}
	UpdateWindow();
}

void CADBFormDlg::OnBnClickedButton1()  //dut detect test
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DetectDevices();
}

DWORD CADBFormDlg::WLANInfo(void)
{
	TestFlag = THREAD_WIFI_RSSI;
	char DumpSysCMD[100] = {0};
	sprintf_s(DumpSysCMD,"%s%s",PWD,CMD_ADB_SHELL_WLAN);
	UpdateLogInfo(-1,"DumpSys WiFi:\r\n");
	UpdateLogInfo(-1,DumpSysCMD);
	UpdateLogInfo(-1,"\r\n");
	//debug
	//AfxMessageBox(DetectCMD);
	//
	RunObj = new CPipeRun(DumpSysCMD,true);
	RunObj->PreparePipe();
	RunObj->RunProgress();

	hGetInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)LogInfoShow,this,NULL,&dGetInfoHandleID);

	return 0;
}

void CADBFormDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WLANInfo();
	//AfxMessageBox(UnDoStrBuff);
}

DWORD CADBFormDlg::AnalyseStr(double dSpec,E_THREAD_TYPE type)
{
	//AfxMessageBox(UnDoStrBuff);
	//
	switch(type)
	{
	case THREAD_DETECT:
		return AnalyseDevices();
		break;
	case THREAD_WIFI_RSSI:
		return AnalyseRSSI();
		break;
	case THREAD_IPERF_TX:
	    break;
	case THREAD_IPERF_RX:
	    break;
	default:
	    break;
	}

	return E_ADB_SUCCESS;
}

DWORD CADBFormDlg::AnalyseDevices(void)
{
	int iPos = -1;
	CString TestBuff = UnDoStrBuff;
	if (TestBuff.GetLength()==0)
	{
		return E_ADB_ERROR;
	}

	iPos = TestBuff.Find(KEY_ADB_DEVICES);
	if (-1==iPos)
	{
		return E_ADB_DETECT_ERR;
	}
	else
	{
		TestBuff = TestBuff.Mid(iPos+26);
		iPos = TestBuff.Find(KEY_ADB_DEVICES_DUT);
		if (-1==iPos)
		{
			return E_ADB_NODUT;
		}
		else if (-1!=TestBuff.Find(KEY_ADB_DEVICES_DUT,iPos+6))
		{
			//TestBuff = TestBuff.Mid(iPos);
			return E_ADB_MULTIDEVICES;
		}
	}
	AfxMessageBox("Detect OK!");
	return E_ADB_SUCCESS;
}

DWORD CADBFormDlg::AnalyseRSSI(void)
{
	int iPos = -1;
	CString TestBuff = UnDoStrBuff;
	int RSSIArrary[255] ={0};
	if (TestBuff.GetLength()==0)
	{
		return E_ADB_ERROR;
	}

	iPos = TestBuff.Find(KEY_RSSI_RESULT);
	if (-1==iPos)
	{
		return E_ADB_RSSI_ERR;
	}
	else
	{
		TestBuff = TestBuff.Mid(iPos+24);
		SearchRSSIArrary(RSSIArrary,TestBuff);
		MaxRSSI = -130;
		for (int j = 0;j<255;j++)
		{
			if (RSSIArrary[j]==0)
			{
				break;
			}
			if (MaxRSSI<RSSIArrary[j])
			{
				MaxRSSI = RSSIArrary[j];
			}
		}
	}
	CString tt = "";
	tt.Format("Max RSSI[%d]",MaxRSSI);
	AfxMessageBox(tt);
	return E_ADB_SUCCESS;
}
DWORD CADBFormDlg::SearchRSSIArrary(int RSSIArr[],CString mStr)
{
	CString mStrtmp  = mStr;
	char    cRSSI[5] = {0};
	int     iRSSI    = 0;
	int     i        = 0;
	int     iPosReturn = -1;
	while (mStrtmp.GetLength()>150)
	{
		sprintf_s(cRSSI,"%s",mStrtmp.Mid(34,4));
		iRSSI = atoi(cRSSI);
		if (iRSSI<0 && iRSSI >-130)
		{
			RSSIArr[i] = iRSSI;
			i++;
		}
		iPosReturn = mStrtmp.Find(KEY_ENTER);
		mStrtmp = mStrtmp.Mid(iPosReturn+2);
	}
	
	return E_ADB_SUCCESS;
}

//iperf TX
void CADBFormDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IperfTx();
}

DWORD CADBFormDlg::IperfTx(void)
{
	TestFlag = THREAD_IPERF_TX;
	char IperfTxCMD[100] = {0};
	sprintf_s(IperfTxCMD,"%s%s",PWD,CMD_PC_IPERF_TX);
	UpdateLogInfo(-1,"IPERF TX:\r\n");
	UpdateLogInfo(-1,IperfTxCMD);
	UpdateLogInfo(-1,"\r\n");
	//debug
	//AfxMessageBox(DetectCMD);
	//
	RunObj = new CPipeRun(IperfTxCMD,true);
	RunObj->PreparePipe();
	RunObj->RunProgress();

	hGetInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)LogInfoShow,this,NULL,&dGetInfoHandleID);

	return 0;
}
//iperf RX
void CADBFormDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IperfRx();
}
DWORD CADBFormDlg::IperfRx(void)
{
	TestFlag = THREAD_IPERF_RX;
	char IperfRxCMD[100] = {0};
	sprintf_s(IperfRxCMD,"%s%s",PWD,CMD_PC_IPERF_RX);
	UpdateLogInfo(-1,"IPERF RX:\r\n");
	UpdateLogInfo(-1,IperfRxCMD);
	UpdateLogInfo(-1,"\r\n");
	//debug
	//AfxMessageBox(DetectCMD);
	//
	RunObj = new CPipeRun(IperfRxCMD,true);
	RunObj->PreparePipe();
	RunObj->RunProgress();

	hGetInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)LogInfoShow,this,NULL,&dGetInfoHandleID);

	return 0;
}

