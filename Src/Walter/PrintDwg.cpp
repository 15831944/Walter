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
		//不能打印完全 需要找到最大与最小的点
		Plot(fileName);
	}
	else
	{
		//图框打印
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
		MessageBox(NULL, L"打印失败", L"提示", MB_OK);
	else
		MessageBox(NULL, L"打印完成", L"提示", MB_OK);
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
	//直接让用户选择要打印的区域
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
		//比较名字 
		if (HasThisName)
		{
			tuKuangIDs.push_back(vIds[i]);
		}
	}
	//添加图框
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

//打印框选
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
	//// 取得当前布局
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); //取得布局管理器对象
	//获取当前布局的时候要根据CAD的中英文
	AcDbLayout *pLayout = pLayoutManager->findLayoutNamed(L"Model");//获得当前布局
	AcDbObjectId layoutId = pLayout->objectId();//获得布局的Id
	//获得打印机验证器对象
	AcDbPlotSettingsValidator *pPSV = acdbHostApplicationServices()->plotSettingsValidator();

	AcPlPlotInfoValidator validator; //创建打印信息验证器
	validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);

	//vector<AcPlPlotInfo*> plotInfo;
	Acad::ErrorStatus es;

	AcDbPlotSettings plotSettings(true);
	pPSV->setPlotCfgName(&plotSettings, m_Plotdevice);//设置打印设备
	pPSV->setCanonicalMediaName(&plotSettings, m_papertype);//设置纸张类型
	pPSV->setCurrentStyleSheet(&plotSettings, m_PlotStyleSheet);
	pPSV->setPlotRotation(&plotSettings, AcDbPlotSettings::k0degrees);//设置打印方向
	pPSV->setUseStandardScale(&plotSettings, true);
	//pPSV->setPlotPaperUnits(&plotSettings, AcDbPlotSettings::kMillimeters); //设置单位
	AcPlPlotInfo* plotInfo = new AcPlPlotInfo;
	//plotInfo.push_back(new AcPlPlotInfo);
	plotInfo->setLayout(layoutId);
	plotInfo->setOverrideSettings(&plotSettings);
	validator.validate(*plotInfo);


	
	//打印机设置
	pPSV->setPlotWindowArea(&plotSettings,xmin,ymin,xmax,ymax);//设置打印范围,超出给范围的将打不出来
	pPSV->setPlotType(&plotSettings, AcDbPlotSettings::kWindow);//设置打印范围为窗口
	pPSV->setPlotCentered(&plotSettings, true);//是否居中打印
	pPSV->setUseStandardScale(&plotSettings, true);//设置是否采用标准比例
	pPSV->setStdScaleType(&plotSettings, AcDbPlotSettings::kScaleToFit);//布满图纸
	//打印机设置完成之后要重新验证否则会出现错误
	plotInfo->setOverrideSettings(&plotSettings);
	validator.validate(*plotInfo);
	////准备打印/////////////////////////////////////////////////////////////////////////
	
	//关闭后台打印，否则打印速度很慢
	pResbuf rb = acutBuildList(RTSHORT, 0, RTNONE);
	acedSetVar(L"BACKGROUNDPLOT", rb);
	acutRelRb(rb);


	AcPlPlotEngine* pEngine = NULL;//创建打印引擎
	if (AcPlPlotFactory::createPublishEngine(pEngine) == Acad::eOk)
	{
		es = pEngine->beginPlot(NULL);
		AcPlPlotPageInfo pageInfo;//打印页信息
		const ACHAR* fileName = NULL;
		acdbHostApplicationServices()->workingDatabase()->getFilename(fileName);
		es = pEngine->beginDocument(*plotInfo, fileName, NULL, 1, true, sPdfName);
		pEngine->beginPage(pageInfo, *plotInfo,true );
		pEngine->beginGenerateGraphics();
		pEngine->endGenerateGraphics();
		pEngine->endPage();
		pEngine->endDocument();
		pEngine->endPlot();
		//返回资源
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



	//识别不到图框时打印整个模型空间
	if (m_allRect.empty())
	{
		AcDbBlockTable *pBlkTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

		// 获得模型空间的块表记录
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

	// 取得当前布局
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); //取得布局管理器对象
	AcDbLayout *pLayout = pLayoutManager->findLayoutNamed(L"Model");//获得当前布局
	AcDbObjectId layoutId = pLayout->objectId();//获得布局的Id

												//获得打印机验证器对象
	AcDbPlotSettingsValidator *pPSV = acdbHostApplicationServices()->plotSettingsValidator();

	AcPlPlotInfoValidator validator; //创建打印信息验证器
	validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);

	vector<AcPlPlotInfo*> plotInfo;
	Acad::ErrorStatus es;

	AcDbPlotSettings plotSettings(true);
	pPSV->setPlotCfgName(&plotSettings, m_Plotdevice);//设置打印设备
	pPSV->setCanonicalMediaName(&plotSettings, m_papertype);//设置纸张类型
	pPSV->setCurrentStyleSheet(&plotSettings,m_PlotStyleSheet);
	pPSV->setPlotRotation(&plotSettings, AcDbPlotSettings::k0degrees);//设置打印方向

	plotInfo.push_back(new AcPlPlotInfo);
	plotInfo[0]->setLayout(layoutId);
	plotInfo[0]->setOverrideSettings(&plotSettings);
	validator.validate(*plotInfo[0]);

	for (UINT i = 0; i < m_allRect.size(); i++)
	{
		AcGePoint3d ptLB = m_allRect[i].GetLB();
		AcGePoint3d ptRT = m_allRect[i].GetRT();

		//打印机设置
		pPSV->setPlotWindowArea(&plotSettings, ptLB.x, ptLB.y, ptRT.x, ptRT.y);//设置打印范围,超出给范围的将打不出来
		pPSV->setPlotType(&plotSettings, AcDbPlotSettings::kWindow);//设置打印范围为窗口
		pPSV->setPlotCentered(&plotSettings, true);//是否居中打印
		pPSV->setUseStandardScale(&plotSettings, true);//设置是否采用标准比例
		pPSV->setStdScaleType(&plotSettings, AcDbPlotSettings::kScaleToFit);//布满图纸

		plotInfo.push_back(new AcPlPlotInfo);
		plotInfo[i + 1]->setLayout(layoutId);
		plotInfo[i + 1]->setOverrideSettings(&plotSettings);
		es = validator.validate(*plotInfo[i + 1]);
	}

	//准备打印/////////////////////////////////////////////////////////////////////////

	//关闭后台打印，否则打印速度很慢
	pResbuf rb = acutBuildList(RTSHORT, 0, RTNONE);
	acedSetVar(L"BACKGROUNDPLOT", rb);
	acutRelRb(rb);

	AcPlPlotEngine* pEngine = NULL;//创建打印引擎
	if (AcPlPlotFactory::createPublishEngine(pEngine) == Acad::eOk)
	{
		es = pEngine->beginPlot(NULL);

		AcPlPlotPageInfo pageInfo;//打印页信息

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

		//返回资源
		pEngine->destroy();
		acutPrintf(L"打印完成");
	}
	else
	{
		acutPrintf(L"打印失败");
	}

	for (UINT i = 0; i < plotInfo.size(); i++)
	{
		delete plotInfo[i];
	}
}