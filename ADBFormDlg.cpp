// ADBFormDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ADBForm.h"
#include "ADBFormDlg.h"
//#include "Test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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


// CADBFormDlg 对话框




CADBFormDlg::CADBFormDlg(CWnd* pParent /*=NULL*/)
: CDialog(CADBFormDlg::IDD, pParent)
, cPCIPaddr(_T(""))
, cDUTIPaddr(_T(""))
, m_ckRSSI(FALSE)
, m_ckThpTx(FALSE)
, m_ckThpRx(FALSE)
, m_ckGPS(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CADBFormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSGSHOW, Cot_msginfoshow);
	DDX_Text(pDX, IDC_EDIT_PCIP, cPCIPaddr);
	DDX_Text(pDX, IDC_EDIT_DUTIP, cDUTIPaddr);
	DDX_Control(pDX, IDC_STATIC_INFO, m_stTestStatus);
	DDX_Control(pDX, IDC_PROGRESS_ALL, ProcessAll);
	DDX_Control(pDX, IDC_BUTTON_START, m_StatAll);
	DDX_Control(pDX, IDC_CHECK_RSSI, cbRSSI);
	DDX_Check(pDX, IDC_CHECK_RSSI, m_ckRSSI);
	DDX_Control(pDX, IDC_CHECK_THPTX, cbThpTx);
	DDX_Check(pDX, IDC_CHECK_THPTX, m_ckThpTx);
	DDX_Control(pDX, IDC_CHECK_THPRX, cbThpRx);
	DDX_Check(pDX, IDC_CHECK_THPRX, m_ckThpRx);
	DDX_Control(pDX, IDC_CHECK_GPS, cbGPS);
	DDX_Check(pDX, IDC_CHECK_GPS, m_ckGPS);
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
	ON_BN_CLICKED(IDC_BUTTON5, &CADBFormDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CADBFormDlg::OnBnClickedButton6)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CADBFormDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON7, &CADBFormDlg::OnBnClickedButton7)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CADBFormDlg 消息处理程序

