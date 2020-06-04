#ifndef	CEXCEL_HEADER_FILE
#define CEXCEL_HEADER_FILE

#include "..\\Excel\CApplication.h"
#include "..\\Excel\CFont0.h"
#include "..\\Excel\CRange.h"
#include "..\\Excel\CRanges.h"
#include "..\\Excel\CWorkbook.h"
#include "..\\Excel\CWorkbooks.h"
#include "..\\Excel\CWorksheet.h"
#include "..\\Excel\CWorksheets.h"
#include "..\\Excel\CBorders.h"
#include "..\\Excel\CBorder.h"
#include "..\\Excel\Cnterior.h"

#ifdef DLL_EXCEL_API
#else 
#define DLL_EXCEL_API /*extern "C" */_declspec(dllimport)
#endif

// 	Excel接口类中层次如下所示：
// 		-_Application
// 			-Workbooks 工作薄集合
// 				-_Workbook 工作薄
// 					-Worksheets 工作表集合
// 						-_Worksheet 工作表
// 							-Range 单元格区域

//为了提高效率,此处用const变量,而非define宏.

/************************************************************************/
/*                                背景图案的类型                        */
/************************************************************************/

//背景图案的类型
const int xlSolid = 1;
const int xlGray8 = 18;
const int xlGray75 = -4126;
const int xlGray50 = -4125;
const int xlGray25 = -4124;
const int xlGray16 = 17;
const int xlHorizontal = -4128;
const int xlVertical = -4166;
const int xlDown = -4121;
const int xlUp = -4162;
const int xlChecker = 9;
const int xlSemiGray75 = 10;
const int xlLightHorizontal = 11;
const int xlLightVertical = 12;
const int xlLightDown = 13;
const int xlLightUp = 14;
const int xlGrid = 15;
const int xlCrissCross = 16;


/************************************************************************/
/*                                边框参数                              */
/************************************************************************/
//边框方位
const int xlDiagonalDown = 5;//这两种设置颜色不生效.不知为何.
const int xlDiagonalUp = 6;//
const int xlEdgeBottom = 9;
const int xlEdgeLeft = 7;
const int xlEdgeRight = 10;
const int xlEdgeTop = 8;
const int xlInsideHorizontal = 12;
const int xlInsideVertical = 11;
//边框划线类型
const int xlContinuous = 1;
const int xlDash = -4115;
const int xlDashDot = 4;
const int xlDashDotDot = 5;
const int xlDot = -4118;
const int xlDouble = -4119;
const int xlLineStyleNone = -4142;
const int xlNone = -4142;
const int xlSlantDashDot = 13;
//边框划线的粗细
const int xlHairline = 1;
const int xlMedium = -4138;
const int xlThick = 4;
const int xlThin  = 2;


/************************************************************************/
/*                                字体参数                              */
/************************************************************************/
//字体水平对齐方式
const int xlGeneral = 1;
const int  xlCenter = -4108;
const int  xlLeft = -4131;
const int  xlRight = -4152;
const int  xlFill = 5;
const int  xlJustify = -4130;
//字体垂直对齐方式
const int  xlTop = -4160;
const int  xlBottom = -4107;
//下滑线的种类
const int xlUnderlineStyleDouble = -4119;
const int xlUnderlineStyleDoubleAccounting = 5;
const int xlUnderlineStyleNone = -4142;
const int xlUnderlineStyleSingle = 2;
const int xlUnderlineStyleSingleAccounting = 4;

//背景设置参数类
class DLL_EXCEL_API CExcelBackStyle
{
public:	
	//构造函数
	CExcelBackStyle();
	//背景颜色
	long m_lColor;
	//背景图案
	short m_sPattern;
	//背景图案颜色
	long m_lPatternColor;
	//透明不透明
	BOOL m_bTransparent;

};


//边框设置参数类
class DLL_EXCEL_API CExcelBorder
{
public:
	//构造函数
	CExcelBorder();
	CExcelBorder(long color, short linestyle, short weigth);
	//线条形状
	short m_sLineStyle;
	//粗线
	short m_sWeight;
	//颜色
	long m_lColor;

};
//字体设置参数类
class DLL_EXCEL_API CExcellFont
{
public:
	//构造函数
	CExcellFont();
// 	CExcellFont(char* pFontName, long size,
// 		long foreColor, BOOL Bold, BOOL Italic,
// 		BOOL Strikethrough, BOOL Shadow,
// 		BOOL Subcript, BOOL Shadow, 
// 		short Underline);
// 	CExcellFont(char* pFontName, long size, long foreColor, BOOL Bold);
// 	CExcellFont(char* pFontName, long size, long foreColor);
// 	CExcellFont(char* pFontName, long size);
	//名字
	CString m_strName;
	//大小
	long m_lSize;
	//前景
	long m_lForeColor;
	//粗体
	BOOL m_bBold;
	//斜体
	BOOL m_bItalic;
	//中间线
	BOOL m_bStrikethrough;
	//阴影
	BOOL m_bShadow;
	//下标
	BOOL m_bSubscript;
	//上标
	BOOL m_bSuperscript;
	//下划线
	short m_sUnderline;

};

