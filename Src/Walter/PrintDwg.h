#pragma once
#include "dbplotsetval.h"
//ֽ�����ͣ�A2,A3,A4
enum PAPER_TYPE
{
	P_A2 = 0,//420mm��594mm
	P_A3,    //297mm��420mm
	P_A4     //210mm��297mm
};
//��ӡ��Χ
struct  Rect
{
	Rect() {}
	Rect(AcGePoint3d lb, AcGePoint3d rt) : RT(rt), LB(lb) {}
	~Rect() {}
	AcGePoint3d RT; //���ϵ�
	AcGePoint3d LB; //���µ�
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
	
	//��������
	void SetRect(const AcGePoint3d& minPoint, const AcGePoint3d& maxPoint);
	void SetRect(double xmin, double ymin, double xmax, double ymax);
	void setDirPath(TCHAR dir[]) { m_dirPath.Format(L"%s",dir); }
	void SetPaperType(PAPER_TYPE paper_type);
	void SetRect(const Rect& rect) { m_rect = rect; }
	void SetIsSingle(BOOL IsSingle) { m_IsSingle = IsSingle; }
	void SetIsPrintAll(BOOL isPrintAll) { m_IsPrintAll = isPrintAll; } 


	//��ӡͼֽ
	void ExportToPdf();
	void ExportToPdf(const Rect& rect);
	void ExportToPdf(double xmin, double ymin, double xmax, double ymax); //������pdf
	
//��ӡͼֽ����ʵ�֣�����¶���ⲿʹ�ã�ֻ���ⲿʹ�ýӿ�
private:
	void Plot(CString sPdfName); //������ӡ����
	void Plot(CString pdfName, double maxx, double maxy, double minx, double miny); //��ӡ����
private:
	BOOL m_IsPrintAll; //�Ƿ��ӡȫ��
	BOOL m_IsSingle; // ��ʾ��ӡ��ҳ���Ƕ�ҳ
	Rect m_rect;
	CString m_dirPath; // �ļ���·��
	AcDbPlotSettingsValidator* m_pSv;
	CString m_papertype; //��ӡֽ������ A2 A3 A4
};