BOOL CADBFormDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	memset(PWD,0,MAX_PATH);
	::GetCurrentDirectory(MAX_PATH,PWD);
	//AfxMessageBox(PWD);
	m_StatAll.SetFocus();

	UnDoStrBuff = "";
	hPCIperfServer = NULL;
	LoadDefaultConfig();

	//action adb server
	//DetectDevices();
	ActionADBServer();
	// log file crate
	bLogPre = LogPrepare();
	//debug
	/*
	CFont    m_font;
	m_font.CreateFont(
	200,                        // nHeight
	50,                         // nWidth
	0,                         // nEscapement
	0,                         // nOrientation
	FW_NORMAL,                 // nWeight
	FALSE,                     // bItalic
	FALSE,                     // bUnderline
	0,                         // cStrikeOut
	ANSI_CHARSET,              // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	"Arial"              // lpszFacename
	);
	m_StatAll.SetFont(&m_font,FALSE);
	*/
	m_StatAll.SetFlat(FALSE);
	m_StatAll.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 32);
	m_StatAll.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_StatAll.SetIcon(IDI_ICON_START);
	m_StatAll.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(208,208,208)); 
	m_StatAll.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(208,208,208));
	BOOL ConfigReady = ConfigIsReady();
	m_StatAll.EnableWindow(ConfigReady);
	//m_StatAll.SetColor(CButtonST::BTNST_COLOR_FG_OUT,32);
	m_StatAll.SetTooltipText(_T("Start testing [开始测试] ..."));
	UpdateTestStatus(ConfigReady?STATUS_READY:STATUS_UNKOWN);
	//

	SetWindowSize(DBGFlag==DEBUG_FLAG?E_WND_DEBUG:E_WND_NORMAL);

	// DUT ip get automatically,disable 
	((CEdit *)GetDlgItem(IDC_EDIT_DUTIP))->EnableWindow(iDutIpAuto==0);

	/*
	CRect rect;
	m_stTestStatus.GetWindowRect(&rect);
	int het = rect.Height();
	int wid = rect.Width();
	((CButton *)GetDlgItem(IDC_BUTTON_START))->GetWindowRect(&rect);
	het = rect.Height();
	wid = rect.Width();*/
	return FALSE;
	//return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CADBFormDlg::UpdateTestStatus(E_TEST_STATUS status,DWORD errcode)
{
	CRect rect;
	m_stTestStatus.GetWindowRect(&rect);
	int  SignalFontWidth = 0;
	char StatusText[12] = {0};
	switch(status)
	{
	case STATUS_READY:
		SignalFontWidth = rect.Width() / 8;
		sprintf_s(StatusText,12,"%s","READY");
		m_stTestStatus.SetBkColor(COLOR_READY);
		//Sleep(2000);
		break;
	case STATUS_RUNNING:
		SignalFontWidth = rect.Width() / 10-1;
		sprintf_s(StatusText,12,"%s","RUNNING");
		m_stTestStatus.SetBkColor(COLOR_RUN);
		//Sleep(2000);
		break;
	case STATUS_PASS:
		SignalFontWidth = rect.Width() / 6;
		sprintf_s(StatusText,12,"%s","PASS");
		m_stTestStatus.SetBkColor(COLOR_PASS);
		//Sleep(2000);
		break;
	case STATUS_FAIL:
		SignalFontWidth = rect.Width() / 8;
		sprintf_s(StatusText,12,"%s[%d]","FAIL",errcode);
		m_stTestStatus.SetBkColor(COLOR_FAIL);
		break;
	default:
		SignalFontWidth = rect.Width() / 10-1;
		sprintf_s(StatusText,12,"%s","CONFIG...");
		m_stTestStatus.SetBkColor(COLOR_UNCONFIG);
		//Sleep(2000);
		break;
	}
	m_stTestStatus.SetStaticFont(_T("Arial"), SignalFontWidth, rect.Height());
	m_stTestStatus.SetTextColor(COLOR_WHITE);
	m_stTestStatus.SetWindowText(StatusText);

}
/************************************************************************/
/* 配置是否准备OK                                                        */
/************************************************************************/
BOOL CADBFormDlg::ConfigIsReady(void)
{
	if (!bLogPre)
	{
		return FALSE;
	}
	if (cPCIPaddr.IsEmpty()||cDUTIPaddr.IsEmpty()||cPrj.IsEmpty()||cWoid.IsEmpty())
	{
		return FALSE;
	}
	if (!(m_ckRSSI || m_ckThpTx || m_ckThpRx || m_ckGPS))
	{
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* LOG文件夹生成                                                         */
/************************************************************************/
BOOL  CADBFormDlg::LogPrepare(void)
{
	// init Lite Form
	m_LiteTestSN   = "";
	i_LiteTestRSSI = 0;
	i_LiteTestThpT = 0;
	i_LiteTestThpR = 0;
	i_LiteTestGPSCount = 0;
	i_LiteTestGPSCN = 0.0;
	// log file crate
	m_sLogFilePath     = "";
	m_sLogFilePathFull = "";
	m_sLogFilePath.Format("%s\\log\\%s",PWD,GetCurTimeStr(0));
	//create Date Log folder
	if (!PathIsDirectory(m_sLogFilePath))
	{
		if(!CreateDirectory(m_sLogFilePath,NULL))
		{
			AfxMessageBox("create Date Folder failure!");
			return FALSE;
		}
	}

	m_sLogFilePathFull.Format("%s\\_temp_.log",m_sLogFilePath);
	hLogFile.SetFilePath(m_sLogFilePath);
	bLogFileNormal = hLogFile.Open(m_sLogFilePathFull,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite);
	if(!bLogFileNormal)
	{
		AfxMessageBox("create log file failure!");
		return FALSE;
	}
	else
	{
		hLogFile.SeekToEnd();
	}
	// 20140324 WuJian add Start
	hLogFileLite.SetFilePath(m_sLogFilePath);
	m_sLogFilePathLiteFull.Format("%s\\%s.csv",m_sLogFilePath,GetCurTimeStr(0));
	bLogFileNormalLite = hLogFileLite.Open(m_sLogFilePathLiteFull,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite);
	if(!bLogFileNormalLite)
	{
		AfxMessageBox("create log(LITE) file failure!");
		return FALSE;
	}
	else
	{
		hLogFileLite.SeekToEnd();
		m_LogLiteStr = "";
	}
	// 20140324 WuJian add End
	return TRUE;
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CADBFormDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CADBFormDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CADBFormDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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
	CString IPERFStr = "";
	char IPERFSeaWord[12] = {0};
	sprintf_s(IPERFSeaWord,11,"0.0-%d",iTransmitTime);
	if(TestFlag == THREAD_ADB_SERVER)
	{
		RunUIChange(true);
		UpdateTestStatus(STATUS_UNKOWN);
	}
	if(TestFlag==THREAD_IPERF_TX)
	{
		hPCIperfServer = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	if(TestFlag==THREAD_IPERF_RX)
	{
		hDUTIperfServer = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	/*
	if(TestFlag == THREAD_DETECT)
	{
	hDetectDevice = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	*/
	UnDoStrBuff = "";
	//RunUIChange(true);
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
		IPERFStr.Format("%s",str);
		GPSTestStr.AppendFormat("%s",str);
		if (TestFlag==THREAD_GPS_RX && GPSStrValid())
		{
			//AfxMessageBox(GPSTestStr);
			dStartRet = AnalyseStr(0,TestFlag);
			if (dStartRet == E_ADB_SUCCESS)//|| dStartRet == E_ADB_GPS_CNLESS)
			{
				SetEvent(hGPSTest);
				break;
			}
			else
			{
				GPSTestStr = "";
			}
		}
		if(TestFlag==THREAD_IPERF_TX && IPERFStr.Find(IPERFSeaWord)!=-1)
		{
			dStartRet = AnalyseStr(0,TestFlag,IPERFStr);
			if (dStartRet == E_ADB_SUCCESS || dStartRet == E_ADB_THPTX_LESS)
			{
				SetEvent(hTHPTxTest);
				break;
			}
		}
		if (TestFlag==THREAD_IPERF_RX && IPERFStr.Find(IPERFSeaWord)!=-1)
		{
			dStartRet = AnalyseStr(0,TestFlag,IPERFStr);
			if (dStartRet == E_ADB_SUCCESS || dStartRet == E_ADB_THPRX_LESS)
			{
				SetEvent(hTHPRxTest);
				break;
			}
		}
		/*
		if(TestFlag==THREAD_IPERF_TX)
		{
		if(WAIT_OBJECT_0==WaitForSingleObject(hPCIperfServer,10))
		{
		break;
		}
		}
		*/
	}
	/*
	if (TestFlag==THREAD_IPERF_RX)
	{
	SetEvent(hNoLogThread);
	}
	*/
	//AfxMessageBox(UnDoStrBuff);
	if (TestFlag==THREAD_IPERF_RX)
	{
		SetEvent(hDUTIperfServer);
		((CButton *)GetDlgItem(IDC_BUTTON6))->EnableWindow(true);
		UpdateLogInfo(-1,"IPERF RX(DUT):DONE!");
		UpdateLogInfo(-1,"\r\n");
		// 防止ADB内存泄漏
		if(WAIT_OBJECT_0==WaitForSingleObject(hDUTIperfServer,10))
		{
			if (DevRunObj!=NULL)
			{
				DevRunObj->DestroyProcess();
			}
		}
	}
	if (TestFlag==THREAD_GPS_RX)
	{
		((CButton *)GetDlgItem(IDC_BUTTON7))->EnableWindow(true);
		SetEvent(hGPSTest);
		//LOGCAT if don't killed will crash
		//这里容易ADB内存泄漏
		if (RunObj!=NULL)
		{
			RunObj->DestroyProcess();
		}
		UpdateLogInfo(-1,"GPS TEST:DONE!");
		UpdateLogInfo(-1,"\r\n");
	}
	dStartRet = AnalyseStr(0,TestFlag,IPERFStr);
	if(TestFlag == THREAD_ADB_SERVER)
	{
		//must be fix code
		UpdateTestStatus(STATUS_READY);
		RunUIChange(false);
	}
	if (TestFlag==THREAD_WIFI_RSSI)
	{
		SetEvent(hRSSITest);
	}
	if (RunObj!=NULL)
	{
		RunObj->DestroyProcess();
	}
	//
	if (TestFlag==THREAD_IPERF_TX)
	{
		SetEvent(hTHPTxTest);
	}
	if (TestFlag==THREAD_IPERF_RX)
	{
		SetEvent(hTHPRxTest);
	}
	RunObj->Close();
	delete RunObj;
	RunObj = NULL;
	//RunUIChange(false);
	CloseHandle(hGetInfoHandle);
	//detect device done
	SetEvent(hDetectDevice);
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
	if (TestFlag!=THREAD_IPERF_RX)
	{
		((CButton *)GetDlgItem(IDC_BUTTON4))->EnableWindow(!RunFlag);
		((CButton *)GetDlgItem(IDC_BUTTON6))->EnableWindow(!RunFlag);
	}
	if (TestFlag!=THREAD_IPERF_TX)
	{
		((CButton *)GetDlgItem(IDC_BUTTON5))->EnableWindow(!RunFlag);
	}
	((CButton *)GetDlgItem(IDC_BUTTON7))->EnableWindow(!RunFlag);
	//((CButton *)GetDlgItem(IDC_COMBO_PRJ))->EnableWindow(!RunFlag);
	//((CButton *)GetDlgItem(IDC_COMBO_WO))->EnableWindow(!RunFlag);
	((CEdit *)GetDlgItem(IDC_EDIT_PCIP))->EnableWindow(!RunFlag);
	if(1!=iDutIpAuto)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_DUTIP))->EnableWindow(!RunFlag);
	}
	((CButton *)GetDlgItem(IDC_BUTTON_START))->EnableWindow(!RunFlag);
	cbRSSI.EnableWindow(!RunFlag);
	cbThpTx.EnableWindow(!RunFlag);
	cbThpRx.EnableWindow(!RunFlag);
	cbGPS.EnableWindow(!RunFlag);
}

