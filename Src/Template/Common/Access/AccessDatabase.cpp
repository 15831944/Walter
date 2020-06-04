#include "../../stdafx.h"
#include "AccessDatabase.h"
#include <fstream>

CAccessDataBase::CAccessDataBase(CString connStr, CString userId, CString passwd)
{
	AfxEnableControlContainer();
	if (!AfxOleInit())
	{
		MessageBox(NULL, _T("数据库初始化失败"), _T("创建连接失败"), MB_OK);
		return;
	}
	try
	{
		m_pConn.CreateInstance("ADODB.Connection");
		m_pConn->ConnectionTimeout = 3;
		m_pConn->Open(_bstr_t(connStr), _bstr_t(userId), _bstr_t(passwd), adModeUnknown);
	}
	catch (_com_error &e)
	{
		MessageBox(NULL, e.Description(), _T("创建连接失败"), MB_OK);
	}
}

CAccessDataBase::~CAccessDataBase()
{
	try
	{
		if (m_pConn != NULL)
		{
			m_pConn->Close();
		}
	}
	catch (_com_error &e)
	{
		MessageBox(NULL, e.Description(), _T("断开连接失败"), MB_OK);
	}
}

MultiRowData CAccessDataBase::Execute(CString sql, UINT nColumn)
{
	MultiRowData resultData;
	OneRowData currRow;
	CString currData;
	try
	{
		m_pRecordSet = m_pConn->Execute(_bstr_t(sql), NULL, adCmdText);
		while (!m_pRecordSet->adoEOF)
		{
			currRow.clear();
			for (UINT i = 0; i < nColumn; i++)
			{
				currData = (LPCSTR)_bstr_t(m_pRecordSet->GetCollect(_variant_t((long)i)));
				currRow.push_back(currData);
			}
			resultData.push_back(currRow);
			m_pRecordSet->MoveNext();
		}

		m_pRecordSet->Close();
	}
	catch (_com_error &e)
	{
		MessageBox(NULL, e.Description(), _T("无法执行查询语句"), MB_OK);
		resultData.clear();
	}
	return resultData;
}

void CAccessDataBase::DownloadFile(CString sql, CString name, CString dir)
{
	try
	{
		m_pRecordSet = m_pConn->Execute(_bstr_t(sql), NULL, adCmdText);

		long dataSize;
		_variant_t dwgFile;
		char* buffer = NULL;
		ofstream outputFile;
		if (!m_pRecordSet->adoEOF)
		{
			dataSize = m_pRecordSet->GetFields()->GetItem(_variant_t((long)0))->ActualSize;
			dwgFile = m_pRecordSet->GetFields()->GetItem(_variant_t((long)0))->GetChunk(dataSize);
			if (dwgFile.vt == (VT_ARRAY | VT_UI1))
			{
				SafeArrayAccessData(dwgFile.parray, (void**)&buffer);
				SafeArrayUnaccessData(dwgFile.parray);

			}
			outputFile.open(dir + name + _T(".dwg"), ios::binary);
			if (outputFile.is_open())
			{
				outputFile.write(buffer, dataSize);
				outputFile.close();
			}
		}
		m_pRecordSet->Close();
	}
	catch (_com_error &e)
	{
		MessageBox(NULL, e.Description(), _T("下载文件失败"), MB_OK);
	}
}
