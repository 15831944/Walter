#include "StdAfx.h"
#include "CExcel.h"

static WCHAR* CharToWchar(const char * pchar)
{
	WCHAR *m_pWchar = NULL;

	int outlen = MultiByteToWideChar(CP_ACP,0,pchar,-1,NULL,0);
	m_pWchar = new WCHAR[outlen];
	MultiByteToWideChar(CP_ACP,0,pchar,-1,m_pWchar,outlen);

	return m_pWchar;
}

static CHAR* WCHARTOCHAR(const WCHAR * pchar)
{
	CHAR *m_pchar = NULL;

	int iSize = WideCharToMultiByte(CP_ACP,0,pchar,-1,NULL,0,NULL,NULL);
	//m_pchar = new CHAR[outlen];
	m_pchar = (char*)malloc((iSize+1)/**sizeof(char)*/);
	WideCharToMultiByte(CP_ACP, 0, pchar, -1, m_pchar, iSize, NULL, NULL);

	return m_pchar;
}

CExcelBackStyle::CExcelBackStyle()
{
	m_lColor = RGB(255,255,255);
	//背景图案
	m_sPattern = xlSolid;
	//背景图案颜色
	m_lPatternColor = RGB(255,0,0);
	//默认为不透明
    m_bTransparent = FALSE;
}
CExcelBorder::CExcelBorder()
{
	m_lColor = RGB(0, 0, 0);
	m_sLineStyle = xlContinuous;
	m_sWeight = xlHairline;
}
CExcelBorder::CExcelBorder(long color, short linestyle, short weigth)
{
	m_lColor = color;
	m_sLineStyle = linestyle;
	m_sWeight = weigth;
}
CExcellFont::CExcellFont()
{
	m_strName = "宋体";
	m_lSize = 12;
	m_lForeColor = RGB(0, 0, 0);
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bShadow = FALSE;
	m_bStrikethrough = FALSE;
	m_bSubscript = FALSE;
	m_bSuperscript = FALSE;
	m_sUnderline = xlUnderlineStyleNone;
}

CExcel::CExcel()
{
	m_bAppOk = FALSE;
	m_bBooksOk = FALSE;
	m_bBookOk = FALSE;
	m_bSheetsOk = FALSE;
	m_bSheetOk = FALSE;
	m_bRangeOk = FALSE;

	m_strCurFilePath = "";
}
CExcel::~CExcel()
{
	if (m_bRangeOk)
	{
		m_range.ReleaseDispatch();
	}
	if (m_bSheetOk)
	{
		m_sheet.ReleaseDispatch();
	}
	if (m_bSheetsOk)
	{
		m_sheets.ReleaseDispatch();
	}
	if (m_bBookOk)
	{
		m_book.ReleaseDispatch();
	}
	if (m_bBooksOk)
	{
		m_books.ReleaseDispatch();
	}
	if (m_bAppOk)
	{
		m_appExcel.ReleaseDispatch();
	}
	m_strCurFilePath = "";
//	CoUninitialize();//如果调用该函数,则关闭Excel后,不能再继续打开.
}
BOOL CExcel::OpenOfficeExcel2003()
{
	if (!m_appExcel.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("Can not open Microsoft Office 2003, Please check whether you have installed it!"));
		m_bAppOk = FALSE;
		return FALSE;
	}
	m_appExcel.put_Visible(TRUE);
	m_appExcel.put_UserControl(TRUE);
	m_bAppOk = TRUE;
	return TRUE;
}
BOOL CExcel::CloseOfficeExcel2003(char* pFileSavePath)
{
	if(!m_bAppOk || pFileSavePath == NULL)
	{
		return FALSE;
	}
	if (IsFileExist(pFileSavePath, FALSE))
	{
		DeleteFileA(pFileSavePath);
	}

	WCHAR *pw = CharToWchar(pFileSavePath);
	m_appExcel.SaveWorkspace(COleVariant(pw));
	m_appExcel.Quit();
	m_appExcel.ReleaseDispatch();
    delete [] pw;
	m_bAppOk = FALSE;
	return TRUE;
}
BOOL CExcel::Close(BOOL bSaveChanges)
{
	COleVariant covFalse((short)bSaveChanges);
	COleVariant	covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
	if (m_bRangeOk)
		m_range.ReleaseDispatch();
	m_bRangeOk = FALSE;
	if(m_bSheetOk)
		m_sheet.ReleaseDispatch();
	m_bSheetOk = FALSE;
	if(m_bSheetsOk)
		m_sheets.ReleaseDispatch();
	m_bSheetsOk = FALSE;
	if(m_bBookOk)	
	{
		m_book.Close(covFalse,covOptional,covOptional);
		m_book.ReleaseDispatch();
	}
	m_bBookOk = FALSE;
	if(m_bBooksOk)	
	{
		m_books.Close();
		m_books.ReleaseDispatch();
	}
	m_bBooksOk = FALSE;
	if(m_bAppOk)	
	{
		m_appExcel.Quit();
		m_appExcel.ReleaseDispatch();
	}
	else
	{
		return FALSE;
	}
	m_bAppOk = FALSE;
	return TRUE;
}
BOOL CExcel::SaveAs(char* pFileSavePath)
{
	if (IsFileExist(pFileSavePath,FALSE) == TRUE)
	{
		DeleteFileA(pFileSavePath);
	}
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk))
	{
		return FALSE;
	}
	COleVariant vtMissing((long)DISP_E_PARAMNOTFOUND,VT_ERROR),
		vtTrue((short)TRUE),vtFalse((short)FALSE);

	WCHAR *pw = CharToWchar(pFileSavePath);
	m_book.SaveAs(COleVariant(pw),
		vtMissing,
		vtMissing,
		vtMissing,
		vtMissing,
		vtMissing,
		0,
		vtMissing,
		vtMissing,
		vtMissing,
		vtMissing,
		vtMissing);
	delete [] pw;
	return TRUE;
}