DWORD CADBFormDlg::DetectDevices(void)
{
	TestFlag = THREAD_DETECT;
	char DetectCMD[MAX_PATH] = {0};
	//sprintf_s(DetectCMD,"%s%s",PWD,CMD_ADB_SHELL);
	if (iGetSNType==0)
	{
		sprintf_s(DetectCMD,"%s%s",PWD,CMD_ADB_DEVICES);
	}
	else
	{
		sprintf_s(DetectCMD,"%s%s",PWD,CMD_ADB_GETSN);
	}
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
//THREAD_ADB_SERVER
//BUG FIX
DWORD CADBFormDlg::ActionADBServer(void)
{
	TestFlag = THREAD_ADB_SERVER;
	char DetectCMD[MAX_PATH] = {0};
	sprintf_s(DetectCMD,"%s%s",PWD,CMD_ADB_DEVICES);

	RunObj = new CPipeRun(DetectCMD,true);
	RunObj->PreparePipe();
	RunObj->RunProgress();

	hGetInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)LogInfoShow,this,NULL,&dGetInfoHandleID);

	return 0;
}
void CADBFormDlg::UpdateLogInfo(DWORD retcode,CString loginfo)
{
	CString MSG_Buffer = "";
	CString ForLogFile;
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
		//hLogFile.Write(loginfo,loginfo.GetLength());
		LogFileUpdate(loginfo);
	}
	UpdateWindow();
	if (retcode==5)
	{
		if (bLogFileNormal)
		{
			ForLogFile.Format("\r\nRETURN CODE[%d]\r\n",dStartRet);
			LogFileUpdate(ForLogFile);
			hLogFile.Close();
			bLogFileNormal = FALSE;
		}
		if (bLogFileNormalLite)
		{
			WriteLiteLog123();
			hLogFileLite.Close();
			bLogFileNormalLite = FALSE;
		}
	}
}

void CADBFormDlg::OnBnClickedButton1()  //dut detect test
{
	// TODO: 在此添加控件通知处理程序代码
	DetectDevices();
}

DWORD CADBFormDlg::WLANInfo(void)
{
	TestFlag = THREAD_WIFI_RSSI;
	char DumpSysCMD[MAX_PATH] = {0};
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
	// TODO: 在此添加控件通知处理程序代码
	WLANInfo();
	//AfxMessageBox(UnDoStrBuff);
}

