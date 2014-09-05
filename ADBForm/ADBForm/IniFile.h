// IniFile.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__8577D2E3_5901_4D38_AD98_AEB837760A63__INCLUDED_)
#define AFX_INIFILE_H__8577D2E3_5901_4D38_AD98_AEB837760A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIniFile  
{
public:
	CIniFile();

	virtual ~CIniFile();
	//    设置ini文件路径
    //    成功返回TRUE;否则返回FALSE
	BOOL  SetPath(CString strPath);
	CString  GetPath();
	//    检查section是否存在
    //    存在返回TRUE;否则返回FALSE
	BOOL  SectionExist(CString strSection);
	//    从指定的Section和Key读取KeyValue
	//    返回KeyValue
	CString GetKeyStrValue(CString    strSection,CString    strKey);    
	CString GetKeyStrValue(CString strSection,CString strKey,CString DefalutVal);
	//    设置Section、Key以及KeyValue，若Section或者Key不存在则创建
	int   GetKeyIntValue(CString strSection,CString strKey,int defaultInt);
	double GetKeyDblValue(CString strSection,CString strKey,double defaultDbl);

	void  SetKeyStrValue(CString    strSection,CString    strKey,CString    strKeyValue);
	void  SetKeyNumValue(CString strSection,CString strKey,int intKeyValue);
	void  SetKeyNumValue(CString strSection,CString strKey,double dlbKeyValue);
	//    删除指定Section下的一个Key
	void  DeleteKey(CString strSection,	CString strKey);
	//    删除指定的Section以及其下的所有Key
	void  DeleteSection(CString strSection);
	//    获得所有的Section
	//    返回Section数目
	int   GetAllSections(CStringArray& strArrSection);
	//    根据指定Section得到其下的所有Key和KeyValue
	//    返回Key的数目
	int   GetAllKeysAndValues(CString strSection,
		CStringArray& strArrKey,
		CStringArray& strArrKeyValue);
	//       删除所有Section
	void  DeleteAllSections();
	CString  GetLastErrorDetail(int flag=0); // 增加获取错误号后，清空错误信息
	void  SetCheckExist( BOOL val);
	bool CheckKeyExist(CString strSection,CString strKey);
	void SetEnableExtendKey( bool val );
private:
	//       ini文件路径
         CString m_strPath;
		 bool blCheckExist;
		 CString strErrorDetail;
		 bool blExtendKey;
		 void doCheckExist(CString strSection,CString strKey);
		 DWORD GetPrivateProfileStringEx(LPCTSTR strSection,LPCTSTR strKey,LPCTSTR strDefault,TCHAR* chKey,int nSize ,LPCTSTR m_strPath);
};

#endif // !defined(AFX_INIFILE_H__8577D2E3_5901_4D38_AD98_AEB837760A63__INCLUDED_)