BOOL CExcel::OpenWorkbook(char *pFileName)
{
	if (pFileName == NULL)
	{
		m_bBooksOk = FALSE;
		return FALSE;
	}
	COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR),
		vtTrue((short)TRUE),vtFalse((short)FALSE);
	m_books.AttachDispatch(m_appExcel.get_Workbooks(), TRUE);
	LPDISPATCH lpDisp = NULL;
	WCHAR *pw = CharToWchar(pFileName);
	lpDisp = m_books.Open(pw,
		vtOptional,
		COleVariant((long)TRUE),
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional );   
	if (lpDisp == NULL)
	{
		m_bBooksOk = FALSE;
		return FALSE;
	}
	m_book.AttachDispatch(lpDisp, TRUE);
	m_bBookOk = TRUE;
	m_bBooksOk = TRUE;
	m_strCurFilePath = pFileName;
	delete [] pw;
	return TRUE;
}

void CExcel::OpenExcel(CString fileName)
{        
	COleException pError;
	if (!m_appExcel.CreateDispatch((_T("Excel.Application")), &pError))

	{
		pError.ReportError();
		return;
	}
	else
	{
		COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		m_books = m_appExcel.get_Workbooks();
		m_book = m_books.Open(fileName,covOptional, covOptional, covOptional, 
			covOptional, covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
		m_sheet = m_book.get_ActiveSheet();
		//out_file_name = fileName;
	}
}

BOOL CExcel::AddWorkbook(char *pFilePath)
{
	if (pFilePath == NULL || (!m_bAppOk))
	{
		m_bBooksOk = FALSE;
		return FALSE;
	}
	COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR),
		vtTrue((short)TRUE),vtFalse((short)FALSE);
	m_books.AttachDispatch(m_appExcel.get_Workbooks(), TRUE);
	m_book = m_books.Add(vtOptional);
	WCHAR *pw = CharToWchar(pFilePath);
	m_book.SaveAs(COleVariant(pw), 
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		0,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional,
		vtOptional
		);
	
	m_bBooksOk = TRUE;
	m_bBookOk = TRUE;
	m_strCurFilePath = pFilePath;
	delete [] pw;
//	m_books.ReleaseDispatch();//注意:不能Release
	return TRUE;
}
BOOL CExcel::AddSheet(char*pSheetName)
{	
	if (!(m_bAppOk && m_bBooksOk))
	{
		return FALSE;
	}
	COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR),
				vtTrue((short)TRUE),
				vtFalse((short)FALSE);

	m_sheets = m_book.get_Sheets();
	m_bSheetsOk = TRUE;
	LPDISPATCH dispatchSheet = NULL;
	dispatchSheet = m_sheets.Add(vtOptional, vtOptional, COleVariant((short)1), vtOptional);
	if(dispatchSheet == NULL)
	{
		m_bSheetOk = FALSE;
		return FALSE;	
	}
	m_sheet.AttachDispatch(dispatchSheet, TRUE);
	m_bSheetOk = TRUE;

	WCHAR *pw = CharToWchar(pSheetName);
	m_sheet.put_Name(pw);
	delete [] pw;
	pw = NULL;