DWORD CADBFormDlg::AnalyseStr(double dSpec,E_THREAD_TYPE type,CString KeyStr)
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
		//case THREAD_IPERF_DUT_TX:
		return AnalyseTHPTx(KeyStr);
		break;
	case THREAD_IPERF_RX:
		//case THREAD_IPERF_DUT_RX:
		return AnalyseTHPRx(KeyStr);
		break;
	case THREAD_GPS_RX:
		return AnalyseGPS();
	default:
		break;
	}

	return E_ADB_SUCCESS;
}
DWORD CADBFormDlg::AnalyseTHPTx(CString ResultStr)
{
	int iPosS = -1;
	int iPosE = -1;
	int iResult;
	CString TestBuff = ResultStr;
	if (TestBuff.GetLength()==0)
	{
		return E_ADB_ERROR;
	}
	iPosS = TestBuff.Find(KEY_IPERF_KBYTES);
	iPosE = TestBuff.Find(KEY_IPERF_KBITSPER);
	if (iPosS==-1 || iPosE==-1)
	{
		return E_ADB_THPTX_ERR;
	}
	else
	{
		iResult = atoi(TestBuff.Mid((iPosS+8),(iPosE-iPosS-4)));
		// 20140324 WuJian modify Start
		i_LiteTestThpT = iResult;
		// 20140324 WuJian modify End
	}
	if (iResult<(iThpTxSpec*1000))
	{
		return E_ADB_THPTX_LESS;
	}
	return E_ADB_SUCCESS;
}
DWORD CADBFormDlg::AnalyseTHPRx(CString ResultStr)
{
	int iPosS = -1;
	int iPosE = -1;
	CString TestBuff = ResultStr;
	int iResult;
	if (TestBuff.GetLength()==0)
	{
		return E_ADB_ERROR;
	}
	iPosS = TestBuff.Find(KEY_IPERF_KBYTES);
	iPosE = TestBuff.Find(KEY_IPERF_KBITSPER);
	if (iPosS==-1 || iPosE==-1)
	{
		return E_ADB_THPRX_ERR;
	}
	else
	{
		iResult = atoi(TestBuff.Mid((iPosS+8),(iPosE-iPosS-4)));
		// 20140324 WuJian modify Start
		i_LiteTestThpR = iResult;
		// 20140324 WuJian modify End
	}
	if (iResult<(iThpRxSpec*1000))
	{
		return E_ADB_THPRX_LESS;
	}
	return E_ADB_SUCCESS;
}
DWORD CADBFormDlg::AnalyseDevices(void)
{
	int iPos = -1;
	CString TestBuff = UnDoStrBuff;
	CString TestSN   = "";
	if (TestBuff.GetLength()==0)
	{
		return E_ADB_ERROR;
	}

	if (iGetSNType==0)
	{
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
		TestSN = TestBuff.Mid(0,iPos);
		TestSN.TrimLeft();
		TestSN.TrimRight();
		LogFileUpdate(TestSN,1);
		// 20140324 WuJian modify Start
		m_LiteTestSN = TestSN;
		// 20140324 WuJian modify End
	}
	else
	{
		TestBuff.TrimLeft();
		TestSN = TestBuff.Mid(0,25);
		TestSN.TrimRight();
		iPos = TestBuff.Find(KEY_ADB_D_NOTFOUND);
		if (-1!=iPos)
		{
			return E_ADB_NODUT;
		}
		if (TestSN.GetLength()<10)
		{
			return E_ADB_BADSN;
		}
		else
		{
			LogFileUpdate(TestSN,1);
		}
	}
	//AfxMessageBox("Detect OK!");
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

	//find ip address
	if (iDutIpAuto==1)
	{
		//run state check
		/*
		interface wlan0 runState=Running   OK
		interface wlan0 runState=Starting  NG
		interface wlan0 runState=Stopped   NG
		*/
		iPos = TestBuff.Find(KEY_WLAN_RUNSTATE);
		if (-1==iPos)
		{
			return E_ADB_RSSI_STOP;
		}
		int IPaddrS = TestBuff.Find(KEY_WLAN_IPADDR);
		int IPaddrE = TestBuff.Find(KEY_WLAN_GATEWAY);
		if (IPaddrS==-1 || IPaddrE==-1 || (IPaddrE-IPaddrS<10))
		{
			return E_ADB_RSSI_BADIPADDR;
		}
		else
		{
			//cDUTIPaddr = TestBuff.Mid(IPaddrS+7,IPaddrE-IPaddrS-8);
			cDUTIPaddr = TestBuff.Mid(IPaddrS+16,IPaddrE-IPaddrS-8);
			int ixgPos = cDUTIPaddr.Find("/");
			cDUTIPaddr = cDUTIPaddr.Mid(0,ixgPos);
			//AfxMessageBox(cDUTIPaddr);
		}	
		/*
		SSID: HWTEST, BSSID: 8c:21:0a:42:5c:1a, MAC: 00:08:22:0a:b4:fb, Supplicant state: COMPLETED, RSSI: -27, Link speed: 54, Net ID: 0
		ipaddr 192.168.25.101 gateway 192.168.25.1 netmask 255.255.255.0 dns1 192.168.25.1 dns2 8.8.8.8 DHCP server 192.168.25.1 lease 7200 seconds
		*/
		// SSID check
		// RSSI check
		// Fetch IP
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
	//CString tt = "";
	//tt.Format("Max RSSI[%d]",MaxRSSI);
	//AfxMessageBox(tt);
	// 20140324 WuJian modify Start
	i_LiteTestRSSI = MaxRSSI;
	// 20140324 WuJian modify End
	if (MaxRSSI < iRSSISpec)
	{
		return E_ADB_RSSI_LESS;
	}
	return E_ADB_SUCCESS;
}
/*
打印log样式如下：
02-11 11:19:05.933 I/ACERGPS ( 1491): GPS_EVENT_STARTED
02-11 11:15:31.758 I/ACERGPS ( 1491): GPS_EVENT_SATELLITE_STATUS
02-11 11:15:31.760 I/ACERGPS ( 1491): satelliteCount = 5
02-11 11:15:31.760 I/ACERGPS ( 1491): gps0 C/N: 0.0
02-11 11:15:31.760 I/ACERGPS ( 1491): gps1 C/N: 0.0
02-11 11:15:31.760 I/ACERGPS ( 1491): gps2 C/N: 0.0
02-11 11:15:31.760 I/ACERGPS ( 1491): gps3 C/N: 0.0
02-11 11:15:31.760 I/ACERGPS ( 1491): gps4 C/N: 0.0
02-11 11:20:02.950 I/ACERGPS ( 1491): gps test finish!
*/
DWORD CADBFormDlg::AnalyseGPS(void)
{
	int iPos = -1;
	//bool CNless = false;
	CString TestBuff = GPSTestStr;//UnDoStrBuff;
	//AfxMessageBox(GPSTestStr);
	if (TestBuff.GetLength()==0)
	{
		return E_ADB_ERROR;
	}
	iPos = TestBuff.Find(KEY_GPS_SATECNT);
	if (-1==iPos)
	{
		return E_ADB_GPS_ERR;
	}
	TestBuff = TestBuff.Mid(iPos+17);

	//satellite count
	char    cSateNum[3] = {0};
	int     iSateNum    = 0;
	char    cSateCN[6]  = {0};
	float   fSateCN[10] = {0};
	int     i           = 0;
	CString CNKeyWord   = "";
	while (TestBuff.GetLength()>55)
	{
		if (i==0)
		{
			sprintf_s(cSateNum,"%s",TestBuff.Mid(0,2));
			iSateNum = atoi(cSateNum);
			if (iSateNum<iGPSSatCnt)
			{
				return E_ADB_GPS_SATLESS;
			}
			// 20140324 WuJian modify Start
			i_LiteTestGPSCount = iSateNum;
			// 20140324 WuJian modify End
		}
		CNKeyWord.Format("gps%d",i);
		iPos = TestBuff.Find(CNKeyWord);
		if (iPos==-1)
		{
			return E_ADB_GPS_FORMAT;
		}
		TestBuff = TestBuff.Mid(iPos);
		sprintf_s(cSateCN,"%s",TestBuff.Mid(10,5));
		fSateCN[i]  = atof(cSateCN);
		if (fSateCN[i] >= iGPSCNSpec)
		{
			//CNless = true;
			// 20140324 WuJian modify Start
			i_LiteTestGPSCN = fSateCN[i];
			// 20140324 WuJian modify End
			return E_ADB_SUCCESS;
		}
		TestBuff = TestBuff.Mid(iPos+12);
		i++;
	}
	//if (CNless)
	//{
	//}
	return E_ADB_GPS_CNLESS;

	//return E_ADB_SUCCESS;
}
// check GPS str is valid
/*
01-11 08:11:04.261 I/ACERGPS ( 2626): GPS_EVENT_SATELLITE_STATUS

01-11 08:11:04.261 I/ACERGPS ( 2626): satelliteCount = 3

01-11 08:11:04.261 I/ACERGPS ( 2626): gps0 C/N: 33.0

01-11 08:11:04.261 I/ACERGPS ( 2626): gps1 C/N: 33.0

01-11 08:11:04.261 I/ACERGPS ( 2626): gps2 C/N: 34.0

01-11 08:11:05.261 I/ACERGPS ( 2626): GPS_EVENT_SATELLITE_STATUS

01-11 08:11:05.261 I/ACERGPS ( 2626): satelliteCount = 3

01-11 08:11:05.261 I/ACERGPS ( 2626): gps0 C/N: 35.0

01-11 08:11:05.261 I/ACERGPS ( 2626): gps1 C/N: 34.0

01-11 08:11:05.262 I/ACERGPS ( 2626): gps2 C/N: 34.0
*/
BOOL CADBFormDlg::GPSStrValid(void)
{
	//GPSTestStr = "";
	CString TestBuff = GPSTestStr;
	if(TestBuff.GetLength()<56)
	{
		return FALSE;
	}
	int iPos = -1;
	iPos = TestBuff.Find(KEY_GPS_SATECNT);
	if (-1==iPos)
	{
		return FALSE;
	}
	TestBuff = TestBuff.Mid(iPos+17);

	char    cSateNum[3] = {0};
	int     iSateNum    = 0;
	CString CNKeyWord   = "";
	sprintf_s(cSateNum,"%s",TestBuff.Mid(0,2));
	iSateNum = atoi(cSateNum);
	if (iSateNum<iGPSSatCnt)
	{
		return FALSE;
	}
	else
	{
		CNKeyWord.Format("gps%d",iSateNum-1);
		//AfxMessageBox(CNKeyWord);
		iPos = TestBuff.Find(CNKeyWord);
		if(iPos!=-1)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
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
	// TODO: 在此添加控件通知处理程序代码
	IperfTx();
}

/************************************************************************
PC端启动IPERF server，没有超时，测试完成后需要手动关闭（destroyprocess）

************************************************************************/
DWORD CADBFormDlg::IperfTx(void)
{
	TestFlag = THREAD_IPERF_TX;
	char IperfTxCMD[MAX_PATH] = {0};
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
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	IperfRx();
}
DWORD CADBFormDlg::IperfRx(void)
{
	TestFlag = THREAD_IPERF_RX;
	char IperfRxCMD[MAX_PATH] = {0};
	CString IPReplace = CMD_PC_IPERF_RX;
	IPReplace.Replace("DUTIP",cDUTIPaddr);
	IPReplace.AppendFormat("%d -T %d",iTransmitTime,iIntervalTime);
	sprintf_s(IperfRxCMD,"%s%s",PWD,IPReplace/*CMD_PC_IPERF_RX*/);
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


void CADBFormDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	IperfDutTx();
}

void CADBFormDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	IperfDutRx();
}

//Device action 
DWORD CADBFormDlg::NoLogInfoDo(LPVOID lpParam)
{
	CADBFormDlg *pThis = (CADBFormDlg *)lpParam;

	return pThis->NoLogInfoDoFunc(0, 0);
}

DWORD CADBFormDlg::NoLogInfoDoFunc(WPARAM wPamam, LPARAM lParam)
{
	char str[40960] = {0};
	CString DutStr = "DEVICE LOG START[\r\n";
	//UnDoStrBuff = "";
	//RunUIChange(true);
	hNoLogThread = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (TestFlag2==THREAD_IPERF_DUT_TX)
	{
		((CButton *)GetDlgItem(IDC_BUTTON5))->EnableWindow(false);
	}
	if (TestFlag2==THREAD_IPERF_DUT_RX)
	{
		((CButton *)GetDlgItem(IDC_BUTTON6))->EnableWindow(false);
	}
	//
	while(E_READ_INFO_ERR != DevRunObj->UpdateOutStr(str))
	{
		//UpdateLogInfo(-1,str);
		//UnDoStrBuff.AppendFormat("%s",str);
		DutStr.AppendFormat("%s",str);
		if(WAIT_OBJECT_0==WaitForSingleObject(hNoLogThread,10))
		{
			break;
		}
	}
	//AnalyseStr(0,TestFlag);
	//AfxMessageBox("!!!");
	DutStr.AppendFormat("]DEVICE LOG END\r\n");
	UpdateLogInfo(-1,DutStr);
	if (TestFlag2==THREAD_IPERF_DUT_TX)
	{
		SetEvent(hPCIperfServer);
		((CButton *)GetDlgItem(IDC_BUTTON5))->EnableWindow(true);
		UpdateLogInfo(-1,"IPERF TX(DUT):DONE!");
		UpdateLogInfo(-1,"\r\n");
		if(WAIT_OBJECT_0==WaitForSingleObject(hPCIperfServer,10))
		{
			if (RunObj!=NULL)
			{
				RunObj->DestroyProcess();
				//RunObj->Close();
				//delete RunObj;
				//RunObj = NULL;
			}
		}
		//SetEvent(hLogThread);  //tx test give a signal to PC
		/*
		if (RunObj!=NULL && hGetInfoHandle!=NULL)
		{
		CloseHandle(hGetInfoHandle);
		delete RunObj;
		RunObj = NULL;
		RunUIChange(false);
		}
		*/
	}
	if (TestFlag==THREAD_IPERF_RX/*THREAD_IPERF_DUT_RX*/)
	{
		//((CButton *)GetDlgItem(IDC_BUTTON6))->EnableWindow(true);
		//UpdateLogInfo(-1,"IPERF RX(DUT):DONE!");
		//UpdateLogInfo(-1,"\r\n");
	}
	if (TestFlag==THREAD_GPS_RX)
	{
		if (DevRunObj!=NULL)
		{
			DevRunObj->DestroyProcess();
		}
		((CButton *)GetDlgItem(IDC_BUTTON7))->EnableWindow(true);
		UpdateLogInfo(-1,"ACERSTARTGPSTEST START DONE!");
		UpdateLogInfo(-1,"\r\n");
	}
	if (TestFlag2 == THREAD_GPS_INSTALL)
	{
		SetEvent(hGPSInstall);
	}
	if (TestFlag2==THREAD_GPS_START || TestFlag2==THREAD_GPS_STOP)
	{
		SetEvent(hGPSAPKAction);
	}

	if (DevRunObj!=NULL)
	{
		DevRunObj->DestroyProcess();
	}
	//end of thread
	DevRunObj->Close();
	delete DevRunObj;
	DevRunObj = NULL;
	//RunUIChange(false);
	CloseHandle(hNoInfoHandle);
	return 0;
}

DWORD CADBFormDlg::IperfDutTx(void)
{
	//TestFlag = THREAD_IPERF_DUT_TX;
	TestFlag2 = THREAD_IPERF_DUT_TX;
	char IperfTxDUTCMD[MAX_PATH] = {0};
	CString IPReplace = CMD_ADB_SHELL_TX;
	IPReplace.Replace("PCIP",cPCIPaddr);
	//
	IPReplace.AppendFormat("%d -i %d",iTransmitTime,iIntervalTime);
	//	
	sprintf_s(IperfTxDUTCMD,"%s%s",PWD,IPReplace/*CMD_ADB_SHELL_TX*/);
	UpdateLogInfo(-1,"IPERF TX(DUT):\r\n");
	UpdateLogInfo(-1,IperfTxDUTCMD);
	UpdateLogInfo(-1,"\r\n");
	//debug
	//AfxMessageBox(IperfTxDUTCMD);
	//
	DevRunObj = new CPipeRun(IperfTxDUTCMD,true);
	DevRunObj->PreparePipe();
	DevRunObj->RunProgress();

	hNoInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)NoLogInfoDo,this,NULL,&dNoInfoHandleID);

	return 0;
}
DWORD CADBFormDlg::IperfDutRx(void)
{
	//TestFlag = THREAD_IPERF_DUT_RX;
	TestFlag2 = THREAD_IPERF_DUT_RX;
	char IperfRxDUTCMD[MAX_PATH] = {0};
	sprintf_s(IperfRxDUTCMD,"%s%s",PWD,CMD_ADB_SHELL_RX);
	UpdateLogInfo(-1,"IPERF RX(DUT):\r\n");
	UpdateLogInfo(-1,IperfRxDUTCMD);
	UpdateLogInfo(-1,"\r\n");
	//debug
	//AfxMessageBox(IperfRxDUTCMD);
	//
	DevRunObj = new CPipeRun(IperfRxDUTCMD,true);
	DevRunObj->PreparePipe();
	DevRunObj->RunProgress();

	hNoInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)NoLogInfoDo,this,NULL,&dNoInfoHandleID);

	return 0;
}

