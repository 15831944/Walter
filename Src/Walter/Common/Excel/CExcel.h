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

// 	Excel�ӿ����в��������ʾ��
// 		-_Application
// 			-Workbooks ����������
// 				-_Workbook ������
// 					-Worksheets ��������
// 						-_Worksheet ������
// 							-Range ��Ԫ������

//Ϊ�����Ч��,�˴���const����,����define��.

/************************************************************************/
/*                                ����ͼ��������                        */
/************************************************************************/

//����ͼ��������
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
/*                                �߿����                              */
/************************************************************************/
//�߿�λ
const int xlDiagonalDown = 5;//������������ɫ����Ч.��֪Ϊ��.
const int xlDiagonalUp = 6;//
const int xlEdgeBottom = 9;
const int xlEdgeLeft = 7;
const int xlEdgeRight = 10;
const int xlEdgeTop = 8;
const int xlInsideHorizontal = 12;
const int xlInsideVertical = 11;
//�߿�������
const int xlContinuous = 1;
const int xlDash = -4115;
const int xlDashDot = 4;
const int xlDashDotDot = 5;
const int xlDot = -4118;
const int xlDouble = -4119;
const int xlLineStyleNone = -4142;
const int xlNone = -4142;
const int xlSlantDashDot = 13;
//�߿��ߵĴ�ϸ
const int xlHairline = 1;
const int xlMedium = -4138;
const int xlThick = 4;
const int xlThin  = 2;


/************************************************************************/
/*                                �������                              */
/************************************************************************/
//����ˮƽ���뷽ʽ
const int xlGeneral = 1;
const int  xlCenter = -4108;
const int  xlLeft = -4131;
const int  xlRight = -4152;
const int  xlFill = 5;
const int  xlJustify = -4130;
//���崹ֱ���뷽ʽ
const int  xlTop = -4160;
const int  xlBottom = -4107;
//�»��ߵ�����
const int xlUnderlineStyleDouble = -4119;
const int xlUnderlineStyleDoubleAccounting = 5;
const int xlUnderlineStyleNone = -4142;
const int xlUnderlineStyleSingle = 2;
const int xlUnderlineStyleSingleAccounting = 4;

//�������ò�����
class DLL_EXCEL_API CExcelBackStyle
{
public:	
	//���캯��
	CExcelBackStyle();
	//������ɫ
	long m_lColor;
	//����ͼ��
	short m_sPattern;
	//����ͼ����ɫ
	long m_lPatternColor;
	//͸����͸��
	BOOL m_bTransparent;

};


//�߿����ò�����
class DLL_EXCEL_API CExcelBorder
{
public:
	//���캯��
	CExcelBorder();
	CExcelBorder(long color, short linestyle, short weigth);
	//������״
	short m_sLineStyle;
	//����
	short m_sWeight;
	//��ɫ
	long m_lColor;

};
//�������ò�����
class DLL_EXCEL_API CExcellFont
{
public:
	//���캯��
	CExcellFont();
// 	CExcellFont(char* pFontName, long size,
// 		long foreColor, BOOL Bold, BOOL Italic,
// 		BOOL Strikethrough, BOOL Shadow,
// 		BOOL Subcript, BOOL Shadow, 
// 		short Underline);
// 	CExcellFont(char* pFontName, long size, long foreColor, BOOL Bold);
// 	CExcellFont(char* pFontName, long size, long foreColor);
// 	CExcellFont(char* pFontName, long size);
	//����
	CString m_strName;
	//��С
	long m_lSize;
	//ǰ��
	long m_lForeColor;
	//����
	BOOL m_bBold;
	//б��
	BOOL m_bItalic;
	//�м���
	BOOL m_bStrikethrough;
	//��Ӱ
	BOOL m_bShadow;
	//�±�
	BOOL m_bSubscript;
	//�ϱ�
	BOOL m_bSuperscript;
	//�»���
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
		BOOL OpenOfficeExcel2003();//��Office 2003. true, �ɹ�; false,ʧ��
		BOOL CloseOfficeExcel2003(char*pFileSavePath);//�ر�Office 2003�����ļ����浽ָ��·��. true,�ɹ�; false,ʧ��
		BOOL SaveAs(char* pFileSavePath);//���ļ����Ϊ,�����ر�Excel
		BOOL Close(BOOL bSaveChanges=TRUE);//�ر�Excel. bSaveChanges,�Ƿ񱣴���ĵ��ļ�
		void SetVisible(bool bvi);
		BOOL OpenWorkbook(char *pFileName);//��(һ��)������.true,�ɹ�;false,ʧ��
		void OpenExcel(CString fileName);

		BOOL AddWorkbook(char *pFilePath);//����һ���µĹ�����. true,�ɹ�;false,ʧ��
		BOOL AddSheet(char*pSheetName=NULL);//���һ��sheet.pSheetName=NULL,Ĭ������(sheet1).
		BOOL DeleteSheet(char*pSheetName);//ɾ��һ��sheet
		BOOL SetCurrentSheet(char *pSheetName);//���õ�ǰSheet. pSheetName,sheet ����
		CWorksheet GetCurrentSheet();//��õ�ǰ�Ĺ�����(sheet)
		BOOL SetCurrentRange(char*pBegin, char*pEnd);//���õ�ǰ��ѡ������(Range)
		CRange GetCurrentRange();//��õ�ǰ��ѡ������
		BOOL SetItemValue(int nRow, int nCol, char*pValue);//����ĳ��ĳ�е�ֵ
		BOOL SetItemValue(char*pSheetName, int nRow, int nCol, char*pValue);//��������ĳ��ĳ�е�ֵ
		CString GetItemValue(int nRow, int nCol);//���ĳ��ĳ�е�ֵ
		CString GetItemValue(char*pSheetName, int nRow, int nCol);//���ػ��ĳ��ĳ�е�ֵ
		BOOL SetColumnWidth(int nWidth);//�����п�
		BOOL SetColumnWidth(char*pSheetName, int nWidth);//���������п�
		BOOL SetRowHeight(int nHeight);//�����и�
		BOOL SetRowHeight(char*pSheetName, int nHeight);
		BOOL CopyRange(char*pCharCellNameBegin, char*pCharCellNameEnd, char*pCharCellNameDes);
		int GetColumnCount();//�������//�������������׳���(����Ҫ��֤m_rangeѡ��һ������),���ʱҲ�����õ�.
		int GetRowCount();//�������
		BOOL MergeRange(char*pBegin, char*pEnd);//�ϲ���Ԫ��
		BOOL ColumnAutoFit();//���Զ�չ��
		BOOL RowAutoFit();//���Զ�չ��	
		BOOL SetRangeHorAlignment(int nHorMode);//����ˮƽ���뷽ʽ
		BOOL SetRangeHorAlignment(char*pBegin, char* pEnd, int nHorMode);//����
		BOOL SetRangeVerAlignment(int nVerMode);//���ô�ֱ���뷽ʽ
		BOOL SetRangeVerAlignment(char*pBegin, char* pEnd, int nVerMode);//�������ô�ֱ���뷽ʽ
		BOOL IsFileExist(char* pFilename, BOOL bDir);			
		BOOL SetRangeFont(CExcellFont& cf);//����ѡ�����������
		BOOL SetRangeBorderLine(short Xposition,CExcelBorder& XBorder);
		BOOL SetBackStyle(CExcelBackStyle BackStyle);

		
};
#endif