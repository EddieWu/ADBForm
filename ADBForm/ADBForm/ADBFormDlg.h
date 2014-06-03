// ADBFormDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "PipeRun.h"
#include "IniFile.h"
#include "ColorStatic.h"
#include "BtnST.h"
#include "afxcmn.h"
#include "shlwapi.h"

//#define _DEBUG__
#define TEST_COMMAND       ("F:\\BenKyoU\\CloudWu\\CCPP\\CCpp\\Debug\\CCpp.exe")
#define CMD_ADB_DEVICES    ("\\adb\\adb devices")
//#define CMD_ADB_GETSN      ("\\adb\\adb shell getprop gsm.serial")
#define CMD_ADB_GETSN      ("\\adb\\adb shell getprop ro.ril.sn.id")
#define CMD_ADB_SHELL      ("\\adb\\adb shell")
#define CMD_ADB_SHELL_WLAN ("\\adb\\adb shell dumpsys wifi")
#define CMD_ADB_SHELL_GPS  ("\\adb\\adb shell am broadcast -a ACERSTARTGPSTEST")
//20140226 Wujian add Start
// 打开手机端应用
// adb shell am start -n com.huaqin.acergpsandwifitest/.MainActivity
// 关闭手机端应用
// adb shell am force-stop com.huaqin.acergpsandwifitest
#define CMD_ADB_SHELL_INSTALL  ("\\adb\\adb install -r AcerGpsAndWifiTest.apk")
#define CMD_ADB_SHELL_GPSST    ("\\adb\\adb shell am start -n com.huaqin.acergpsandwifitest/.MainActivity")
#define CMD_ADB_SHELL_GPSED    ("\\adb\\adb shell am force-stop com.huaqin.acergpsandwifitest")
//20140226 Wujian add End
#define CMD_ADB_GPS_FETCH      ("\\adb\\adb logcat -b main -v time -s ACERGPS")
//#define CMD_ADB_GPS_FETCH  ("\\adb\\adb logcat -b main -v time")
#define CMD_ADB_SHELL_UNINSTALL  ("\\adb\\adb uninstall AcerGpsAndWifiTest.apk")

//throughput test
//Tx test
#define CMD_PC_IPERF_TX    ("\\bin\\iperf.exe -s -P 0 -i 1 -p 5001 -C -f k")
//#define CMD_ADB_SHELL_TX   ("\\adb\\adb shell iperf -c PCIP -t 30 -i 1")
#define CMD_ADB_SHELL_TX   ("\\adb\\adb shell iperf -c PCIP -t ")
//Rx test
#define CMD_ADB_SHELL_RX   ("\\adb\\adb shell iperf -s")
//#define CMD_PC_IPERF_RX    ("\\bin\\iperf.exe -c DUTIP -P 1 -i 1 -p 5001 -C -f k -t 30 -T 1")
#define CMD_PC_IPERF_RX    ("\\bin\\iperf.exe -c DUTIP -P 1 -i 1 -p 5001 -C -f k -t ")

//key word
#define KEY_ADB_DEVICES       "List of devices attached"
#define KEY_ADB_DEVICES_DUT   "device"
#define KEY_ADB_D_NOTFOUND    "error: device not found"
#define KEY_RSSI_RESULT       "Flags             SSID"
#define KEY_WLAN_RUNSTATE     "ConnectedState"//"runState=Running"
#define KEY_WLAN_IPADDR       "LinkAddresses:"//"ipaddr"
#define KEY_WLAN_GATEWAY      "Routes:"//"gateway"
#define KEY_ENTER             "\r\n"
#define KEY_IPADDR            "ipaddr"
#define KEY_GATEWAY           "gateway"
//GPS key word
#define KEY_GPS_SATECNT       "satelliteCount"
//IPERF Key Word
#define KEY_IPERF_RESULT      "0.0-30.0 sec"  //some times 0.0-30.2 sec
#define KEY_IPERF_KBYTES      "KBytes"
#define KEY_IPERF_KBITSPER    "Kbits/sec"


//color
#define COLOR_READY     RGB(27, 136, 228)
#define COLOR_WHITE     RGB(255, 255, 255)
#define COLOR_RUN       RGB(240, 160, 16)
#define COLOR_PASS      RGB(16, 124, 32)
#define COLOR_FAIL      RGB(223, 32, 16)
#define COLOR_UNCONFIG  RGB(160, 160, 160)