BOOL CADBFormDlg::LoadDefaultConfig(void)
{
	CString m_sConfigPath = "";
	m_sConfigPath.Format("%s\\config.ini",PWD);
	if(TRUE==m_hConfig.SetPath(m_sConfigPath))
	{
		//iSIMCheck4 =  m_hConfig.GetKeyIntValue("Settings","SIMCheck4",0);
		//sMCCMNC1 = m_hConfig.GetKeyStrValue("Settings","MCCMNC1","");
		cPCIPaddr  = m_hConfig.GetKeyStrValue("Settings","PCIP","");
		cDUTIPaddr = m_hConfig.GetKeyStrValue("Settings","DeviceIP","");
		cPrj       = m_hConfig.GetKeyStrValue("Settings","Project","");
		cWoid      = m_hConfig.GetKeyStrValue("Settings","WorkOrder","");
		m_ckRSSI   = m_hConfig.GetKeyIntValue("TestItem","Enable RSSI Test",0);
		m_ckThpTx  = m_hConfig.GetKeyIntValue("TestItem","Enable Throughput Tx Test",0);
		m_ckThpRx  = m_hConfig.GetKeyIntValue("TestItem","Enable Throughput Rx Test",0);
		m_ckGPS    = m_hConfig.GetKeyIntValue("TestItem","Enable GPS Test",0);
		DBGFlag    = m_hConfig.GetKeyIntValue("TestItem","Debug Status",0);
		iIperfServerTime
			= m_hConfig.GetKeyIntValue("TestItem","IPERF Server Startup Time",0);
		//GPS test timeout
		iGpsTimeout= m_hConfig.GetKeyIntValue("TestItem","GPS Test Timeout",10000);
		//adb connect timeout
		iConnectTimeout
			= m_hConfig.GetKeyIntValue("TestItem","Connect Timeout",5000);
		iRSSITimeout
			= m_hConfig.GetKeyIntValue("TestItem","RSSI Test Timeout",3000);
		iTHPTxTimeout
			= m_hConfig.GetKeyIntValue("TestItem","Throughput TX Test Timeout",30);
		iTHPRxTimeout
			= m_hConfig.GetKeyIntValue("TestItem","Throughput RX Test Timeout",30);
		iTHPTimeBuff
			= m_hConfig.GetKeyIntValue("TestItem","Throughput Test Timeout Buffer",5);
		//spec
		iGPSCNSpec = m_hConfig.GetKeyIntValue("Spec","GPS CN SPEC",35);
		iGPSSatCnt = m_hConfig.GetKeyIntValue("Spec","GPS SATELLITE COUNT",3);
		iRSSISpec = m_hConfig.GetKeyIntValue("Spec","WLAN RSSI SPEC",-60);
		iThpTxSpec = m_hConfig.GetKeyIntValue("Spec","Throughput TX SPEC",8);
		iThpRxSpec = m_hConfig.GetKeyIntValue("Spec","Throughput RX SPEC",8);
		//throughput test config
		iTransmitTime
			= m_hConfig.GetKeyIntValue("TestItem","IPERF Test Time",30);
		iIntervalTime
			= m_hConfig.GetKeyIntValue("TestItem","IPERF Report Interval Time",1);
		// SN command type
		iGetSNType = m_hConfig.GetKeyIntValue("TestItem","Fetch Serial Number",0);
		// DUT ip get automatically
		iDutIpAuto = m_hConfig.GetKeyIntValue("TestItem","Fetch DUT IP Automatically",1);
		// retry times
		iRetryTimes= m_hConfig.GetKeyIntValue("TestItem","Retry Times",3);
	}
	else
	{
		cPCIPaddr  = "";
		cDUTIPaddr = "";
		cPrj       = "";
		cWoid      = "";
	}

	UpdateData(FALSE);

	return TRUE;
}
BOOL CADBFormDlg::SaveConfig(void)
{
	//m_hConfig.SetKeyStrValue("Settings","Software",sSWV);
	//m_hConfig.SetKeyNumValue("Settings","SIMCheck1",iSIMCheck1);
	UpdateData(TRUE);

	m_hConfig.SetKeyStrValue("Settings","PCIP",cPCIPaddr);
	m_hConfig.SetKeyStrValue("Settings","DeviceIP",cDUTIPaddr);
	m_hConfig.SetKeyStrValue("Settings","Project",cPrj);
	m_hConfig.SetKeyStrValue("Settings","WorkOrder",cWoid);
	m_hConfig.SetKeyNumValue("TestItem","Enable RSSI Test",m_ckRSSI);
	m_hConfig.SetKeyNumValue("TestItem","Enable Throughput Tx Test",m_ckThpTx);
	m_hConfig.SetKeyNumValue("TestItem","Enable Throughput Rx Test",m_ckThpRx);
	m_hConfig.SetKeyNumValue("TestItem","Enable GPS Test",m_ckGPS);
	m_hConfig.SetKeyNumValue("TestItem","Debug Status",DBGFlag);
	//iperf server startup time
	m_hConfig.SetKeyNumValue("TestItem","IPERF Server Startup Time",iIperfServerTime);
	//GPS test timeout
	m_hConfig.SetKeyNumValue("TestItem","GPS Test Timeout",iGpsTimeout);
	//ADB connect timeout
	m_hConfig.SetKeyNumValue("TestItem","Connect Timeout",iConnectTimeout);
	//RSSI test time out
	m_hConfig.SetKeyNumValue("TestItem","RSSI Test Timeout",iRSSITimeout);
	//DUT upload test timeout
	m_hConfig.SetKeyNumValue("TestItem","Throughput TX Test Timeout",iTHPTxTimeout);
	//DUT download test timeout
	m_hConfig.SetKeyNumValue("TestItem","Throughput RX Test Timeout",iTHPRxTimeout);
	//DUT iperf test timeout buffer
	m_hConfig.SetKeyNumValue("TestItem","Throughput Test Timeout Buffer",iTHPTimeBuff);
	//SPEC
	//GPS test satellite cn spec
	m_hConfig.SetKeyNumValue("Spec","GPS CN SPEC",iGPSCNSpec);
	//
	m_hConfig.SetKeyNumValue("Spec","GPS SATELLITE COUNT",iGPSSatCnt);
	//Wifi test rssi spec
	m_hConfig.SetKeyNumValue("Spec","WLAN RSSI SPEC",iRSSISpec);
	//throughput tx spec
	m_hConfig.SetKeyNumValue("Spec","Throughput TX SPEC",iThpTxSpec);
	//throughput rx spec
	m_hConfig.SetKeyNumValue("Spec","Throughput RX SPEC",iThpRxSpec);
	//throughput test config
	m_hConfig.SetKeyNumValue("TestItem","IPERF Test Time",iTransmitTime);
	m_hConfig.SetKeyNumValue("TestItem","IPERF Report Interval Time",iIntervalTime);
	// SN command type
	m_hConfig.SetKeyNumValue("TestItem","Fetch Serial Number",iGetSNType);
	// DUT ip get automatically
	m_hConfig.SetKeyNumValue("TestItem","Fetch DUT IP Automatically",iDutIpAuto);
	// retry times
	m_hConfig.SetKeyNumValue("TestItem","Retry Times",iRetryTimes);

	return TRUE;
}

