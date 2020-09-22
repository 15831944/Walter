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
	int ret = 1;
	if (m_IsSingle) 
	{
		CString fileName = m_dirPath + m_pdfFileName +L".pdf";
		//���ܴ�ӡ��ȫ ��Ҫ�ҵ��������С�ĵ�
		Plot(fileName);
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
			ret = ExportToSinglePdf(fileName,m_allRect[i]);
			if (ret < 0) {
				continue;
			}
		}
		TY_Progress_Close();
	}
	if(ret < 0)
		MessageBox(NULL, L"��ӡʧ��", L"��ʾ", MB_OK);
	else
		MessageBox(NULL, L"��ӡ���", L"��ʾ", MB_OK);
}

int CPrintDwg::ExportToSinglePdf(CString fileName, const CTYRect & rect)
{
	 return  Plot(fileName, rect.Left(), rect.Bottom(), rect.Right(), rect.Top());
}

void CPrintDwg::SetPaperType(PAPER_TYPE paper_type)
{
	switch (paper_type)
	{
	case P_A2:
		m_papertype = L"ISO_full_bleed_A2_(594.00_x_420.00_MM)";
		break;
	case P_A3:
		m_papertype = L"ISO_full_bleed_A3_(420.00_x_297.00_MM)";
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
		CDocLock lock;
		CString  blkName;
		CBlockUtil::GetBlockNameByBlockRef(vIds[i], blkName);
		bool HasThisName = false;
		for (int i = 0;i<tukuangBlockNames.size();++i)
		{
			if (tukuangBlockNames[i].Compare(blkName) == 0)
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
int CPrintDwg::Plot(CString sPdfName, double xmin, double ymin, double xmax, double ymax)
{
	/*
	* @return    : sucess is 0 failure is 1
	* @sPdfName  : the fileName of print
	* @xmin	     : the x of left bottom in rectangle
	* @ymin      : the y of left bottom in rectangle
	* @xmax	     : the x of right top in rectangle
	* @ymax	     : the y of right top in rectangle
	*/
	CDocLock lock;
	//// ȡ�õ�ǰ����
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); //ȡ�ò��ֹ���������
	//��ȡ��ǰ���ֵ�ʱ��Ҫ����CAD����Ӣ��
	AcDbLayout *pLayout = pLayoutManager->findLayoutNamed(L"Model");//��õ�ǰ����
	AcDbObjectId layoutId = pLayout->objectId();//��ò��ֵ�Id
	//��ô�ӡ����֤������
	AcDbPlotSettingsValidator *pPSV = acdbHostApplicationServices()->plotSettingsValidator();

	AcPlPlotInfoValidator validator; //������ӡ��Ϣ��֤��
	validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);

	//vector<AcPlPlotInfo*> plotInfo;
	Acad::ErrorStatus es;

	AcDbPlotSettings plotSettings(true);
	pPSV->setPlotCfgName(&plotSettings, m_Plotdevice);//���ô�ӡ�豸
	pPSV->setCanonicalMediaName(&plotSettings, m_papertype);//����ֽ������
	pPSV->setCurrentStyleSheet(&plotSettings, m_PlotStyleSheet);
	pPSV->setPlotRotation(&plotSettings, AcDbPlotSettings::k0degrees);//���ô�ӡ����
	pPSV->setUseStandardScale(&plotSettings, true);
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
		return 0;
	}
	else
	{
		return -1;
	}

}
void CPrintDwg::Plot(CString sPdfName)
{
	CDocLock lock;



	//ʶ�𲻵�ͼ��ʱ��ӡ����ģ�Ϳռ�
	if (m_allRect.empty())
	{
		AcDbBlockTable *pBlkTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

		// ���ģ�Ϳռ�Ŀ���¼
		AcDbBlockTableRecord *pBlkTblRcd = NULL;
		pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
		pBlkTbl->close();

		AcDbExtents extent;
		Acad::ErrorStatus es = extent.addBlockExt(pBlkTblRcd);
		pBlkTblRcd->close();

		m_allRect.resize(1);
		m_allRect[0].SetLB(extent.minPoint());
		m_allRect[0].SetRT(extent.maxPoint());
	}

	// ȡ�õ�ǰ����
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); //ȡ�ò��ֹ���������
	AcDbLayout *pLayout = pLayoutManager->findLayoutNamed(L"Model");//��õ�ǰ����
	AcDbObjectId layoutId = pLayout->objectId();//��ò��ֵ�Id

												//��ô�ӡ����֤������
	AcDbPlotSettingsValidator *pPSV = acdbHostApplicationServices()->plotSettingsValidator();

	AcPlPlotInfoValidator validator; //������ӡ��Ϣ��֤��
	validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);

	vector<AcPlPlotInfo*> plotInfo;
	Acad::ErrorStatus es;

	AcDbPlotSettings plotSettings(true);
	pPSV->setPlotCfgName(&plotSettings, m_Plotdevice);//���ô�ӡ�豸
	pPSV->setCanonicalMediaName(&plotSettings, m_papertype);//����ֽ������
	pPSV->setCurrentStyleSheet(&plotSettings,m_PlotStyleSheet);
	pPSV->setPlotRotation(&plotSettings, AcDbPlotSettings::k0degrees);//���ô�ӡ����

	plotInfo.push_back(new AcPlPlotInfo);
	plotInfo[0]->setLayout(layoutId);
	plotInfo[0]->setOverrideSettings(&plotSettings);
	validator.validate(*plotInfo[0]);

	for (UINT i = 0; i < m_allRect.size(); i++)
	{
		AcGePoint3d ptLB = m_allRect[i].GetLB();
		AcGePoint3d ptRT = m_allRect[i].GetRT();

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

		for (UINT i = 0; i < m_allRect.size(); i++)
		{
			bool bLast = (i == m_allRect.size() - 1);
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