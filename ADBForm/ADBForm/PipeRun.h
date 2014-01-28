#pragma once


typedef enum
{
	E_SUCCESS = 0,
	E_CREATE_PIPE_FAIL,
	E_CREATE_PROC_FAIL,
    E_READ_INFO_CONTINUE,




}E_PIPERUN_RESULT;

class CPipeRun
{
public:
	CPipeRun(char *ExcuteCommand,bool ReOrNo);
	~CPipeRun(void);
private:
	char    cCommandline[MAX_PATH];
	bool    bReturnFlag;
	HANDLE  hPipeRead;
    HANDLE  hPipeWrite;
	CString strOutput;
	PROCESS_INFORMATION pi;
public:
	DWORD   PreparePipe(void);
	DWORD   RunProgress(void);
	DWORD   UpdateOutStr(char oStr[4096]);
	DWORD   Close(void);
	CString GetRunInfo(void);
};
