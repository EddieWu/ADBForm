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
	char    cCommandline[MAX_PATH];    // ִ������
	bool    bReturnFlag;               // �Ƿ��ȡ�ܵ�����
	HANDLE  hPipeRead;                 // �ܵ������
    HANDLE  hPipeWrite;                // �ܵ�д���
	CString strOutput;                 // �ܵ���ȡ������Ϣ�ַ���
	bool    bReadFlag;
	PROCESS_INFORMATION pi;            // ����PI
public:
	// �����ܵ�
	DWORD   PreparePipe(void);
	// ��������
	DWORD   RunProgress(void);
	// ��ȡ�ܵ���Ϣ��������ʽ��
	DWORD   UpdateOutStr(char oStr[]);
	// ���ٽ���
	DWORD   DestroyProcess(void);
	// �رս��̾��
	DWORD   Close(void);
    // ��ȡ������Ϣ���ܵ���Ϣ�ַ�����
	CString GetRunInfo(void);
};
