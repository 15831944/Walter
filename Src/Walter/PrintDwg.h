#pragma once
#include "dbplotsetval.h"
#include "Geometry/TYRect.h"
//纸张类型，A2,A3,A4
enum PAPER_TYPE
{
	P_A2 = 0,//420mm×594mm
	P_A3,    //297mm×420mm
	P_A4     //210mm×297mm
};

class CPrintDwg
{
public:
	CPrintDwg();
	~CPrintDwg();
	
	//参数设置
	void SetPlotDevice(const CString& deviceName) { m_Plotdevice = deviceName; }
	void SetPlotStyleSheet(const CString& styleSheet) { m_PlotStyleSheet = styleSheet; }
	void setDirPath(TCHAR dir[]) { m_dirPath.Format(L"%s",dir); }
	void SetPaperType(PAPER_TYPE paper_type);
	void SetIsSingle(BOOL IsSingle) { m_IsSingle = IsSingle; }

	void AddRect( const CTYRect& rect);
	void GetAllPrintRects(bool select, vCString tukuangBlockNames);
	//打印图纸
	void ExportToPdf();
	
//打印图纸的是实现，不暴露给外部使用，只给外部使用接口
public:
	int ExportToSinglePdf(CString fileName, const CTYRect & rect);
	int Plot(CString pdfName, double maxx, double maxy, double minx, double miny); //打印单张
	void Plot(CString fileName);
private:


	
	
	CString m_dirPath; // 文件夹路径
     //单张 m_dirPath + m_pdfFileName
	//如果是多张 
	//m_dirPath + m_pdfFileName + "_1"
	//m_dirPath + m_pdfFileName + "_2"
	CString m_pdfFileName;
	CString m_Plotdevice;//the device of print
	CString m_PlotStyleSheet; // the style sheet of print;

	CString m_papertype; //打印纸张类型 A2 A3 A4
	BOOL m_IsSingle; // 表示打印单页还是多页
	vCTYRect m_allRect;
};