void CADBFormDlg::SetWindowSize(E_WND_SIZE sType)
{
	//CString sTmp;
	CRect rect;
	int SleepTime = 8;
	GetWindowRect(&rect);
	switch(sType)
	{
	case E_WND_NORMAL:
		if (rect.Height()==FROM_NOR_HEIGHT)
		{
			return;
		}
		for(int i=FROM_DBG_HEIGHT;i>=FROM_NOR_HEIGHT;i-=5)
		{
			MoveWindow(rect.left,rect.top,FORM_WIDTH,i);
			Sleep(SleepTime);
		}
		MoveWindow(rect.left,rect.top,FORM_WIDTH,FROM_NOR_HEIGHT);
		break;
	case E_WND_DEBUG:
		//GetWindowRect(&rect);
		if (rect.Height()==FROM_DBG_HEIGHT)
		{
			return;
		}
		for(int i=FROM_NOR_HEIGHT;i<=FROM_DBG_HEIGHT;i+=5)
		{
			MoveWindow(rect.left,rect.top,FORM_WIDTH,i);
			Sleep(SleepTime);
		}
		MoveWindow(rect.left,rect.top,FORM_WIDTH,FROM_DBG_HEIGHT);
		break;
	default:
		break;
	}
}

void CADBFormDlg::SetProgressPos(int CurrPos,int MaxPos)
{
	if (MAX_PROGRESS_LEN != MaxPos)
	{
		ProcessAll.SetRange(1,MaxPos);
	}
	ProcessAll.SetPos(CurrPos);
};

void CADBFormDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SaveConfig();
	CDialog::OnClose();
}

void CADBFormDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox("Click More!");
	UpdateData(TRUE);

	// diabled button
	RunUIChange(true);
	// running status message show
	UpdateTestStatus(STATUS_RUNNING);
	// Connect device

	UpdateLogInfo(-1,"["+GetCurTimeStr()+"] START\r\n");
	hMainThreadHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)MainThread,this,NULL,&dMainThreadHandleID);
	//Sleep(2000);
	//AfxMessageBox("Click More!");
}

// GPS Test
void CADBFormDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	GPSRx();
}

DWORD CADBFormDlg::GPSRx(void)
{
	TestFlag = THREAD_GPS_RX;

	char GpsRxCMD[MAX_PATH] = {0};
	sprintf_s(GpsRxCMD,"%s%s",PWD,CMD_ADB_SHELL_GPS);
	UpdateLogInfo(-1,"GPS TEST:\r\n");
	UpdateLogInfo(-1,GpsRxCMD);
	UpdateLogInfo(-1,"\r\n");
	//debug
	//AfxMessageBox(DetectCMD);
	//
	DevRunObj = new CPipeRun(GpsRxCMD,true);
	DevRunObj->PreparePipe();
	DevRunObj->RunProgress();

	hNoInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)NoLogInfoDo,this,NULL,&dNoInfoHandleID);

	Sleep(200);
	char GpsRxFetchCMD[MAX_PATH] = {0};
	sprintf_s(GpsRxFetchCMD,"%s%s",PWD,CMD_ADB_GPS_FETCH);
	UpdateLogInfo(-1,"GPS LOGCAT:\r\n");
	UpdateLogInfo(-1,GpsRxFetchCMD);
	UpdateLogInfo(-1,"\r\n");

	RunObj = new CPipeRun(GpsRxFetchCMD,true);
	RunObj->PreparePipe();
	RunObj->RunProgress();

	hGetInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)LogInfoShow,this,NULL,&dGetInfoHandleID);

	return 0;
}