//	m_sheet.ReleaseDispatch();//注意:不能Release
	return TRUE;
}
BOOL CExcel::DeleteSheet(char*pSheetName)
{
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk))
	{
		return FALSE;
	}
	LPDISPATCH lpDispatch = NULL;
	WCHAR *pw = CharToWchar(pSheetName);
	lpDispatch = m_sheets.get_Item(COleVariant(pw));
	if (lpDispatch == NULL)
	{
		m_bSheetsOk = FALSE;
		m_bSheetOk = FALSE;
		return FALSE;
	}
	m_sheet.AttachDispatch(lpDispatch, TRUE);	
	m_sheet.Delete();
	m_bSheetOk = TRUE;
	delete [] pw;
	pw = NULL;
//	m_sheet.ReleaseDispatch();//注意:不能Release
	
	return TRUE;
}
BOOL CExcel::SetCurrentSheet(char *pSheetName)
{
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk))
	{
		return FALSE;
	}
	LPDISPATCH lpDisp = NULL;
	lpDisp = m_book.get_Sheets();
	if (lpDisp == NULL)
	{
		m_bSheetOk = FALSE;
		return FALSE;
	}
	m_sheets.AttachDispatch(lpDisp, TRUE);
	m_bSheetsOk = TRUE;

	lpDisp = NULL;
	WCHAR *pw = CharToWchar(pSheetName);
	lpDisp = m_sheets.get_Item(COleVariant(pw));
	if (lpDisp == NULL)
	{
		m_bSheetOk = FALSE;
		return FALSE;
	}
	m_sheet.AttachDispatch(lpDisp, TRUE);
	m_bSheetOk = TRUE;
	delete [] pw;
	pw = NULL;

// 	m_sheets.ReleaseDispatch();//注意:不能Release
// 	m_sheet.ReleaseDispatch();

	return TRUE;
}
BOOL CExcel::SetItemValue(int nRow, int nCol, char*pValue)
{
	if (pValue == NULL || !(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk))
	{
		m_bRangeOk = FALSE;
		return FALSE;
	}

// 	long lRow=0,lCol=0;	
// 	lRow = GetRowCount();
// 	lCol = GetColumnCount();
// 	if (nRow>lRow ||nCol>lCol)
// 	{
// 		CString strText;
// 		
// 		strText.Format("由于(%d,%d)已经超过了(%d,%d)的范围,所以在(%d,%d)设置失败!"
// 			,nRow,nCol,lRow,lCol,nRow,nCol);
// 		AfxMessageBox(_T(strText),MB_OK|MB_ICONERROR);
// 		
// 		return FALSE;
// 	}

	LPDISPATCH lpDisp = NULL;
	lpDisp = m_sheet.get_Cells();
	if (lpDisp == NULL)
	{
		m_bRangeOk = FALSE;
		return FALSE;
	}
	m_range.AttachDispatch(lpDisp, TRUE);
	WCHAR *pw = CharToWchar(pValue);
	m_range.put_Item(COleVariant((long)nRow), COleVariant((long)nCol), COleVariant(pw));
	m_bRangeOk = TRUE;
	
	m_book.Save();
	delete [] pw;
	pw = NULL;
	//SaveAs((char*)(LPCSTR)m_strCurFilePath);
	
	return TRUE;
}
BOOL CExcel::SetItemValue(char*pSheetName, int nRow, int nCol, char*pValue)
{
	if(!SetCurrentSheet(pSheetName))
	{
		m_bRangeOk = FALSE;
		return FALSE;
	}
	if (pValue == NULL || !(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk))
	{
		m_bRangeOk = FALSE;
		return FALSE;
	}

// 	long lRow=0,lCol=0;	
// 	lRow = GetRowCount();
// 	lCol = GetColumnCount();
// 	if (nRow>lRow ||nCol>lCol)
// 	{
// 		CString strText;
// 		
// 		strText.Format("由于(%d,%d)已经超过了(%d,%d)的范围,所以在(%d,%d)设置失败!"
// 			,nRow,nCol,lRow,lCol,nRow,nCol);
// 		AfxMessageBox(_T(strText),MB_OK|MB_ICONERROR);
// 		
// 		return FALSE;
// 	}

	LPDISPATCH lpDisp = NULL;
	lpDisp = m_sheet.get_Cells();
	if (lpDisp == NULL)
	{
		m_bRangeOk = FALSE;
		return FALSE;
	}
	m_range.AttachDispatch(lpDisp, TRUE);
	//注:SetItem函数的前两个参数只支持数字类型,不支持类似"A1", "B2"这样访问.
	WCHAR *pw = CharToWchar(pValue);
	m_range.put_Item(COleVariant((long)nRow), COleVariant((long)nCol), COleVariant(pw));	
	m_bRangeOk = TRUE;
	m_book.Save();
	delete [] pw;
	pw = NULL;
//	SaveAs((char*)(LPCSTR)m_strCurFilePath);

	return TRUE;
}

