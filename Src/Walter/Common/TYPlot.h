#pragma once

using namespace std;

#include <dbcurve.h>
#include <vector>

//#define  SIMPLE_PLOT

typedef std::vector<AcDbCurve *> vAcDbCurvePointer;

namespace TYPlot
{
	void SetTyPlotConfigFolder(CString p_sFolder);

	//����ʶ�������һ�������
	typedef struct TYTitle
	{
		AcGePoint3d m_lb; //left bottom point
		AcGePoint3d m_rt;//right top point

		CString m_layout;//ģ�ͻ���ͼֽ�ռ�
		CString m_paperName;//�����ͼֽ�ռ������
	}STYTitle;

	typedef std::vector<STYTitle> vSTYTitle;

/*-----------------------------ʶ�𲿷�---------------------------*/
    //��ȡ��ǰ�򿪵�database��ģ�Ϳռ�����е�ͼ��
    int TYPLOT_GetTitlesForModelSpace
        (  
            vSTYTitle &titles, //���-ͼ�����Ϣ
            bool blockMethod = true, //����-�Ƿ�ʶ��ͼ���ڵĹؼ���
            bool keyWordMethod = true, //�Ƿ񵥶�ʶ��ؼ���
            bool bLearn = false, //�Ƿ����ѧϰ�������ݽ���ʶ��
            bool bDraw = false//�Ƿ����ѧϰ�������ݽ���ʶ��
        );//Model Space

#ifndef SIMPLE_PLOT
     //��ȡ��ǰ�򿪵�database��ͼֽ�ռ�����е�ͼ��
    int TYPLOT_GetTitlesForPaperSpace
        (
            vSTYTitle &titles, //���-ͼ�����Ϣ
            CString paperName = _T(""),  //���Ϊ��ʶ�����е�ͼֽ�ռ䣬����ǿ�ֻʶ�������ͼֽ�ռ�
            bool blockMethod = true, //����-�Ƿ�ʶ��ͼ���ڵĹؼ���
            bool keyWordMethod = true, //�Ƿ񵥶�ʶ��ؼ���
            bool bLearn = false, //�Ƿ����ѧϰ�������ݽ���ʶ��
            bool bDraw = false//�Ƿ����ѧϰ�������ݽ���ʶ��
         );


/*----------------------------��ӡ����---------------------------------------------*/
	//ͨ��pdffactory��ӡ����ӡͼ��
	int TYPLOT_PrinterWithPdfFactory
		(
		CString pdfFileName,//pdf�ļ�����
		vSTYTitle &modelTitles, //ģ�Ϳռ���Ҫ��ӡ��ͼ��
		vSTYTitle &paperTitles,//ͼֽ�ռ���Ҫ��ӡ��ͼ��
		CString paperStr = L"A4", //paperStr @A0 @A1 @A2 @A3 @A4 A0 A1 A2 A3 A4 �Զ���
		double height = 0,//���paperStr���Զ��� ������������� ��ʾ��ӡ�ĸ߶�
		double width = 0//���paperStr���Զ��� ��������������ʾ��ӡ�Ŀ��	
		);

	//ͨ��cad�Դ���ӡ����ӡͼ��
	void TYPLOT_PrinterWithCADPrinter
		(
			CString pdfFolder, //�ļ���·�� ��Ϊcad�Դ���ӡ����һ��һ�������pdf �����Ҫ���кϲ�
			CString pdfFileName,//pdf�ļ�·�����ƣ����·����
			vSTYTitle &modelTitles, //ģ�Ϳռ���Ҫ��ӡ��ͼ��
			vSTYTitle &paperTitles,//ͼֽ�ռ���Ҫ��ӡ��ͼ��
			int outputMode=0,//���ģʽ��1��ʾ���һ������pdf��2��ʾ������Ű�ͼ���ļ���3��ʾͬʱ��������pdf�Ͱ�ͼ���pdf
			CString paperStr = L"A4", //paperStr @A0 @A1 @A2 @A3 @A4 A0 A1 A2 A3 A4 �Զ���
			double height = 0,//���paperStr���Զ��� ������������� ��ʾ��ӡ�ĸ߶�
			double width = 0//���paperStr���Զ��� ��������������ʾ��ӡ�Ŀ��
		);

	void CurDocumentModelSpacePrinterToPdf
		(
			CString pdfFolder, //�ļ���·�� ��Ϊcad�Դ���ӡ����һ��һ�������pdf �����Ҫ���кϲ�
			CString pdfFileName,//pdf�ļ�·�����ƣ����·����
			int outputMode = 0,//���ģʽ��1��ʾ���һ������pdf��2��ʾ������Ű�ͼ���ļ���3��ʾͬʱ��������pdf�Ͱ�ͼ���pdf
			CString paperStr = L"A4", //paperStr @A0 @A1 @A2 @A3 @A4 A0 A1 A2 A3 A4 �Զ���
			double height = 0,//���paperStr���Զ��� ������������� ��ʾ��ӡ�ĸ߶�
			double width = 0//���paperStr���Զ��� ��������������ʾ��ӡ�Ŀ��
			);

	int TYPLOT_PrinterWithRealPrinter
	(
		CString realPrinterName,//ʵ���ӡ������
		vSTYTitle &modelTitles, //ģ�Ϳռ���Ҫ��ӡ��ͼ��
		vSTYTitle &paperTitles,//ͼֽ�ռ���Ҫ��ӡ��ͼ��
		CString paperStr = L"A4", //paperStr @A0 @A1 @A2 @A3 @A4 A0 A1 A2 A3 A4 �Զ���
		double height = 0,//���paperStr���Զ��� ������������� ��ʾ��ӡ�ĸ߶�
		double width = 0 //���paperStr���Զ��� ��������������ʾ��ӡ�Ŀ��	
	);

#endif

/*----------------------------�����������---------------------------------------------*/
	 //�ѵ�ǰͼֽ��һ�����󵼳���һ��������dwg�ļ�
	int TYPLOT_ExportDwg(AcDbObjectIdArray& outObjIds, CString filename);
	//�ѵ�ǰͼֽ��ʶ�������ĳ��ͼ����������� ����Ϊһ��dwgͼֽ
	int TYPLOT_ExportAllObjectsInTitle(STYTitle title, CString filename);

	//��ѧ����-����һ�����ӵ㣬�ҳ���������ӵ������һȦ�� ���������
	 //�÷�ʾ��
	/*AcGePoint3d seedOri = seedPoint;
	int findIndex = 0;//��0��ʼ������
	vAcDbCurveP loop;
	TYPlot::TYJGCOM_LoopCountCurve(cuves, seedOri, seedPoint, findIndex, loop);���ж�*/
	int TYJGCOM_LoopCountCurve
		(
		vAcDbCurvePointer &pCurves, //���еĴ�����������
		AcGePoint3d &seedOri, //���ӵ� һ����ĳ�����ߵ��������յ�
		AcGePoint3d seedPnt, //�м����ӵ� ʹ��ʱͬseedOri
		int &findIndex, //������� ��ʼ��Ϊ0
		vAcDbCurvePointer &loop//����Ľ��
		);    
}

