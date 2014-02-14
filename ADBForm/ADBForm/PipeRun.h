#pragma once


typedef enum
{
	E_SUCCESS = 0,
	E_CREATE_PIPE_FAIL,
	E_CREATE_PROC_FAIL,
    E_READ_INFO_CONTINUE,
	E_READ_INFO_ERR,
	E_DESTROY_PRO_FAIL




}E_PIPERUN_RESULT;

class CPipeRun
{
public:
	CPipeRun(char *ExcuteCommand,bool ReOrNo);
	~CPipeRun(void);
private:
	char    cCommandline[MAX_PATH];    // 执行命令
	bool    bReturnFlag;               // 是否读取管道返回
	HANDLE  hPipeRead;                 // 管道读句柄
    HANDLE  hPipeWrite;                // 管道写句柄
	CString strOutput;                 // 管道读取所有消息字符串
	bool    bReadFlag;
	PROCESS_INFORMATION pi;            // 进程PI
public:
	// 创建管道
	DWORD   PreparePipe(void);
	// 创建进程
	DWORD   RunProgress(void);
	// 读取管道消息（阻塞方式）
	DWORD   UpdateOutStr(char oStr[]);
	// 销毁进程
	DWORD   DestroyProcess(void);
	// 关闭进程句柄
	DWORD   Close(void);
    // 获取允许消息（管道消息字符串）
	CString GetRunInfo(void);
};
