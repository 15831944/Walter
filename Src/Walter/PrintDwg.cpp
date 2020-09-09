#include "stdafx.h"
#include "PrintDwg.h"
#include "acplplotinfovalidator.h"
#include "AcPlPlotEngine.h"
#include "acplplotfactory.h"
#include "TYPlot.h"
#include "Com.h"

CPrintDwg::CPrintDwg()
{
}


CPrintDwg::~CPrintDwg()
{
}

void CPrintDwg::ExportToPdf()
{
		ExportToPdf(m_rect); //��ӡ��ѡ
}

void CPrintDwg::ExportToPdf(const Rect & rect )
{
	ExportToPdf(rect.LB.x, rect.LB.y, rect.RT.x, rect.RT.y);
}

void CPrintDwg::ExportToPdf(double xmin, double ymin, double xmax, double ymax)
{
	CString test = m_dirPath + L"\\test.pdf";
	//Plot(test,xmin,ymin,xmax,ymax);
	Plot(test);
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

//��ӡ��ѡ
void CPrintDwg::Plot(CString sPdfName, double xmin, double ymin, double xmax, double ymax)
{
	CDocLock lock;
	TYPlot::SetTyPlotConfigFolder(TY_GetFrameFolder());
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
	pPSV->setPlotPaperUnits(&plotSettings, AcDbPlotSettings::kMillimeters); //���õ�λ
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
//��ӡȫ��
void CPrintDwg::Plot(CString sPdfName)
{
	CDocLock lock;

	TYPlot::SetTyPlotConfigFolder(TY_GetFrameFolder()+ L"TuYuan");

	TYPlot::vSTYTitle titlesModel;
	TYPlot::TYPLOT_GetTitlesForModelSpace(titlesModel);

	////ʶ�𲻵�ͼ��ʱ��ӡ����ģ�Ϳռ�
	//if (titlesModel.empty())
	//{
	//	AcDbBlockTable *pBlkTbl = NULL;
	//	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

	//	// ���ģ�Ϳռ�Ŀ���¼
	//	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	//	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	//	pBlkTbl->close();

	//	AcDbExtents extent;
	//	Acad::ErrorStatus es = extent.addBlockExt(pBlkTblRcd);
	//	pBlkTblRcd->close();

	//	titlesModel.resize(1);
	//	titlesModel[0].m_lb = extent.minPoint();
	//	titlesModel[0].m_rt = extent.maxPoint();
	//}

	// ȡ�õ�ǰ����
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); //ȡ�ò��ֹ���������
	AcDbLayout *pLayout = pLayoutManager->findLayoutNamed(L"ģ��");//��õ�ǰ����
	AcDbObjectId layoutId = pLayout->objectId();//��ò��ֵ�Id

												//��ô�ӡ����֤������
	AcDbPlotSettingsValidator *pPSV = acdbHostApplicationServices()->plotSettingsValidator();

	AcPlPlotInfoValidator validator; //������ӡ��Ϣ��֤��
	validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);

	vector<AcPlPlotInfo*> plotInfo;
	Acad::ErrorStatus es;

	AcDbPlotSettings plotSettings(true);
	pPSV->setPlotCfgName(&plotSettings, L"DWG To PDF.pc3");//���ô�ӡ�豸
	pPSV->setCanonicalMediaName(&plotSettings, m_papertype);//����ֽ������
	pPSV->setCurrentStyleSheet(&plotSettings, L"monochrome.ctb");
	pPSV->setPlotRotation(&plotSettings, AcDbPlotSettings::k0degrees);//���ô�ӡ����
	pPSV->setPlotPaperUnits(&plotSettings, AcDbPlotSettings::kMillimeters); //���õ�λ

	plotInfo.push_back(new AcPlPlotInfo);
	plotInfo[0]->setLayout(layoutId);
	plotInfo[0]->setOverrideSettings(&plotSettings);
	validator.validate(*plotInfo[0]);

	for (UINT i = 0; i < titlesModel.size(); i++)
	{
		AcGePoint3d ptLB = titlesModel[i].m_lb;
		AcGePoint3d ptRT = titlesModel[i].m_rt;

		//��ӡ������
		pPSV->setPlotWindowArea(&plotSettings, ptLB.x, ptLB.y, ptRT.x, ptRT.y);//���ô�ӡ��Χ,��������Χ�Ľ��򲻳���
		pPSV->setPlotType(&plotSettings, AcDbPlotSettings::kWindow);//���ô�ӡ��ΧΪ����
		pPSV->setPlotCentered(&plotSettings, true);//�Ƿ���д�ӡ
		pPSV->setUseStandardScale(&plotSettings, true);//�����Ƿ���ñ�׼����
		pPSV->setStdScaleType(&plotSettings, AcDbPlotSettings::kScaleToFit);//����ͼֽ

		plotInfo.push_back(new AcPlPlotInfo);
		plotInfo[i + 1]->setLayout(layoutId);
		plotInfo[i + 1]->setOverrideSettings(&plotSettings);
		es = validator.validate(*plotInfo[i + 1]);
	}

	//׼����ӡ/////////////////////////////////////////////////////////////////////////

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
		es = pEngine->beginDocument(*plotInfo[0], fileName, NULL, 1, true, sPdfName);

		for (UINT i = 0; i < titlesModel.size(); i++)
		{
			bool bLast = (i == titlesModel.size() - 1);
			pEngine->beginPage(pageInfo, *plotInfo[i + 1], bLast);
			pEngine->beginGenerateGraphics();
			pEngine->endGenerateGraphics();
			pEngine->endPage();
		}

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

	for (UINT i = 0; i < plotInfo.size(); i++)
	{
		delete plotInfo[i];
	}
}