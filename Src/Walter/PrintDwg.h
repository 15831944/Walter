#pragma once
#include "dbplotsetval.h"
//纸张类型，A2,A3,A4
enum PAPER_TYPE
{
	P_A2 = 0,//420mm×594mm
	P_A3,    //297mm×420mm
	P_A4     //210mm×297mm
};
//打印范围
struct  Rect
{
	Rect() {}
	Rect(AcGePoint3d lb, AcGePoint3d rt) : RT(rt), LB(lb) {}
	~Rect() {}
	AcGePoint3d RT; //右上点
	AcGePoint3d LB; //左下点
	Rect& operator=(const Rect& r) {
		this->LB = r.LB;
		this->RT = r.RT;
		return (*this);
	}
};
class CPrintDwg
{
public:
	CPrintDwg();
	~CPrintDwg();
	
	//参数设置
	void SetRect(const AcGePoint3d& minPoint, const AcGePoint3d& maxPoint);
	void SetRect(double xmin, double ymin, double xmax, double ymax);
	void setDirPath(TCHAR dir[]) { m_dirPath.Format(L"%s",dir); }
	void SetPaperType(PAPER_TYPE paper_type);
	void SetRect(const Rect& rect) { m_rect = rect; }
	void SetIsSingle(BOOL IsSingle) { m_IsSingle = IsSingle; }
	void SetIsPrintAll(BOOL isPrintAll) { m_IsPrintAll = isPrintAll; } 


	//打印图纸
	void ExportToPdf();
	void ExportToPdf(const Rect& rect);
	void ExportToPdf(double xmin, double ymin, double xmax, double ymax); //导出到pdf
	
//打印图纸的是实现，不暴露给外部使用，只给外部使用接口
private:
	void Plot(CString sPdfName); //连续打印多张
	void Plot(CString pdfName, double maxx, double maxy, double minx, double miny); //打印单张
private:
	BOOL m_IsPrintAll; //是否打印全部
	BOOL m_IsSingle; // 表示打印单页还是多页
	Rect m_rect;
	CString m_dirPath; // 文件夹路径
	AcDbPlotSettingsValidator* m_pSv;
	CString m_papertype; //打印纸张类型 A2 A3 A4
};

