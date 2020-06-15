#include "stdafx.h"
#include "Com.h"

CString TY_GetAppPath()
{
#ifdef DEV_TEST_ENV
	WCHAR lpFileName[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), lpFileName, MAX_PATH);
	CString strFileName = lpFileName;
	int nIndex = strFileName.ReverseFind('\\');
	CString strPath;
	if (nIndex > 0)
	{
		strPath = strFileName.Left(nIndex);
	}
	else
	{
		strPath = "";
	}
	return strPath;
#else
	return L"C:\\Users\\nv4390\\Desktop\\20200612";
#endif // DEV_TEST_ENV
}
