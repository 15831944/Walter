#pragma once
class COpenExcelDialog
{
	
public:
	COpenExcelDialog(void);
	~COpenExcelDialog(void);
	static vCString TYCOM_GetFilesFromDialog(CString fileExt, int maxFileNumber = 256);
	static vCString TYCOM_GetFilesFromDialog( int maxFileNumber = 256);
};

