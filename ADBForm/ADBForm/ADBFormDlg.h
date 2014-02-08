// ADBFormDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "PipeRun.h"

#define TEST_COMMAND       ("F:\\BenKyoU\\CloudWu\\CCPP\\CCpp\\Debug\\CCpp.exe")
#define CMD_ADB_DEVICES    ("\\adb\\adb devices")
#define CMD_ADB_SHELL      ("\\adb\\adb shell")
#define CMD_ADB_SHELL_WLAN ("\\adb\\adb shell dumpsys wifi")
//throughput test
//Tx test
#define CMD_PC_IPERF_TX    ("\\bin\\iperf.exe -s -P 0 -i 1 -p 5001 -C -f k")
#define CMD_ADB_SHELL_TX   ("\\adb\\adb shell iperf -c 192.168.1.101 -t 30 -i 1")
//Rx test
#define CMD_ADB_SHELL_RX   ("\\adb\\adb shell iperf -s")
#define CMD_PC_IPERF_RX    ("\\bin\\iperf.exe -c 192.168.1.102 -P 1 -i 1 -p 5001 -C -f k -t 30 -T 1")


//key word
#define KEY_ADB_DEVICES       "List of devices attached"
#define KEY_ADB_DEVICES_DUT   "device"
#define KEY_RSSI_RESULT       "Flags             SSID"
#define KEY_ENTER             "\r\n"


typedef enum
{
	THREAD_DETECT = 1,
	THREAD_WIFI_RSSI,
	THREAD_IPERF_TX,
	THREAD_IPERF_RX,
	
	
	UNKOWN_TYPE = 255
}E_THREAD_TYPE;


//ERRCODE
typedef enum
{
	E_ADB_SUCCESS = 0,
	E_ADB_ERROR,
	E_ADB_DETECT_ERR,
	E_ADB_NODUT,
	E_ADB_MULTIDEVICES,
	E_ADB_RSSI_ERR,
	E_ADB_RSSI_NULL




}E_ADBFORM_RETURN;
// CADBFormDlg 对话框
class CADBFormDlg : public CDialog
{
// 构造
public:
	CADBFormDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ADBFORM_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	static DWORD LogInfoShow(LPVOID lpParam);
	DWORD        LogInfoShowFunc(WPARAM wPamam, LPARAM lParam);
public:
	CEdit     Cot_msginfoshow;
	CPipeRun  *RunObj;
	HANDLE    hGetInfoHandle;
	DWORD     dGetInfoHandleID;
	void      RunUIChange(bool RunFlag);
	char      PWD[MAX_PATH];
	DWORD     DetectDevices(void);
	void      UpdateLogInfo(DWORD retcode,CString loginfo);
	DWORD     WLANInfo(void);
	CString   UnDoStrBuff;
	E_THREAD_TYPE     TestFlag;
	DWORD     AnalyseStr(double dSpec,E_THREAD_TYPE type);
	DWORD     AnalyseDevices(void);
	DWORD     AnalyseRSSI(void);
	DWORD     SearchRSSIArrary(int RSSIArr[],CString mStr);	
	int       MaxRSSI;
	DWORD     IperfTx(void);
	DWORD     IperfRx(void);
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton4();
};
