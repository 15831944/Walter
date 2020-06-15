#include "stdafx.h"
#include "OpenExcelDialog.h"



COpenExcelDialog::COpenExcelDialog(void)
{
}


COpenExcelDialog::~COpenExcelDialog(void)
{
}
//���ļ��Ի���
vCString COpenExcelDialog::TYCOM_GetFilesFromDialog(CString fileExt, int maxFileNumber)
{
	CString strFilter;
	strFilter.Format(_T("%s Files (*.%s)|*.%s|All Files (*.*)|*.*||"),fileExt,fileExt,fileExt);

	CString prompt;
	prompt.Format(_T(" %s file(*.%s)"),fileExt,fileExt);

	CFileDialog FileDlg (true, prompt, NULL,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, strFilter);

	vCString files;
	const DWORD numberOfFileNames = maxFileNumber;//�������32���ļ�
	const DWORD fileNameMaxLength = MAX_PATH + 1;
	const DWORD bufferSize = (numberOfFileNames * fileNameMaxLength) + 1;
	TCHAR* filenamesBuffer = new TCHAR[bufferSize];
	// Initialize beginning and end of buffer.
	filenamesBuffer[0] = NULL;//�����
	filenamesBuffer[bufferSize - 1] = NULL;

	// Attach buffer to OPENFILENAME member.
	FileDlg.m_ofn.lpstrFile = filenamesBuffer;
	FileDlg.m_ofn.nMaxFile = bufferSize;
	if (IDOK == FileDlg.DoModal())
	{
		CString aviName;
		POSITION pos = FileDlg.GetStartPosition();
		while (pos != NULL)
		{
			aviName = FileDlg.GetNextPathName(pos);//����ѡ���ļ��ļ���// Retrieve file name(s).
			files.push_back(aviName);
		}
	}

	return files;
}


//���ļ��Ի���2
vCString COpenExcelDialog::TYCOM_GetFilesFromDialog( int maxFileNumber)
{
	CString strFilter;
	strFilter.Format(_T("xls Files (*.xls)|*.xls|xlsx Files (*.xlsx)|*.xlsx|All Files (*.*)|*.*||"));

	CString prompt;
	prompt.Format(_T("Select xls file(*.xls)|Select xlsx file(*.xlsx)"));

	CFileDialog FileDlg (true, NULL, NULL,
		OFN_FILEMUSTEXIST| OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT, strFilter);

	vCString files;
	const DWORD numberOfFileNames = maxFileNumber;//�������32���ļ�
	const DWORD fileNameMaxLength = MAX_PATH + 1;
	const DWORD bufferSize = (numberOfFileNames * fileNameMaxLength) + 1;
	TCHAR* filenamesBuffer = new TCHAR[bufferSize];
	// Initialize beginning and end of buffer.
	filenamesBuffer[0] = NULL;//�����
	filenamesBuffer[bufferSize - 1] = NULL;

	// Attach buffer to OPENFILENAME member.
	FileDlg.m_ofn.lpstrFile = filenamesBuffer;
	FileDlg.m_ofn.nMaxFile = bufferSize;
	if (IDOK == FileDlg.DoModal())
	{
		CString aviName;
		POSITION pos = FileDlg.GetStartPosition();
		while (pos != NULL)
		{
			aviName = FileDlg.GetNextPathName(pos);//����ѡ���ļ��ļ���// Retrieve file name(s).
			files.push_back(aviName);
		}
	}

	return files;
}
CString COpenExcelDialog::HS_GetAppPath()
{
	return L"C:\\Users\\nv4390\\Desktop\\20200612";
	/*WCHAR lpFileName[MAX_PATH];
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
	return strPath;*/
}