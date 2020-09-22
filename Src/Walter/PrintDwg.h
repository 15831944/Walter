#pragma once
#include "dbplotsetval.h"
#include "Geometry/TYRect.h"
//ֽ�����ͣ�A2,A3,A4
enum PAPER_TYPE
{
	P_A2 = 0,//420mm��594mm
	P_A3,    //297mm��420mm
	P_A4     //210mm��297mm
};

class CPrintDwg
{
public:
	CPrintDwg();
	~CPrintDwg();
	
	//��������
	void SetPlotDevice(const CString& deviceName) { m_Plotdevice = deviceName; }
	void SetPlotStyleSheet(const CString& styleSheet) { m_PlotStyleSheet = styleSheet; }
	void setDirPath(TCHAR dir[]) { m_dirPath.Format(L"%s",dir); }
	void SetPaperType(PAPER_TYPE paper_type);
	void SetIsSingle(BOOL IsSingle) { m_IsSingle = IsSingle; }

	void AddRect( const CTYRect& rect);
	void GetAllPrintRects(bool select, vCString tukuangBlockNames);
	//��ӡͼֽ
	void ExportToPdf();
	
//��ӡͼֽ����ʵ�֣�����¶���ⲿʹ�ã�ֻ���ⲿʹ�ýӿ�
public:
	int ExportToSinglePdf(CString fileName, const CTYRect & rect);
	int Plot(CString pdfName, double maxx, double maxy, double minx, double miny); //��ӡ����
	void Plot(CString fileName);
private:


	
	
	CString m_dirPath; // �ļ���·��
     //���� m_dirPath + m_pdfFileName
	//����Ƕ��� 
	//m_dirPath + m_pdfFileName + "_1"
	//m_dirPath + m_pdfFileName + "_2"
	CString m_pdfFileName;
	CString m_Plotdevice;//the device of print
	CString m_PlotStyleSheet; // the style sheet of print;

	CString m_papertype; //��ӡֽ������ A2 A3 A4
	BOOL m_IsSingle; // ��ʾ��ӡ��ҳ���Ƕ�ҳ
	vCTYRect m_allRect;
};

