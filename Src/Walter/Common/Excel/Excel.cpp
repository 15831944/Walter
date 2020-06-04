#include "stdafx.h"

#include "Excel.h"

using namespace Excel;

void CExcelUtil::CreateExcel()
{
    COleException pError;
    if (!m_excelApp.CreateDispatch(_T("Excel.Application"), &pError))
    {
        pError.ReportError();
        return;
    }
    else
    {
        m_excelBooks = m_excelApp.get_Workbooks();
        m_excelBook = m_excelBooks.Add(COleVariant((short)TRUE));
        m_excelSheet = m_excelBook.get_ActiveSheet();
    }
}

static int StrToVariant(LPCTSTR input, VARIANT &output)
{
	BSTR x = SysAllocString(input);
	output.vt = VT_BSTR;
	output.bstrVal = x;
	SysFreeString(x); 

	return 0;
}

bool CExcelUtil::OpenExcel(CString fileName, bool readOnly)
{        
    COleException pError;
    if (!m_excelApp.CreateDispatch((_T("Excel.Application")), &pError))

    {
        pError.ReportError();
        return false;
    }
    else
    {
        COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
        m_excelBooks = m_excelApp.get_Workbooks();

		VARIANT output;
		StrToVariant(L"2018", output);

		if(readOnly)
		{
			int lValue = 1;  
			VARIANT vParam;  
			vParam.vt = VT_I4;  
			vParam.lVal = lValue;  

			m_excelBook = m_excelBooks.Open(fileName,covOptional, vParam, covOptional, 
				output, output, covOptional, covOptional, covOptional,
				covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
		}
		else
		{
			m_excelBook = m_excelBooks.Open(fileName,covOptional, covOptional, covOptional, 
				output, output, covOptional, covOptional, covOptional,
				covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
		}

		if (m_excelBooks==NULL)
		{
			return false;

		}

		m_excelSheets = m_excelBook.get_Sheets();
        m_excelSheet = m_excelBook.get_ActiveSheet();
        out_file_name = fileName;
    }

	return true;
}

void CExcelUtil::SetVisible(bool bvi)
{        
	m_excelApp.put_Visible(bvi);
}

bool CExcelUtil::SaveExcel()
{
	m_excelBook.Save();
	return false;

    if (out_file_name.GetLength()==0)
    {
        //AfxMessageBox("Excel路径未指定,无法保存", MB_OK);
        return false;
    }
    else
    {
        TRY 
        {
            //if (_access(out_file_name, 0) == 0)
            //  remove(out_file_name);
            COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
            m_excelBook.SaveAs(COleVariant(out_file_name),  covOptional, covOptional,
                covOptional, covOptional, covOptional, 
                0,covOptional, covOptional, covOptional, covOptional, covOptional);
        }
        CATCH (CException, e)
        {
            return false;
        }
        END_CATCH
    }
    return false;
}

void CExcelUtil::CloseExcel()
{    
	

    COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
    m_excelBook.Close(COleVariant((short)FALSE), covOptional, covOptional);
    m_excelBooks.Close();
   

	m_excelRange.ReleaseDispatch();  
	m_excelSheet.ReleaseDispatch();  
	m_excelSheets.ReleaseDispatch();  
	m_excelBook.ReleaseDispatch();  
	m_excelBooks.ReleaseDispatch(); 

	m_excelApp.Quit();
	m_excelApp.ReleaseDispatch();
}

// <summary>
// 取得特定的Worksheet
//</summary>
// <param name="SheetName">worksheet的名字</param>
//<returns></returns>
CWorksheet CExcelUtil::GetSheet(CString SheetName)
{
    bool has_the_sheet = false;
    CWorksheets sheets = m_excelBook.get_Worksheets();
    for (int si=1; si<=sheets.get_Count(); si++)
    {
        CWorksheet sheet = sheets.get_Item(COleVariant((long)si));
        if (sheet.get_Name().Compare(SheetName) == 0)
        {
            has_the_sheet = true;
            break;
        }
    }

    if (!has_the_sheet)
    {
        //AfxMessageBox("Sheet名字不对，请检查");
        return NULL;
    }
    TRY 
    {
        m_excelSheet = sheets.get_Item(COleVariant(SheetName));
    }
    CATCH (CException, e)
    {
        return NULL;
    }
    END_CATCH
        return m_excelSheet;
}

/// <summary>
/// 取得特定的Worksheet
/// </summary>
/// <param name="index">worksheet的索引，从1开始</param>
/// <returns></returns>

CWorksheet CExcelUtil::GetSheet(int index)
{
    CWorksheets sheets = m_excelBook.get_Worksheets();
    int sheet_num = sheets.get_Count();
    if (index <= 0 || index > sheet_num)
    {
        char buf[BUFSIZ] = "";
        sprintf_s(buf, "索引必须大于0且小于等于%d", sheet_num);
        //AfxMessageBox(buf);
        return NULL;
    }
    TRY
    {
        m_excelSheet = sheets.get_Item(COleVariant((long)(index)));
    }
    CATCH (CException, e)
    {
        return NULL;
    }
    END_CATCH
        return m_excelSheet;

}

/// <summary>
/// 添加特定的Worksheet
/// </summary>
/// <param name="SheetName">新添加的worksheet的名字</param>
/// <returns>成功返回创建的worksheet，否则控制台打印相应的出错信息</returns>
CWorksheet CExcelUtil::AddSheet(CString SheetName)
{
    if( SheetName.Trim().GetLength() == 0 )
    {
        //AfxMessageBox("sheet名字非法");
        return NULL;
    }
    bool has_the_sheet = false;
    CWorksheets sheets = m_excelBook.get_Worksheets();
    for (int si=1; si<=sheets.get_Count(); si++)
    {
        CWorksheet sheet = sheets.get_Item(COleVariant((long)si));
        if (sheet.get_Name().Compare(SheetName) == 0)
        {
            has_the_sheet = true;
            break;
        }
    }

    if( has_the_sheet )
    {
        //AfxMessageBox("excel文件中已有该sheet");
        return NULL;
    }

    CWorksheet new_sheet;
    TRY 
    {
        COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
        new_sheet = sheets.Add(covOptional, covOptional, COleVariant((short)1), covOptional);
        new_sheet.put_Name(SheetName);
    }
    CATCH (CException, e)
    {
        return NULL;
    }
    END_CATCH
        return new_sheet;
}

/// <summary>
/// 取得sheet的数量
/// </summary>
/// <returns>sheet的数量</returns>
int CExcelUtil::GetSheetNum()
{
    CWorksheets sheets = m_excelBook.get_Worksheets();
    return sheets.get_Count();
}

/// <summary>
/// 设置当前的sheet
/// </summary>
/// <returns>成功0，否则其他</returns>
int CExcelUtil::SetActiveSheet(long iSheetName)
{
	CWorksheets sheets = m_excelBook.get_Worksheets();

	m_excelSheet =sheets.get_Item(COleVariant(iSheetName));
	m_excelSheet.Activate();
	return 0;
}

/// <summary>
/// 设置一个sheet的某个单元格的值
/// </summary>
/// <returns>成功0，否则其他</returns>
int CExcelUtil::SetSheetCellValue(long iSheetName, long irow, long icol,VARIANT &vt)
{
	CWorksheets sheets = m_excelBook.get_Worksheets();

	m_excelSheet =sheets.get_Item(COleVariant(iSheetName));
	m_excelSheet.Activate();
	CRange range;
	range.AttachDispatch(m_excelSheet.get_Cells(),true);
	range.put_Item(COleVariant(irow),COleVariant(icol),COleVariant(vt));
	return 0;
}

/// <summary>
/// 得到一个sheet的某个单元格的值
/// </summary>
/// <returns>成功0，否则其他</returns>
int CExcelUtil::GetSheetCellValue(long iSheetName, long irow, long icol,VARIANT &vt)
{
	CWorksheets sheets = m_excelBook.get_Worksheets();

	m_excelSheet =sheets.get_Item(COleVariant(iSheetName));
	m_excelSheet.Activate();
	CRange range;
	range.AttachDispatch(m_excelSheet.get_Cells(),true);
	vt = range.get_Item(COleVariant(irow),COleVariant(icol));
	return 0;
}

int CExcelUtil::GetSheetCellValue(long irow, long icol,VARIANT &vt)
{
	CRange range;
	range.AttachDispatch(m_excelSheet.get_Cells(),true);
	vt = range.get_Item(COleVariant(irow),COleVariant(icol));
	return 0;
}

//操作单元格/合并单元格
/// <summary>
/// 返回特定的单元格
/// </summary>
/// <param name="ColNum">列号,从1开始,不大于255</param>
/// <param name="RowNum">行号,从1开始,不大于255</param>
/// <returns></returns>
CString CExcelUtil::GetCell(int ColNum, int RowNum)
{
    char buf[BUFSIZ] = "";
    //单元格列号数组
    CString m_colString[] = {
        _T(""),
        _T("A"),_T("B"),_T("C"),_T("D"),_T("E"),
        _T("F"),_T("G"),_T("H"),_T("I"),_T("J"),
        _T("K"),_T("L"),_T("M"),_T("N"),_T("O"),
        _T("P"),_T("Q"),_T("R"),_T("S"),_T("T"),
        _T("U"),_T("V"),_T("W"),_T("X"),_T("Y"),
        _T("Z") };

        int row = RowNum;
        if (ColNum <= 0 || ColNum > 255)
        {
            //AfxMessageBox("行号错误");
            return _T("");
        }
        if (ColNum <= 26)
        {
            sprintf_s(buf, "%s%d", m_colString[ColNum], RowNum);
            return CString(buf);
        }
        else
        {
            int i0 = 0, i1 = 0;
            i0 = (int)(ColNum / 26);
            i1 = ColNum % 26;
            if (i1 != 0)
            {
                sprintf_s(buf, "%s%s%d", m_colString[i0], m_colString[i1], row);
                return CString(buf);
            }
            else //如果ColNum = 52、78、104
            {
                sprintf_s(buf, "%s%s%d", m_colString[i0-1], m_colString[i1+26], row);
                return CString(buf);
            }
        }
}

//取得一个单元格
void CExcelUtil::SetRange(int ColNum, int RowNum)
{
    COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
    m_excelRange = m_excelSheet.get_Range(COleVariant(GetCell(ColNum, RowNum)), covOptional);
}

//取得一个单元格区域
void CExcelUtil::SetRange(int StartColNum, int StartRowNum, int EndColNum, int EndRowNum)
{
    m_excelRange = m_excelSheet.get_Range(COleVariant(GetCell(StartColNum, StartRowNum)),
        COleVariant(GetCell(EndColNum, EndRowNum)));
}

//合并一个单元格区域
void CExcelUtil::Merge(int startRowIndex, int startColIndex, int endRowIndex, int endColIndex)
{
    SetRange(startColIndex, startRowIndex, endColIndex, endRowIndex);
    m_excelRange.Merge(COleVariant((short)FALSE));
}

//单元格赋值/单元格区域赋值
void CExcelUtil::SetCellValue(CString value)
{
    if (m_excelRange == NULL) { AfxMessageBox(_T("没有设定单元格")); return; }
    m_excelRange.put_Value2(COleVariant(value));
}

void CExcelUtil::SetCellValue(int row, int col, CString value)
{
    SetRange(col, row);
    m_excelRange.put_NumberFormatLocal(COleVariant(_T("@")));
    m_excelRange.put_Value2(COleVariant(value));
}

void CExcelUtil::SetCellFormula(int row, int col, CString value)
{
	SetRange(col, row);
	//m_excelRange.put_NumberFormatLocal(COleVariant(_T("@")));
	m_excelRange.put_Formula(COleVariant(value));
}

void CExcelUtil::SetCellActive(int row, int col)
{
	SetRange(col, row);
	//m_excelRange.put_NumberFormatLocal(COleVariant(_T("@")));
	m_excelRange.Activate();
}

void CExcelUtil::SetCellStringFormat()
{
    m_excelRange.put_NumberFormatLocal(COleVariant(_T("@")));
}

void CExcelUtil::SetCellValue( int startRow,int startCol, int endRow, int endCol, CString value )
{
    Merge(startRow, startCol, endRow, endCol);
    m_excelRange.put_NumberFormatLocal(COleVariant(_T("@")));
    m_excelRange.put_Value2(COleVariant(value));
}

CString CExcelUtil::GetCellValue( int row, int col )
{
    SetRange(col, row);
    return (CString)m_excelRange.get_Text();
}

//设置单元格样式
void CExcelUtil::SetBoldFontAndSize( int size )
{
    CFont0 font = m_excelRange.get_Font();
    font.put_Bold(COleVariant((short)TRUE));
    font.put_Size(COleVariant((short)size));
}

//设置报表表格为最适应宽度
//设置报表表格为最适应宽度
void CExcelUtil::SetAutoFitColumns( int startRow, int startCol, int endRow, int endCol )
{
    SetRange(startCol, startRow, endCol, endRow);
    CRange cols = m_excelRange.get_EntireColumn();
    cols.AutoFit();
}

void CExcelUtil::SetCenterAlign()
{
    m_excelRange.put_HorizontalAlignment(COleVariant((short)0));
}

void CExcelUtil::SetBorder()
{
    CBorders borders = m_excelRange.get_Borders();
    borders.put_LineStyle(COleVariant((short)1));
    CBorder border = borders.get_Item(7);
    border.put_Weight(COleVariant((short)4));
    border = borders.get_Item(8);
    border.put_Weight(COleVariant((short)4));
    border = borders.get_Item(10);
    border.put_Weight(COleVariant((short)4));
    border = borders.get_Item(9);
    border.put_Weight(COleVariant((short)4));
}

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

BOOL CExcelUtil::IsFileExist(char* pFilename, BOOL bDir)
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

BOOL CExcelUtil::SaveAs(char* pFileSavePath)
{
	if (IsFileExist(pFileSavePath,FALSE) == TRUE)
	{
		DeleteFileA(pFileSavePath);
	}

	COleVariant vtMissing((long)DISP_E_PARAMNOTFOUND,VT_ERROR),
		vtTrue((short)TRUE),vtFalse((short)FALSE);

	WCHAR *pw = CharToWchar(pFileSavePath);
	m_excelBook.SaveAs(COleVariant(pw),
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
	out_file_name = pFileSavePath;
	delete [] pw;
	return TRUE;
}

int CExcelUtil::SetSheetName(int index, CString newName)
{
	CWorksheet sheet = GetSheet(index);
	sheet.put_Name(newName);
	return 0;
}

int CExcelUtil::DeleteSheet(int index)
{
	CWorksheet sheet = GetSheet(index);
	sheet.Delete();
	return 0;
}

CString CExcelUtil::GetSheetName(int index)
{
	CWorksheet sheet = GetSheet(index);
	return sheet.get_Name();
}