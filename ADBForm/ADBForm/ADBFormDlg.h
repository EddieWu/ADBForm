// ADBFormDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CADBFormDlg �Ի���
class CADBFormDlg : public CDialog
{
// ����
public:
	CADBFormDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ADBFORM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	static DWORD LogInfoShow(LPVOID lpParam);
	DWORD LogInfoShowFunc(WPARAM wPamam, LPARAM lParam);
public:
	CEdit Cot_msginfoshow;
};