//
#define MAX_PROGRESS_LEN 100

typedef enum
{
	THREAD_DETECT = 1,
	THREAD_WIFI_RSSI,
	THREAD_IPERF_TX,
	THREAD_IPERF_RX,
	THREAD_IPERF_DUT_TX,
	THREAD_IPERF_DUT_RX,
	THREAD_GPS_RX,
	THREAD_GPS_INSTALL,
	THREAD_GPS_START,
	THREAD_GPS_STOP,
	THREAD_ADB_SERVER,
	
	UNKOWN_TYPE = 255
}E_THREAD_TYPE;

typedef enum
{
	STATUS_READY = 0,
	STATUS_RUNNING,
	STATUS_PASS,
	STATUS_FAIL,

	STATUS_UNKOWN = 255
}E_TEST_STATUS;

//ERRCODE
typedef enum
{
	E_ADB_SUCCESS = 0,
	E_ADB_ERROR,
	E_ADB_DETECT_ERR,
	E_ADB_NODUT,
	E_ADB_MULTIDEVICES,
	E_ADB_BADSN,
	E_ADB_RSSI_STOP,
	E_ADB_RSSI_BADIPADDR,
	E_ADB_RSSI_ERR,
	E_ADB_RSSI_NULL,
	E_ADB_RSSI_LESS,
	E_ADB_GPS_ERR,
	E_ADB_GPS_CNLESS,
	E_ADB_GPS_SATLESS, //satellite
	E_ADB_GPS_FORMAT,
	E_ADB_THPTX_ERR,
	E_ADB_THPTX_LESS,
	E_ADB_THPRX_ERR,
	E_ADB_THPRX_LESS

}E_ADBFORM_RETURN;
//ERRCODE

//wnd size type
typedef enum
{
	E_WND_NORMAL = 0,
	E_WND_DEBUG

}E_WND_SIZE;
#define DEBUG_FLAG       5144
#define FORM_WIDTH       0x0000025e
#define FROM_NOR_HEIGHT  0x00000171
#define FROM_COF_HEIGHT  0x00000220
#define FROM_DBG_HEIGHT  0x000001a6
// CADBFormDlg 对话框
class CADBFormDlg : public CDialog
{
// 构造
public:
	CADBFormDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ADBFORM_DIALOG };

	CColorStatic	m_stTestStatus;
	CButtonST       m_StatAll;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();  //DeDUT
	afx_msg void OnBnClickedButton3();  //WiFiSys
	afx_msg void OnBnClickedButton2();  //IperfT
	afx_msg void OnBnClickedButton4();  //IperfR
	afx_msg void OnBnClickedButton5();  //DutT
	afx_msg void OnBnClickedButton6();  //DutR
	afx_msg void OnBnClickedButton7();  //GPS
	static DWORD LogInfoShow(LPVOID lpParam);
	DWORD        LogInfoShowFunc(WPARAM wPamam, LPARAM lParam);
	static DWORD NoLogInfoDo(LPVOID lpParam);
	DWORD        NoLogInfoDoFunc(WPARAM wPamam, LPARAM lParam);
	static DWORD MainThread(LPVOID lpParam);
	DWORD        MainThreadDo(WPARAM wPamam, LPARAM lParam);
	HANDLE    hMainThreadHandle;
	DWORD     dMainThreadHandleID;