BOOL CExcel::SetColumnWidth(int nWidth)
{
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk))
	{
		m_bRangeOk = FALSE;
		return FALSE;
	}

	LPDISPATCH lpDisp = NULL;
	lpDisp = m_sheet.get_Cells();
	if (lpDisp == NULL)
	{
		m_bRangeOk = FALSE;
		return FALSE;
	}
	m_range.AttachDispatch(lpDisp, TRUE);
	

	m_range.put_ColumnWidth(COleVariant((short)nWidth));
	m_bRangeOk = TRUE;
	return TRUE;
}
BOOL CExcel::SetColumnWidth(char*pSheetName, int nWidth)
{
	if (pSheetName == NULL || !(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk && m_bRangeOk))
	{
		m_bRangeOk = FALSE;
		return FALSE;
	}
	if(!SetCurrentSheet(pSheetName))
	{
		m_bRangeOk = FALSE;
		return FALSE;
	}
	
	m_range.put_ColumnWidth(COleVariant((short)nWidth));
	m_bRangeOk = TRUE;
	return TRUE;
}
BOOL CExcel::SetRowHeight(int nWidth)
{
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk && m_bRangeOk))
	{
		m_bRangeOk = FALSE;
		return FALSE;
	}
	m_range.put_RowHeight(COleVariant((short)nWidth));
	m_bRangeOk = TRUE;
	return TRUE;
}
BOOL CExcel::CopyRange(char*pCharCellNameBeginSrc, char*pCharCellNameEndSrc, char*pCharCellNameDes)
{
	if (pCharCellNameBeginSrc == NULL || pCharCellNameEndSrc == NULL || pCharCellNameDes == NULL || !m_bRangeOk)
	{
		return FALSE;
	}
	CRange rangeCopyFrom;

	LPDISPATCH lpDisp = NULL;
	WCHAR *pw = CharToWchar(pCharCellNameBeginSrc);
	WCHAR *pw1 = CharToWchar(pCharCellNameEndSrc);
	lpDisp = m_sheet.get_Range(COleVariant(pw), COleVariant(pw1));
	if (lpDisp == NULL)
	{
		return FALSE;
	}
	rangeCopyFrom.AttachDispatch(lpDisp, TRUE);
	lpDisp = NULL;
	WCHAR *pw2 = CharToWchar(pCharCellNameDes);
	WCHAR *pw3 = CharToWchar(pCharCellNameDes);
	lpDisp = m_sheet.get_Range(COleVariant(pw2), COleVariant(pw3));
	m_range.AttachDispatch(lpDisp, TRUE);
	rangeCopyFrom.Copy(m_range.get_Value2());
	delete [] pw;
	pw = NULL;
	delete [] pw1;
	pw = NULL;
	delete [] pw2;
	pw = NULL;
	delete [] pw3;
	pw = NULL;
	return TRUE;
}
// char* CExcel::GetAppPath()
// {
// 	char lpFileName[MAX_PATH];
// 	
// 	GetModuleFileName(AfxGetInstanceHandle(),lpFileName,MAX_PATH);
// 	
// 	CString strFileName = lpFileName;
// 	int nIndex = strFileName.ReverseFind ('\\');
// 	
// 	CString strPath;
// 	
// 	if (nIndex > 0)
// 	{
// 		strPath = strFileName.Left (nIndex);
// 	}
// 	else
// 	{
// 		strPath = "";
// 	}
// 	
// 	return (char*)(LPCSTR)strPath;
// }
int CExcel::GetColumnCount()
{
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk && m_bRangeOk))
	{
		AfxMessageBox(_T("尚未选中一个区域,无法确定列数!"));
		return 0;
	}
	long lCount = 0l;
	CRange rg = m_range.get_EntireColumn();
	lCount = rg.get_Count();
	rg.ReleaseDispatch();
	return lCount;

}
int CExcel::GetRowCount()
{
	long lCount = 0l;
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk && m_bRangeOk))
	{
		AfxMessageBox(_T("尚未选中一个区域,无法确定行数!"));
		return 0;
	}
	CRange rg = m_range.get_EntireRow();
	lCount = rg.get_Count();
	rg.ReleaseDispatch();
	return lCount;
}
BOOL CExcel::SetCurrentRange(char*pBegin, char*pEnd)
{
	if (pBegin == NULL || pEnd == NULL || !(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk))
	{
		return FALSE;
	}
	WCHAR *pw = CharToWchar(pBegin);
	WCHAR *pw1 = CharToWchar(pEnd);
	m_range = m_sheet.get_Range(COleVariant(pw), COleVariant(pw1));
	m_bRangeOk = TRUE;
	delete [] pw;
	pw = NULL;
	delete [] pw1;
	pw = NULL;
	return TRUE;
}
CWorksheet CExcel::GetCurrentSheet()
{
	return m_sheet;
}
CRange CExcel::GetCurrentRange()
{
	return m_range;
}
BOOL CExcel::MergeRange(char*pBegin, char*pEnd)
{
	if (!SetCurrentRange(pBegin, pEnd))
	{
		return FALSE;
	}
	m_range.Merge(COleVariant((short)FALSE));
	return TRUE;
}
BOOL CExcel::RowAutoFit()
{
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk && m_bRangeOk))
	{
		return FALSE;
	}
	CRange rg = m_range.get_EntireRow();
	rg.AutoFit();
	rg.ReleaseDispatch();
	return TRUE;
}
BOOL CExcel::ColumnAutoFit()
{
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk && m_bRangeOk))
	{
		return FALSE;
	}
	CRange rg = m_range.get_EntireColumn();
	rg.AutoFit();
	rg.ReleaseDispatch();
	return TRUE;
}
BOOL CExcel::SetRangeHorAlignment(int nHorMode)
{
	if (!m_bRangeOk)
	{
		return FALSE;
	}
	m_range.put_HorizontalAlignment(COleVariant((short)nHorMode));
	return TRUE;
}
BOOL CExcel::SetRangeVerAlignment(int nVerMode)
{
	if (!m_bRangeOk)
	{
		return FALSE;
	}
	m_range.put_VerticalAlignment(COleVariant((short)nVerMode));
	return TRUE;
}
BOOL CExcel::SetRangeHorAlignment(char*pBegin, char* pEnd, int nHorMode)
{
	if (!m_bRangeOk)
	{
		return FALSE;
	}
	if(!SetCurrentRange(pBegin, pEnd))
	{
		return FALSE;
	}
	m_range.put_HorizontalAlignment(COleVariant((short)nHorMode));
	return TRUE;
}
BOOL CExcel::SetRangeVerAlignment(char*pBegin, char* pEnd, int nVerMode)
{
	if (!m_bRangeOk)
	{
		return FALSE;
	}
	if(!SetCurrentRange(pBegin, pEnd))
	{
		return FALSE;
	}
	m_range.put_VerticalAlignment(COleVariant((short)nVerMode));
	return TRUE;
}
BOOL CExcel::IsFileExist(char* pFilename, BOOL bDir)
{
	HANDLE h;
	LPWIN32_FIND_DATA pFD=new WIN32_FIND_DATA;
	BOOL bFound=FALSE;
	
	if (pFD)
	{
		WCHAR *pw = CharToWchar(pFilename);
		h=FindFirstFile(pw, pFD);
		bFound=(h!=INVALID_HANDLE_VALUE);
		if (bFound)
		{
			if (bDir)
				bFound= (pFD->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=NULL;
			FindClose(h);
		}
		delete [] pw;
		pw = NULL;
		delete pFD;
	}
	
	return bFound;
}
BOOL CExcel::SetRangeFont(CExcellFont& cf)
{
	if (!m_bRangeOk)
	{
		return FALSE;
	}
	CFont0 font;
	
	font =  m_range.get_Font();
	
	font.put_Name(COleVariant(cf.m_strName));
	font.put_Shadow(COleVariant((short)cf.m_bShadow));
	font.put_Size(COleVariant((short)cf.m_lSize));
	font.put_Underline(COleVariant((short)cf.m_sUnderline));
	font.put_Bold(COleVariant((short)cf.m_bBold));
	font.put_Color(COleVariant((long)cf.m_lForeColor));
	font.put_Italic(COleVariant((short)cf.m_bItalic));
	font.put_Strikethrough(COleVariant((short)cf.m_bStrikethrough));
	font.put_Subscript(COleVariant((short)cf.m_bSubscript));
	font.put_Superscript(COleVariant((short)cf.m_bSuperscript));
    font.ReleaseDispatch();
	return TRUE;
}
CString CExcel::GetItemValue(int nRow, int nCol)
{
	CString strRet = L"";
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk && m_bRangeOk))
	{
		strRet = "";
	}

