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
		ExportToPdf(m_rect); //打印框选
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

//打印框选
void CPrintDwg::Plot(CString sPdfName, double xmin, double ymin, double xmax, double ymax)
{
	CDocLock lock;
	TYPlot::SetTyPlotConfigFolder(TY_GetFrameFolder());
	//// 取得当前布局
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); //取得布局管理器对象
	AcDbLayout *pLayout = pLayoutManager->findLayoutNamed(L"模型");//获得当前布局
	AcDbObjectId layoutId = pLayout->objectId();//获得布局的Id

	//获得打印机验证器对象
	AcDbPlotSettingsValidator *pPSV = acdbHostApplicationServices()->plotSettingsValidator();

	AcPlPlotInfoValidator validator; //创建打印信息验证器
	validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);

	//vector<AcPlPlotInfo*> plotInfo;
	Acad::ErrorStatus es;

	AcDbPlotSettings plotSettings(true);
	pPSV->setPlotCfgName(&plotSettings, L"DWG To PDF.pc3");//设置打印设备
	pPSV->setCanonicalMediaName(&plotSettings, m_papertype);//设置纸张类型
	pPSV->setCurrentStyleSheet(&plotSettings, L"monochrome.ctb");
	pPSV->setPlotRotation(&plotSettings, AcDbPlotSettings::k0degrees);//设置打印方向
	pPSV->setPlotPaperUnits(&plotSettings, AcDbPlotSettings::kMillimeters); //设置单位
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
		acutPrintf(L"打印完成");
	}
	else
	{
		acutPrintf(L"打印失败");
	}

}
//打印全部
void CPrintDwg::Plot(CString sPdfName)
{
	CDocLock lock;

	TYPlot::SetTyPlotConfigFolder(TY_GetFrameFolder()+ L"TuYuan");

	TYPlot::vSTYTitle titlesModel;
	TYPlot::TYPLOT_GetTitlesForModelSpace(titlesModel);

	////识别不到图框时打印整个模型空间
	//if (titlesModel.empty())
	//{
	//	AcDbBlockTable *pBlkTbl = NULL;
	//	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

	//	// 获得模型空间的块表记录
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

	// 取得当前布局
	AcDbLayoutManager *pLayoutManager = acdbHostApplicationServices()->layoutManager(); //取得布局管理器对象
	AcDbLayout *pLayout = pLayoutManager->findLayoutNamed(L"模型");//获得当前布局
	AcDbObjectId layoutId = pLayout->objectId();//获得布局的Id

												//获得打印机验证器对象
	AcDbPlotSettingsValidator *pPSV = acdbHostApplicationServices()->plotSettingsValidator();

	AcPlPlotInfoValidator validator; //创建打印信息验证器
	validator.setMediaMatchingPolicy(AcPlPlotInfoValidator::kMatchEnabled);

	vector<AcPlPlotInfo*> plotInfo;
	Acad::ErrorStatus es;

	AcDbPlotSettings plotSettings(true);
	pPSV->setPlotCfgName(&plotSettings, L"DWG To PDF.pc3");//设置打印设备
	pPSV->setCanonicalMediaName(&plotSettings, m_papertype);//设置纸张类型
	pPSV->setCurrentStyleSheet(&plotSettings, L"monochrome.ctb");
	pPSV->setPlotRotation(&plotSettings, AcDbPlotSettings::k0degrees);//设置打印方向
	pPSV->setPlotPaperUnits(&plotSettings, AcDbPlotSettings::kMillimeters); //设置单位

	plotInfo.push_back(new AcPlPlotInfo);
	plotInfo[0]->setLayout(layoutId);
	plotInfo[0]->setOverrideSettings(&plotSettings);
	validator.validate(*plotInfo[0]);

	for (UINT i = 0; i < titlesModel.size(); i++)
	{
		AcGePoint3d ptLB = titlesModel[i].m_lb;
		AcGePoint3d ptRT = titlesModel[i].m_rt;

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