//main action thread 
DWORD CADBFormDlg::MainThread(LPVOID lpParam)
{
	CADBFormDlg *pThis = (CADBFormDlg *)lpParam;

	return pThis->MainThreadDo(0, 0);
}

DWORD CADBFormDlg::MainThreadDo(WPARAM wPamam, LPARAM lParam)
{
	//Sleep(1000);
	// detect device
	int LoopCnt = 0;

LOOP_DETECT:
	LoopCnt++;
	hDetectDevice = CreateEvent(NULL, FALSE, FALSE, NULL);
	UpdateLogInfo(-1,"["+GetCurTimeStr()+"] ");
	DetectDevices();
	if(WAIT_TIMEOUT==WaitForSingleObject(hDetectDevice,iConnectTimeout))
	{
		if (RunObj!=NULL)
		{
			RunObj->DestroyProcess();
		}
	}
	if (dStartRet!=E_ADB_SUCCESS)
	{
		if (LoopCnt<iRetryTimes)
		{
			Sleep(500);
			goto LOOP_DETECT;
		}
		// running status message show
		UpdateTestStatus(STATUS_FAIL,dStartRet);
		// enabled button
		RunUIChange(false);
		// test end
		UpdateLogInfo(5,"\r\n["+GetCurTimeStr()+"] End\r\n");
		return dStartRet;
	}
	// GPS test
	if (m_ckGPS)
	{
		// GPS action handle
		hGPSTest = CreateEvent(NULL, FALSE, FALSE, NULL);
		UpdateLogInfo(-1,"["+GetCurTimeStr()+"] ");
		//20140226 Wujian add Start
		hGPSAPKAction = CreateEvent(NULL, FALSE, FALSE, NULL);
		HQGPSAction(1);
		if(WAIT_TIMEOUT==WaitForSingleObject(hGPSAPKAction,iGpsTimeout))
		{
			if (DevRunObj!=NULL)
			{
				DevRunObj->DestroyProcess();
			}
		}
		//20140226 Wujian add End
		GPSRx();
		if(WAIT_TIMEOUT==WaitForSingleObject(hGPSTest,iGpsTimeout))
		{
			if (RunObj!=NULL)
			{
				RunObj->DestroyProcess();
			}
		}
		//这是为了线程不同步而改，坑
		Sleep(200);
		//20140226 Wujian add Start
		//stop
		//HQGPSAction();
		//20140226 Wujian add End
		if (dStartRet!=E_ADB_SUCCESS)
		{
			// running status message show
			UpdateTestStatus(STATUS_FAIL,dStartRet);
			// enabled button
			RunUIChange(false);
			// test end
			UpdateLogInfo(5,"\r\n["+GetCurTimeStr()+"] End\r\n");
			return dStartRet;
		}
	}
	// RSSI test
	if (m_ckRSSI)
	{
		UpdateLogInfo(-1,"["+GetCurTimeStr()+"] ");
		WLANInfo();
		// RSSI action handle
		hRSSITest = CreateEvent(NULL, FALSE, FALSE, NULL);
		if(WAIT_TIMEOUT==WaitForSingleObject(hRSSITest,iRSSITimeout))
		{
			if (RunObj!=NULL)
			{
				RunObj->DestroyProcess();
			}
		}
		//Sleep(200);
		if (dStartRet!=E_ADB_SUCCESS)
		{
			// running status message show
			UpdateTestStatus(STATUS_FAIL,dStartRet);
			// enabled button
			RunUIChange(false);
			// test end
			UpdateLogInfo(5,"\r\n["+GetCurTimeStr()+"] End\r\n");
			return dStartRet;
		}
	}
	// Throughput Tx(Upload) test
	if (m_ckThpTx)
	{
		LoopCnt = 0;
LOOP_IPERF_TX:
		LoopCnt++;
		UpdateLogInfo(-1,"["+GetCurTimeStr()+"] ");
		IperfTx();
		Sleep(1);
		IperfDutTx();
		// Throughput Tx action handle
		hTHPTxTest = CreateEvent(NULL, FALSE, FALSE, NULL);
		if(WAIT_TIMEOUT==WaitForSingleObject(hTHPTxTest,(iTHPTxTimeout+iTHPTimeBuff)*1000))
		{
			if (RunObj!=NULL)
			{
				RunObj->DestroyProcess();
				//delete RunObj;
				//RunObj = NULL;
			}
			if (DevRunObj!=NULL)
			{
				DevRunObj->DestroyProcess();
				//delete DevRunObj;
				//DevRunObj = NULL;
			}
		}
		if (RunObj!=NULL)
		{
			RunObj->DestroyProcess();
			//delete RunObj;
			//RunObj = NULL;
		}
		if (DevRunObj!=NULL)
		{
			DevRunObj->DestroyProcess();
			//delete DevRunObj;
			//DevRunObj = NULL;
		}
		Sleep(200);
		if (dStartRet!=E_ADB_SUCCESS)
		{
			if (LoopCnt<iRetryTimes)
			{
				Sleep(500);
				goto LOOP_IPERF_TX;
			}
			// running status message show
			UpdateTestStatus(STATUS_FAIL,dStartRet);
			// enabled button
			RunUIChange(false);
			// test end
			UpdateLogInfo(5,"\r\n["+GetCurTimeStr()+"] End\r\n");
			return dStartRet;
		}
	}
	// Throughput Rx(Download) test
	if (m_ckThpRx)
	{
		LoopCnt = 0;
LOOP_IPERF_RX:
		LoopCnt++;
		UpdateLogInfo(-1,"["+GetCurTimeStr()+"] ");
		IperfDutRx();
		Sleep(1);
		IperfRx();
		// Throughput Rx action handle
		hTHPRxTest = CreateEvent(NULL, FALSE, FALSE, NULL);
		if(WAIT_TIMEOUT==WaitForSingleObject(hTHPRxTest,(iTHPRxTimeout+iTHPTimeBuff)*1000))
		{
			if (RunObj!=NULL)
			{
				RunObj->DestroyProcess();
			}
			if (DevRunObj!=NULL)
			{
				DevRunObj->DestroyProcess();
			}
		}
		if (RunObj!=NULL)
		{
			RunObj->DestroyProcess();
			//delete RunObj;
			//RunObj = NULL;
		}
		if (DevRunObj!=NULL)
		{
			DevRunObj->DestroyProcess();
			//delete DevRunObj;
			//DevRunObj = NULL;
		}
		Sleep(200);
		if (dStartRet!=E_ADB_SUCCESS)
		{
			if (LoopCnt<iRetryTimes)
			{
				Sleep(500);
				goto LOOP_IPERF_RX;
			}
			// running status message show
			UpdateTestStatus(STATUS_FAIL,dStartRet);
			// enabled button
			RunUIChange(false);
			// test end
			UpdateLogInfo(5,"\r\n["+GetCurTimeStr()+"] End\r\n");
			return dStartRet;
		}
	}

	// running status message show
	// all test item is ok
	UpdateTestStatus(STATUS_PASS);
	// enabled button
	RunUIChange(false);
	// test end
	UpdateLogInfo(5,"\r\n["+GetCurTimeStr()+"] End\r\n");
	return 0;
}
//20140226 Wujian add Start
DWORD CADBFormDlg::HQGPSAction(int actiontype)
{
	char GpsActionCMD[MAX_PATH] = {0};
	if (actiontype==0) //STOP
	{
		TestFlag2 = THREAD_GPS_STOP;
		sprintf_s(GpsActionCMD,"%s%s",PWD,CMD_ADB_SHELL_GPSED);
		UpdateLogInfo(-1,"GPS APK STOP:\r\n");
		UpdateLogInfo(-1,GpsActionCMD);
		UpdateLogInfo(-1,"\r\n");

		DevRunObj = new CPipeRun(GpsActionCMD,true);
		DevRunObj->PreparePipe();
		DevRunObj->RunProgress();

		hNoInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)NoLogInfoDo,this,NULL,&dNoInfoHandleID);

		Sleep(10);
	}
	else if (actiontype==1) //START
	{
		//APK INSTALL
#if 1
		hGPSInstall = CreateEvent(NULL,FALSE,FALSE,NULL);

		TestFlag2 = THREAD_GPS_INSTALL;
		sprintf_s(GpsActionCMD,"%s%s",PWD,CMD_ADB_SHELL_INSTALL);
		UpdateLogInfo(-1,"GPS APK INSTALL:\r\n");
		UpdateLogInfo(-1,GpsActionCMD);
		UpdateLogInfo(-1,"\r\n");

		DevRunObj = new CPipeRun(GpsActionCMD,true);
		DevRunObj->PreparePipe();
		DevRunObj->RunProgress();

		hNoInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)NoLogInfoDo,this,NULL,&dNoInfoHandleID);

		Sleep(10);
		if (WAIT_OBJECT_0 != WaitForSingleObject(hGPSInstall,iGpsTimeout))
		{
			if (DevRunObj!=NULL)
			{
				DevRunObj->DestroyProcess();
			}
			return 2;
		}
