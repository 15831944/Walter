#pragma once
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
#include <io.h>

namespace Excel 
{
    class  CExcelUtil
    {
    private:
        CString out_file_name;
    public:
        CString GetOutFileName() { return out_file_name; };
        void SetOutFileName(CString new_file_name) 
        { if (new_file_name.GetLength()>0) out_file_name = new_file_name; }

    public:
        CApplication m_excelApp;
        CWorkbooks    m_excelBooks;
        CWorkbook    m_excelBook;
        CWorksheets     m_excelSheets;
        CWorksheet   m_excelSheet;
        CRange        m_excelRange;

    public:
        void CreateExcel();
		//有时候无法打开excel是因为excel com加载项的问题
		bool OpenExcel(CString fileName, bool readOnly = false);
        bool SaveExcel();
        void CloseExcel();
		void SetVisible(bool bvi);
        // <summary>
        // 取得特定的Worksheet
        //</summary>
        // <param name="SheetName">worksheet的名字</param>
        //<returns></returns>
        CWorksheet GetSheet(CString SheetName);

        /// <summary>
        /// 取得特定的Worksheet
        /// </summary>
        /// <param name="index">worksheet的索引，从1开始</param>
        /// <returns></returns>
        CWorksheet GetSheet(int index);

		int SetSheetName(int index, CString name);
		CString GetSheetName(int index);
		int DeleteSheet(int index);

        /// <summary>
        /// 添加特定的Worksheet
        /// </summary>
        /// <param name="SheetName">新添加的worksheet的名字</param>
        /// <returns>成功返回创建的worksheet，否则控制台打印相应的出错信息</returns>
        CWorksheet AddSheet(CString SheetName);

        /// <summary>
        /// 取得sheet的数量
        /// </summary>
        /// <returns>sheet的数量</returns>
        int GetSheetNum();

        //操作单元格/合并单元格
        /// <summary>
        /// 返回特定的单元格
        /// </summary>
        /// <param name="ColNum">列号,从1开始,不大于255</param>
        /// <param name="RowNum">行号,从1开始,不大于255</param>
        /// <returns></returns>
        CString GetCell(int ColNum, int RowNum);

        //取得一个单元格
        void SetRange(int ColNum, int RowNum);

        //取得一个单元格区域
        void SetRange(int StartColNum, int StartRowNum, int EndColNum, int EndRowNum);

        //合并一个单元格区域
        void Merge(int startRowIndex, int startColIndex, int endRowIndex, int endColIndex);

        //单元格赋值/单元格区域赋值
        void SetCellValue(CString value);
        void SetCellValue(int row, int col, CString value);
		void SetCellFormula(int row, int col, CString value);
        void SetCellStringFormat();
        void SetCellValue( int startRow,int startCol, int endRow, int endCol, CString value );
        CString GetCellValue( int row, int col );

		void SetCellActive(int row, int col);

        //设置单元格样式
        void SetBoldFontAndSize( int size );

        //设置报表表格为最适应宽度
        //设置报表表格为最适应宽度
        void SetAutoFitColumns( int startRow, int startCol, int endRow, int endCol );
        void SetCenterAlign();
        void SetBorder();


		//以下是我添加的201108 专门为MD 不准确的
		int SetActiveSheet(long iSheetName);
		int SetSheetCellValue(long iSheetName, long irow, long icol,VARIANT &vt);
		int GetSheetCellValue(long iSheetName, long irow, long icol,VARIANT &vt);
		int GetSheetCellValue(long irow, long icol,VARIANT &vt);
		BOOL SaveAs(char* pFileSavePath);
		BOOL IsFileExist(char* pFilename, BOOL bDir);

    };
}
