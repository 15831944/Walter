#pragma once
#include <vector>

#import"C:\Program Files\Common Files\System\ado\msado15.dll"  rename("EOF","adoEOF"), rename("EOS","adoEOS"), rename("BOF","adoBOF")

using namespace ADODB;
using namespace std;

typedef vector<CString> OneRowData;
typedef vector<OneRowData> MultiRowData;

class CAccessDataBase
{
public:
	CAccessDataBase(CString connStr, CString userId, CString passwd);
	~CAccessDataBase();
	MultiRowData Execute(CString sql, UINT nColumn);
	void DownloadFile(CString sql, CString name, CString dir);
protected:
	_ConnectionPtr m_pConn;
	_RecordsetPtr m_pRecordSet;
};