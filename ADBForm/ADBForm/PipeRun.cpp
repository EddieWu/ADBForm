#include "StdAfx.h"
#include "PipeRun.h"

/************************************************************************/
/* ���캯����ExcuteCommand:������,ReOrNo:�Ƿ��ȡ�ܵ�����ֵ             */
/************************************************************************/
CPipeRun::CPipeRun(char *ExcuteCommand,bool ReOrNo)
{
	memset(cCommandline,0x0,sizeof(cCommandline));
	//ASSERT(strlen(cCommandline)<MAX_PATH);
	strncpy_s(cCommandline,ExcuteCommand,strlen(ExcuteCommand));
	bReturnFlag = ReOrNo;
	hPipeRead   = NULL;
	hPipeWrite  = NULL;
	//
	strOutput   = "";
	//
	bReadFlag   = false;
}

CPipeRun::~CPipeRun(void)
{

}

DWORD CPipeRun::PreparePipe(void)
{
	SECURITY_ATTRIBUTES sa;  
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sa.lpSecurityDescriptor = NULL;   //ʹ��ϵͳĬ�ϵİ�ȫ������ 
	sa.bInheritHandle = TRUE;         //�����Ľ��̼̳о��

	if (!CreatePipe(&hPipeRead,&hPipeWrite,&sa,0))  //���������ܵ�
	{  
		MessageBox(NULL,"CreatePipe Failed!","WARNING",MB_OK | MB_ICONWARNING);  
		return E_CREATE_PIPE_FAIL;
	}


	return E_SUCCESS;
}

DWORD   CPipeRun::RunProgress(void)
{
	STARTUPINFO si; 
	//PROCESS_INFORMATION pi;
	//BOOL bCrateProcess;

	ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO); 
	GetStartupInfo(&si); 
	si.hStdError   = hPipeWrite; 
	si.hStdOutput  = hPipeWrite;    //�´������̵ı�׼�������д�ܵ�һ��
	si.wShowWindow = SW_HIDE;       //���ش��� 
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if (!CreateProcess(NULL,cCommandline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))  //�����ӽ���
	{
		MessageBox(NULL,"CreateProcess Failed!","WARNING",MB_OK | MB_ICONWARNING);  
		return E_CREATE_PROC_FAIL;
	}
	CloseHandle(hPipeWrite);  //�رչܵ����

	return E_SUCCESS;
}


DWORD   CPipeRun::UpdateOutStr(char oStr[])
{
	//�ȴ�����ִ����� 
	//WaitForSingleObject(pi.hProcess, INFINITE);

	char buffer[80000] = {0};
	//CString strOutput;
	DWORD bytesRead;
	bReadFlag = true;

	try
	{
		if (ReadFile(hPipeRead,buffer,80000,&bytesRead,NULL) == NULL)  //��ȡ�ܵ�
		{
			bReadFlag = false;
			return E_READ_INFO_ERR;
		}
		else
		{
			strOutput += buffer;
			strncpy_s(oStr,80000,buffer,strlen(buffer));
			Sleep(1);
			return E_READ_INFO_CONTINUE;
		}
	}
	catch (CException* e)
	{
		return E_READ_INFO_ERR;	
	}
	

	/*
	while (true) 
	{
		if (ReadFile(hPipeRead,buffer,4095,&bytesRead,NULL) == NULL)  //��ȡ�ܵ�
		{
			break;
		}

		strOutput += buffer;
		SetDlgItemText(IDC_EDIT_OUTPUT,strOutput);  //��ʾ�����Ϣ���༭��,��ˢ�´���
		UpdateWindow();
		strncpy_s(oStr,buffer,strlen(buffer));
		Sleep(100);
	}
	*/
	//return E_SUCCESS;
}

DWORD   CPipeRun::Close(void)
{
	CloseHandle(hPipeRead);
	CloseHandle(pi.hProcess); 
	CloseHandle(pi.hThread);
	return E_SUCCESS;
}
CString CPipeRun::GetRunInfo(void)
{
	return 	strOutput;
}
DWORD CPipeRun::DestroyProcess(void)
{
	//KillProcess
	/*
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,pi.dwProcessId);
	if(hProcess==NULL)
	{
		return E_DESTROY_PRO_FAIL;
	}
	*/
	if(!TerminateProcess(pi.hProcess,0))
	{
		return E_DESTROY_PRO_FAIL;
	};

	return E_SUCCESS;
}