class DLL_EXCEL_API CExcel
{
	private:
		CString m_strCurFilePath;
	private:
		BOOL m_bAppOk;
		BOOL m_bBooksOk;
		BOOL m_bBookOk;
		BOOL m_bSheetsOk;
		BOOL m_bSheetOk;
		BOOL m_bRangeOk;
	private:
		CApplication m_appExcel;
		CWorkbooks m_books;
		CWorkbook m_book;
		CWorksheets m_sheets;
		CWorksheet m_sheet;
		CRange m_range;
	public:
		CExcel();
		~CExcel();
		BOOL OpenOfficeExcel2003();//打开Office 2003. true, 成功; false,失败
		BOOL CloseOfficeExcel2003(char*pFileSavePath);//关闭Office 2003并将文件保存到指定路径. true,成功; false,失败
		BOOL SaveAs(char* pFileSavePath);//将文件另存为,但不关闭Excel
		BOOL Close(BOOL bSaveChanges=TRUE);//关闭Excel. bSaveChanges,是否保存更改的文件
		void SetVisible(bool bvi);
		BOOL OpenWorkbook(char *pFileName);//打开(一个)工作簿.true,成功;false,失败
		void OpenExcel(CString fileName);

		BOOL AddWorkbook(char *pFilePath);//创建一个新的工作薄. true,成功;false,失败
		BOOL AddSheet(char*pSheetName=NULL);//添加一个sheet.pSheetName=NULL,默认名称(sheet1).
		BOOL DeleteSheet(char*pSheetName);//删除一个sheet
		BOOL SetCurrentSheet(char *pSheetName);//设置当前Sheet. pSheetName,sheet 名称
		CWorksheet GetCurrentSheet();//获得当前的工作表(sheet)
		BOOL SetCurrentRange(char*pBegin, char*pEnd);//设置当前的选择区域(Range)
		CRange GetCurrentRange();//获得当前的选择区域
		BOOL SetItemValue(int nRow, int nCol, char*pValue);//设置某行某列的值
		BOOL SetItemValue(char*pSheetName, int nRow, int nCol, char*pValue);//重载设置某行某列的值
		CString GetItemValue(int nRow, int nCol);//获得某行某列的值
		CString GetItemValue(char*pSheetName, int nRow, int nCol);//重载获得某行某列的值
		BOOL SetColumnWidth(int nWidth);//设置列宽
		BOOL SetColumnWidth(char*pSheetName, int nWidth);//重载设置列宽
		BOOL SetRowHeight(int nHeight);//设置行高
		BOOL SetRowHeight(char*pSheetName, int nHeight);
		BOOL CopyRange(char*pCharCellNameBegin, char*pCharCellNameEnd, char*pCharCellNameDes);
		int GetColumnCount();//获得列数//这两个函数容易出错(必须要保证m_range选中一个区域),编程时也较少用到.
		int GetRowCount();//获得行数
		BOOL MergeRange(char*pBegin, char*pEnd);//合并单元格
		BOOL ColumnAutoFit();//列自动展开
		BOOL RowAutoFit();//行自动展开	
		BOOL SetRangeHorAlignment(int nHorMode);//设置水平对齐方式
		BOOL SetRangeHorAlignment(char*pBegin, char* pEnd, int nHorMode);//重载
		BOOL SetRangeVerAlignment(int nVerMode);//设置垂直对齐方式
		BOOL SetRangeVerAlignment(char*pBegin, char* pEnd, int nVerMode);//重载设置垂直对齐方式
		BOOL IsFileExist(char* pFilename, BOOL bDir);			
		BOOL SetRangeFont(CExcellFont& cf);//设置选择区域的字体
		BOOL SetRangeBorderLine(short Xposition,CExcelBorder& XBorder);
		BOOL SetBackStyle(CExcelBackStyle BackStyle);

		
};
#endif