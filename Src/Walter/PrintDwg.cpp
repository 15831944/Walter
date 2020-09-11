#include "stdafx.h"
#include "PrintDwg.h"
#include "acplplotinfovalidator.h"
#include "AcPlPlotEngine.h"
#include "acplplotfactory.h"
#include "Com.h"

CPrintDwg::CPrintDwg()
{
	m_pdfFileName = L"\\walter";
}

CPrintDwg::~CPrintDwg()
{
}

void CPrintDwg::ExportToPdf()
{
	if (m_IsSingle) 
	{
		CString fileName = m_dirPath + m_pdfFileName +L".pdf";
		//���ܴ�ӡ��ȫ ��Ҫ�ҵ��������С�ĵ�
		double xmin = 0.0;
		double ymin = 0.0;
		double xmax = 0.0;
		double ymax = 0.0;
		for (auto rect : m_allRect)
		{
			if (xmin >= rect.Left()) xmin = rect.Left();
			if (ymin >= rect.Bottom()) xmin = rect.Bottom();
			if (xmax <= rect.Right()) xmax = rect.Right();
			if (ymax <= rect.Top()) ymax = rect.Top();
		}
		AcGePoint3d pt1(xmin, ymin, 0);
		AcGePoint3d pt2(xmax, ymax, 0);
		ExportToSinglePdf(fileName, CTYRect(pt1,pt2)); //��ӡ��ѡ
	}
	else
	{
		//ͼ���ӡ
		TY_Progress_Init();
		for (int i = 0;i < m_allRect.size() ;++i)
		{
			TY_SetProgress((int)m_allRect.size(), i + 1);
			CString temp;
			temp.Format(L"_%d.pdf", i+1);
		
			CString fileName = m_dirPath + m_pdfFileName + temp;
			ExportToSinglePdf(fileName,m_allRect[i]);
		}
		TY_Progress_Close();
	}
}

void CPrintDwg::ExportToSinglePdf(CString fileName, const CTYRect & rect)
{
	Plot(fileName, rect.Left(), rect.Bottom(), rect.Right(), rect.Top());
}

void CPrintDwg::SetPaperType(PAPER_TYPE paper_type)
{
	switch (paper_type)
	{
	case P_A2:
		m_papertype = L"ISO_full_bleed_A2_(594.00_x_420.00_MM)";
		break;
	case P_A3:
		m_papertype = L"ISO_full_bleed_A3_(297.00_x_420.00_MM)";
		break;
	case P_A4:
		m_papertype = L"ISO_full_bleed_A4_(297.00_x_210.00_MM)";
		break;
	default:
		break;
	}
}

void CPrintDwg::GetAllPrintRects(bool select, vCString tukuangBlockNames)
{
	CDocLock lock;
	//ֱ�����û�ѡ��Ҫ��ӡ������
	vAcDbObjectId vIds;
	if (select)
		CSelectUtil::SelectMany(vIds);
	else
		CBlockUtil::CycleAllBlockReferences(vIds);
	vAcDbObjectId tuKuangIDs;
	for (int i = 0;i < vIds.size();++i)
	{
		CString  blkName;
		CBlockUtil::GetBlockNameByBlockRef(vIds[i], blkName);
		bool HasThisName = false;
		for (int i = 0;i<tukuangBlockNames.size();++i)
		{
			if (tukuangBlockNames[i].CompareNoCase(blkName) == 0)
			{
				HasThisName = true;
				break;
			}
		}
		//�Ƚ����� 
		if (HasThisName)
		{
			tuKuangIDs.push_back(vIds[i]);
		}
	}
	//���ͼ��
	for (auto id : tuKuangIDs)
	{
		CTYRect rect(id);
		AddRect(rect);
	}
}


void CPrintDwg::AddRect(const CTYRect& rect)
{
	m_allRect.push_back(rect);
}