public:
	//debug flag
	int       DBGFlag;
	CEdit     Cot_msginfoshow;
	CProgressCtrl ProcessAll;
	CPipeRun  *RunObj;
	CPipeRun  *DevRunObj;
	HANDLE    hGetInfoHandle;
	DWORD     dGetInfoHandleID;
	HANDLE    hPCIperfServer;
	HANDLE    hDUTIperfServer;
	// LOG file
	CString   m_sLogFilePath;
	CString   m_sLogFilePathFull;
	CFile     hLogFile;
	// 20140324 WuJian add Start
	CFile     hLogFileLite;
	CString   m_sLogFilePathLiteFull;
	CString   m_LogLiteStr;
	CString   m_LiteTestSN;
	int       i_LiteTestRSSI;
	int       i_LiteTestGPSCount;
	float     i_LiteTestGPSCN;
	int       i_LiteTestThpT;
	int       i_LiteTestThpR;
	BOOL      WriteLiteLog(void);
	BOOL      bLogFileNormalLite;
	// 20140324 WuJian add End
	BOOL      bLogFileNormal;
	BOOL      FileRen(CFile *oriFile,CString newName);
	//detect device action handle
	HANDLE    hDetectDevice;
	// GPS action handle
	HANDLE    hGPSTest;
	HANDLE    hGPSAPKAction;
	HANDLE    hGPSInstall;
	// RSSI test
	HANDLE    hRSSITest;
	// Throughput Tx(Upload) test
	HANDLE    hTHPTxTest;
	// Throughput Rx(Download) test
	HANDLE    hTHPRxTest;

	HANDLE    hNoInfoHandle;
	DWORD     dNoInfoHandleID;
	HANDLE    hNoLogThread;
	CIniFile  m_hConfig;        //config file handle
	void      RunUIChange(bool RunFlag);
	void      SetProgressPos(int CurrPos,int MaxPos=MAX_PROGRESS_LEN);
	char      PWD[MAX_PATH];
	DWORD     DetectDevices(void);
	DWORD     ActionADBServer(void);
	void      UpdateLogInfo(DWORD retcode,CString loginfo);
	DWORD     WLANInfo(void);
	CString   UnDoStrBuff;
	CString   GPSTestStr;
	BOOL      ApkOK;
	E_THREAD_TYPE     TestFlag;
	E_THREAD_TYPE     TestFlag2;
	DWORD     AnalyseStr(double dSpec,E_THREAD_TYPE type,CString KeyStr="");
	DWORD     AnalyseDevices(void);
	DWORD     AnalyseRSSI(void);
	DWORD     AnalyseGPS(void);
	BOOL      GPSStrValid(void);
	DWORD     AnalyseTHPTx(CString ResultStr);
	DWORD     AnalyseTHPRx(CString ResultStr);
	DWORD     SearchRSSIArrary(int RSSIArr[],CString mStr);	
	int       MaxRSSI;
	DWORD     IperfTx(void);
	DWORD     IperfRx(void);
	//
	DWORD     IperfDutTx(void);
	DWORD     IperfDutRx(void);
	BOOL      SaveConfig(void);
	BOOL      LoadDefaultConfig(void);
	void      UpdateTestStatus(E_TEST_STATUS status,DWORD errcode=0);
	BOOL      ConfigIsReady(void);
	void      SetWindowSize(E_WND_SIZE sType);
	DWORD     GPSRx(void);
	//20140226 Wujian add Start
    DWORD     HQGPSAction(int actiontype=0);
	//20140226 Wujian add End
	CString   GetCurTimeStr(int iType=1);
	void      LogFileUpdate(CString loginfo,int iFlag=0);
	//20140324 Wujian modify Start
	BOOL      LogPrepare(void);
	BOOL      bLogPre;
	//20140324 Wujian modify End
public:
	// PC ip address
	CString cPCIPaddr;
	// device under test IP address
	CString cDUTIPaddr;
	// project
	CString cPrj;
	// work order
	CString cWoid;
public:
	// Start all
	afx_msg void OnBnClickedButtonStart();
	DWORD     dStartRet;
	// RSSI Checkbox
	CButton   cbRSSI;
	BOOL      m_ckRSSI;
    // Throughput Tx(device upload)
	CButton   cbThpTx;
	BOOL      m_ckThpTx;
	// Throughput Rx(device download)
	CButton   cbThpRx;
	BOOL      m_ckThpRx;
	// GPS test checkbox
	CButton   cbGPS;
	BOOL      m_ckGPS;
	//timeout
	int       iConnectTimeout;
	int       iIperfServerTime;// microsecond
	int       iGpsTimeout;
	int       iRSSITimeout;
	int       iTHPTxTimeout;
	int       iTHPRxTimeout;
	int       iTHPTimeBuff;

	//SPEC
	int       iGPSCNSpec;
	int       iGPSSatCnt;
	int       iRSSISpec;
	int       iThpTxSpec;
	int       iThpRxSpec;
	//throughput test config
	int       iTransmitTime;
	int       iIntervalTime;
	// SN command type
	int       iGetSNType;
	// DUT ip get automatically
	int       iDutIpAuto;
	// retry times
	int       iRetryTimes;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CFile DebugLog;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	int       GPSBugFixCnt;
};