// 	long lRow=0,lCol=0;	
// 	lRow = GetRowCount();
// 	lCol = GetColumnCount();
// 	if (nRow>lRow ||nCol>lCol)
// 	{
// 		CString strText;
// 		strText.Format("由于(%d,%d)已经超过了(%d,%d)的范围,所以在(%d,%d)设置失败!"
// 			,nRow,nCol,lRow,lCol,nRow,nCol);
// 		AfxMessageBox(_T(strText),MB_OK|MB_ICONERROR);
// 
// 	}
	VARIANT vtDis = m_range.get_Item(COleVariant((long)nRow), COleVariant((long)nCol));
	CRange rgValue;
	rgValue.AttachDispatch(vtDis.pdispVal, TRUE);
	VARIANT vtValue = rgValue.get_Value2();
	if (vtValue.vt == VT_EMPTY)
	{
		rgValue.ReleaseDispatch();
		strRet = _T("");
	}
	else
	{
		vtValue = rgValue.get_Text();
		strRet = vtValue.bstrVal;
	}
	return strRet;
}
CString CExcel::GetItemValue(char*pSheetName, int nRow, int nCol)
{
	CString strRet = L"";
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk && m_bRangeOk))
	{
		strRet = _T("");
	}
	if(!SetCurrentSheet(pSheetName))
	{
		return _T("");
	}
	VARIANT vtDis = m_range.get_Item(COleVariant((long)nRow), COleVariant((long)nCol));
	CRange rgValue;
	rgValue.AttachDispatch(vtDis.pdispVal, TRUE);
	VARIANT vtValue = rgValue.get_Value2();
	if (vtValue.vt == VT_EMPTY)
	{
		rgValue.ReleaseDispatch();
		strRet = _T("");
	}
	else
	{
		vtValue = rgValue.get_Text();
		strRet = vtValue.bstrVal;
	}
	return strRet;
}
BOOL CExcel::SetRangeBorderLine(short Xposition,CExcelBorder& XBorder)
{
	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk && m_bRangeOk))
	{
		return FALSE;
	}
	long Row,Col;
	Row=GetRowCount();
	Col=GetColumnCount();
	
	if (Row==1)
	{
		if (Xposition==xlInsideHorizontal) 
		{
			return  FALSE;
		}
		if (Col==1)
		{
			if (Xposition==xlInsideVertical) 
			{
				return FALSE;
			}
		}
	}
	LPDISPATCH  lpDisp=NULL;
	lpDisp=m_range.get_Borders();
	CBorders   bds;
	bds.AttachDispatch(lpDisp);
	CBorder   bd;   
	lpDisp=bds.get_Item((long)Xposition);
	bd.AttachDispatch(lpDisp);
	bd.put_LineStyle(COleVariant((short)XBorder.m_sLineStyle));
	bd.put_Color(COleVariant((long)XBorder.m_lColor));
	bd.put_Weight(COleVariant((short)XBorder.m_sWeight));
	bd.ReleaseDispatch();
    bds.ReleaseDispatch();
	return TRUE;
}
BOOL CExcel::SetBackStyle(CExcelBackStyle BackStyle)
{

	if (!(m_bAppOk && m_bBooksOk && m_bBookOk && m_bSheetsOk && m_bSheetOk && m_bRangeOk))
	{
		return FALSE;
	}

	LPDISPATCH  lpDisp=NULL;
	Cnterior Itor;
	
	lpDisp=m_range.get_Interior();
	Itor.AttachDispatch(lpDisp,TRUE);
	if (BackStyle.m_bTransparent==TRUE)
	{
		Itor.put_ColorIndex(COleVariant((short)xlNone));
	}
	else
	{
		Itor.put_Color(COleVariant((long)BackStyle.m_lColor));
		Itor.put_Pattern(COleVariant((short)BackStyle.m_sPattern));
		Itor.put_PatternColor(COleVariant((long)BackStyle.m_lPatternColor));
	}
	
	Itor.ReleaseDispatch();
	return TRUE;
}

void CExcel::SetVisible(bool bvi)
{        
	m_appExcel.put_Visible(bvi);
}