//��ӡ��ѡ
void CPrintDwg::Plot(CString sPdfName, double xmin, double ymin, double xmax, double ymax)
{
	CDocLock lock;
	//TYPlot::SetTyPlotConfigFolder(TY_GetFrameFolder());
	//// ȡ�õ�ǰ����
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); //ȡ�ò��ֹ���������
	AcDbLayout *pLayout = pLayoutManager->findLayoutNamed(L"ģ��");//��õ�ǰ����
	AcDbObjectId layoutId = pLayout->objectId();//��ò��ֵ�Id

	//��ô�ӡ����֤������
	AcDbPlotSettingsValidator *pPSV = acdbHostApplicationServices()->plotSettingsValidator();

	AcPlPlotInfoValidator validator; //������ӡ��Ϣ��֤��
	validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);

	//vector<AcPlPlotInfo*> plotInfo;
	Acad::ErrorStatus es;

	AcDbPlotSettings plotSettings(true);
	pPSV->setPlotCfgName(&plotSettings, L"DWG To PDF.pc3");//���ô�ӡ�豸
	pPSV->setCanonicalMediaName(&plotSettings, m_papertype);//����ֽ������
	pPSV->setCurrentStyleSheet(&plotSettings, L"monochrome.ctb");
	pPSV->setPlotRotation(&plotSettings, AcDbPlotSettings::k0degrees);//���ô�ӡ����
	//pPSV->setPlotPaperUnits(&plotSettings, AcDbPlotSettings::kMillimeters); //���õ�λ
	AcPlPlotInfo* plotInfo = new AcPlPlotInfo;
	//plotInfo.push_back(new AcPlPlotInfo);
	plotInfo->setLayout(layoutId);
	plotInfo->setOverrideSettings(&plotSettings);
	validator.validate(*plotInfo);



	//��ӡ������
	pPSV->setPlotWindowArea(&plotSettings,xmin,ymin,xmax,ymax);//���ô�ӡ��Χ,��������Χ�Ľ��򲻳���
	pPSV->setPlotType(&plotSettings, AcDbPlotSettings::kWindow);//���ô�ӡ��ΧΪ����
	pPSV->setPlotCentered(&plotSettings, true);//�Ƿ���д�ӡ
	pPSV->setUseStandardScale(&plotSettings, true);//�����Ƿ���ñ�׼����
	pPSV->setStdScaleType(&plotSettings, AcDbPlotSettings::kScaleToFit);//����ͼֽ
	//��ӡ���������֮��Ҫ������֤�������ִ���
	plotInfo->setOverrideSettings(&plotSettings);
	validator.validate(*plotInfo);

	////׼����ӡ/////////////////////////////////////////////////////////////////////////
	
	//�رպ�̨��ӡ�������ӡ�ٶȺ���
	pResbuf rb = acutBuildList(RTSHORT, 0, RTNONE);
	acedSetVar(L"BACKGROUNDPLOT", rb);
	acutRelRb(rb);

	AcPlPlotEngine* pEngine = NULL;//������ӡ����
	if (AcPlPlotFactory::createPublishEngine(pEngine) == Acad::eOk)
	{
		es = pEngine->beginPlot(NULL);
		AcPlPlotPageInfo pageInfo;//��ӡҳ��Ϣ
		const ACHAR* fileName = NULL;
		acdbHostApplicationServices()->workingDatabase()->getFilename(fileName);
		es = pEngine->beginDocument(*plotInfo, fileName, NULL, 1, true, sPdfName);
		pEngine->beginPage(pageInfo, *plotInfo,true );
		pEngine->beginGenerateGraphics();
		pEngine->endGenerateGraphics();
		pEngine->endPage();
		pEngine->endDocument();
		pEngine->endPlot();
		//������Դ
		pEngine->destroy();
		acutPrintf(L"��ӡ���");
	}
	else
	{
		acutPrintf(L"��ӡʧ��");
	}

}