#endif		
		//APK START
		TestFlag2 = THREAD_GPS_START;
		sprintf_s(GpsActionCMD,"%s%s",PWD,CMD_ADB_SHELL_GPSST);
		UpdateLogInfo(-1,"GPS APK START:\r\n");
		UpdateLogInfo(-1,GpsActionCMD);
		UpdateLogInfo(-1,"\r\n");

		DevRunObj = new CPipeRun(GpsActionCMD,true);
		DevRunObj->PreparePipe();
		DevRunObj->RunProgress();

		hNoInfoHandle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)NoLogInfoDo,this,NULL,&dNoInfoHandleID);

		Sleep(10);
	}

	return E_ADB_SUCCESS;
};
//20140226 Wujian add End
CString CADBFormDlg::GetCurTimeStr(int iType)
{
	//获得当天时间字符串
	if (0==iType)
	{
		CTime Td = CTime::GetCurrentTime();
		return Td.Format("%Y%m%d");
	}
	else if (1==iType)
	{
		CTime Td = CTime::GetCurrentTime();
		return Td.Format("%Y-%m-%d %H:%M:%S");
	}
	return "";
}
void CADBFormDlg::LogFileUpdate(CString loginfo,int iFlag)
{
	if(!bLogFileNormal)
	{
		bLogFileNormal = hLogFile.Open(m_sLogFilePathFull,CFile::modeWrite);
		if (!bLogFileNormal)
		{
			return;
		}
		else
		{
			hLogFile.SeekToEnd();
		}
	}
	if (1==iFlag)
	{
		// before rename action
		//hLogFile.Close();
		//bLogFileNormal = FALSE;
		//
		//CString OldFileName = m_sLogFilePathFull;
		//hLogFile.SetFilePath();
		//OldFileName.Replace("_temp_",loginfo);
		//m_sLogFilePathFull.Replace("_temp_",loginfo);
		//this function will crash!!!
		//hLogFile.Rename("_temp_.log",loginfo+".log");
		//int iRet = ::rename(OldFileName,m_sLogFilePathFull);
		//DWORD dRet = GetLastError();
		FileRen(&hLogFile,loginfo);
		return;
	}
	hLogFile.Write(loginfo,loginfo.GetLength());
}


BOOL CADBFormDlg::FileRen(CFile *oriFile,CString newName)
{
	if (oriFile==NULL)
	{
		return FALSE;
	}
	if (m_sLogFilePathFull.Find(newName)!=-1)
	{
		return TRUE;
	}
	CString OriFileName = "";
	OriFileName.Format("%s",oriFile->GetFilePath());
	m_sLogFilePathFull.Replace("_temp_",newName);
	try
	{
		// read all context
		UINT bufflen = oriFile->SeekToEnd();
		char *buffstr = new char[bufflen+1];
		oriFile->SeekToBegin();
		oriFile->Read(buffstr,bufflen);
		// before rename action
		oriFile->Close();
		bLogFileNormal = FALSE;

		// delete old file 
		CFile::Remove(OriFileName);
		// create new file
		//CFile Nfile;
		bLogFileNormal = oriFile->Open(m_sLogFilePathFull,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite);
		if(!bLogFileNormal)
		{
			AfxMessageBox("RENAME create log file failure!");
		}
		else
		{
			bLogFileNormal = TRUE;
			oriFile->SeekToEnd();
			oriFile->Write(buffstr,bufflen);
			oriFile->SeekToEnd();
			//Nfile.Close();
		}
		delete buffstr;
		//CFile::Rename( OriFileName.GetBuffer(OriFileName.GetLength()), newName.GetBuffer(newName.GetLength()));
	}
	catch(CFileException* pEx )
	{
		pEx->Delete();
		return FALSE;
	}

	return TRUE;
}













BOOL CADBFormDlg::WriteLiteLog123(void)
{
	int a=1;
	int b=2;
	AfxMessageBox("Tezt");

	
	if (m_LiteTestSN.GetLength()<10)
	{
	return FALSE;
	}
	m_LogLiteStr.Format("%s,%s,",GetCurTimeStr(1),m_LiteTestSN);
	// RSSI
	if (m_ckRSSI)
	{
	if (0 == i_LiteTestRSSI)
	{
	m_LogLiteStr.AppendFormat("%s,","NoN");
	}
	else
	{
	m_LogLiteStr.AppendFormat("%d,",i_LiteTestRSSI);
	}
	}
	else
	{
	m_LogLiteStr.AppendFormat("%s,","NoT");
	}

	// GPS
	if (m_ckGPS)
	{
	if (0 == i_LiteTestGPSCount)
	{
	m_LogLiteStr.AppendFormat("%s,","NoN");
	}
	else
	{
	m_LogLiteStr.AppendFormat("%d,%.1f,",i_LiteTestGPSCount,i_LiteTestGPSCN);
	}
	}
	else
	{
	m_LogLiteStr.AppendFormat("%s,","NoT");
	}

	// ThroughPut Tx
	if (m_ckThpTx)
	{
	if (0 == i_LiteTestThpT)
	{
	m_LogLiteStr.AppendFormat("%s,","NoN");
	}
	else
	{
	m_LogLiteStr.AppendFormat("%d,",i_LiteTestThpT);
	}
	}
	else
	{
	m_LogLiteStr.AppendFormat("%s,","NoT");
	}

	// ThroughPut Rx
	if (m_ckThpRx)
	{
	if (0 == i_LiteTestThpR)
	{
	m_LogLiteStr.AppendFormat("%s","NoN");
	}
	else
	{
	m_LogLiteStr.AppendFormat("%d",i_LiteTestThpR);
	}
	}
	else
	{
	m_LogLiteStr.AppendFormat("%s","NoT");
	}

	m_LogLiteStr.AppendFormat("%s","\r\n");
	hLogFileLite.Write(m_LogLiteStr,m_LogLiteStr.GetLength());

	return TRUE;
}

void CADBFormDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

