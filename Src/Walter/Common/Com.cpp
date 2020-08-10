#include "stdafx.h"
#include "Com.h"
#include "Entity/DynamicBlockUtil.h"

static HINSTANCE s_gTyToolInst = 0;

CString TY_GetAppPath()
{
	WCHAR lpFileName[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), lpFileName, MAX_PATH);
	CString strFileName = lpFileName;
	int nIndex = strFileName.ReverseFind('\\');
	CString strPath;
	if (nIndex > 0)
	{
		strPath = strFileName.Left(nIndex);
	}
	else
	{
		strPath = "";
	}
	return strPath;
}


//刀具库存储目录
CString TY_GetDwgFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath() + "\\Support\\Walter\\DaoJuXingHao\\";
#else
	//return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\dwg\\";
	return L"G:\\Departments\\TTB\\dwg\\";
#endif // DEV_TEST_ENV
}


//得到图框存储路径
CString TY_GetFrameFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath() + "\\Support\\Walter\\TuKuang\\";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\frame1\\";
#endif // DEV_TEST_ENV
}

CString TY_GetDllFilePathName()
{
#ifdef DEV_TEST_ENV
	return L"C:\\TYTool.dll";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\frame1\\TYTool.dll";
#endif // DEV_TEST_ENV
}
CString TY_GetHLRFilePathName()
{
#ifdef DEV_TEST_ENV
#ifdef ARX_2014
	return L"C:\\Program Files\\Autodesk\\AutoCAD 2014\\AsdkHlrApi19.dbx";
#elif ARX_2017
	return L"C:\\Program Files\\Autodesk\\AutoCAD 2017\\AsdkHlrApi21.dbx";
#endif
#else
#ifdef ARX_2014
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\frame1\\AsdkHlrApi19.dbx";
#elif ARX_2017
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\frame1\\AsdkHlrApi21.dbx";
#endif
#endif // DEV_TEST_ENV
}


//刀具库存储目录
CString TY_GetDynamicBlockFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath() + "\\Support\\Walter\\DynamicBlocks\\";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\DynamicBlocks\\";
#endif // DEV_TEST_ENV
}

//刀具库存储目录
CString TY_GetDaoBingFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath() + "\\Support\\Walter\\DaoBingForPCDJD\\";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\DaoBingForPCDJD\\";
#endif // DEV_TEST_ENV
}

//得到钻头和直槽刀的刀柄路径
CString TY_GetDaoBingSFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath() + "\\Support\\Walter\\DaoBingForZuanTou\\";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\DaoBingForZuanTou\\";
#endif
}
//整硬刀具的刀柄
CString TY_GetDaoBingZyFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath() + "\\Support\\Walter\\DaoBingForZYDJ\\";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\DaoBingForZYDJ\\";
#endif
}

//获取文件夹先的所有Dwg文件名称
vector<CString> GetAllDwgFile(const CString& dirPath)
{
	vector<CString> fileLists;
	CFileFind tempfind;
	BOOL isfind;
	//找寻所有dwg文件
	isfind = tempfind.FindFile(dirPath + "*.dwg");
	while (isfind)
	{
		isfind = tempfind.FindNextFile();
		//如果是文件夹就跳过，只查找本文件目录下的dwg文件
		if (tempfind.IsDots() || tempfind.IsDirectory())
			continue;
		else
		{
			CString fileName = tempfind.GetFileName();
			int pos = fileName.ReverseFind('.');
			fileLists.push_back(fileName.Left(pos));
		}
	}
	tempfind.Close();
	return fileLists;
}
//去掉小数点后无效的0
CString removeLastZero(double num)
{
	CString temp;
	temp.Format(L"%f", num);
	int pos = temp.ReverseFind('.');
	int dotpos = pos;
	//最多包含两位小数
	for (int i = 0; i <= N;i++)
	{
		if (temp[i + dotpos] != '0' ) {
			pos++;
		}
	}
	CString result = temp.Left(pos);
	//整数去掉小数点
	if (result[result.GetLength() - 1] == '.') {
		result = temp.Left(pos - 1);
	}
	return result;
}

//根据刀柄名称获取刀柄长度
double GetHandleLengthFromDaoBing(CString daoBingName)
{
	double length = 36.0; //默认36.0
	if (daoBingName.Compare(L"W06") == 0)			length = 36.0;		//BT30 22
	else if (daoBingName.Compare(L"W08") == 0)	length = 36.0;		//BT40 27
	else if (daoBingName.Compare(L"W10") == 0)	length = 40.0;		//BT50 38
	else if (daoBingName.Compare(L"W12") == 0)	length = 46.0;		//HSK-A40 20
	else if (daoBingName.Compare(L"W16") == 0)	length = 49.0;	//HSK-A50 25.95
	else if (daoBingName.Compare(L"W18") == 0)	length = 48.0;	//HSK-A63 25.95
	else if (daoBingName.Compare(L"W20") == 0)	length = 51.0;	//HSK-A80 25.95
	else if (daoBingName.Compare(L"W25") == 0)	length = 57.0;	//HSK-A100 28.95
	else if (daoBingName.Compare(L"W32") == 0)	length = 61.0;	//SK30 19.05
	else if (daoBingName.Compare(L"WN06") == 0)	length = 36.0;	//SK40 19.05
	else if (daoBingName.Compare(L"WN08") == 0)	length = 36.0;	//SK50 19.05
	else if (daoBingName.Compare(L"WN10") == 0)		length =40.0;		//W16 0
	else if (daoBingName.Compare(L"WN12") == 0)		length = 45.0;		//W20 0
	else if (daoBingName.Compare(L"WN14") == 0)		length = 45.0;		//W25 0
	else if (daoBingName.Compare(L"WN16") == 0)		length = 49.0;		//W32 0
	else if (daoBingName.Compare(L"WN18") == 0)		length = 48.0;		//W40 0
	else if (daoBingName.Compare(L"WN20") == 0)		length = 51.0;		//Z16 0
	else if (daoBingName.Compare(L"WN25") == 0)		length = 57.0;		//Z20 0
	else if (daoBingName.Compare(L"WN32") == 0)		length = 61.0;		//Z25 0
	else if (daoBingName.Compare(L"Z03") == 0)		length = 28.0;		//Z32 0
	else if (daoBingName.Compare(L"Z06") == 0)		length = 36.0;		//Z40 0
	else if (daoBingName.Compare(L"Z08") == 0)		length = 36.0;		//W20 0
	else if (daoBingName.Compare(L"Z10") == 0)		length = 40.0;		//W25 0
	else if (daoBingName.Compare(L"Z12") == 0)		length = 45.0;		//W32 0
	else if (daoBingName.Compare(L"Z14") == 0)		length = 45.0;		//W40 0
	else if (daoBingName.Compare(L"Z16") == 0)		length = 49.0;		//Z16 0
	else if (daoBingName.Compare(L"Z18") == 0)		length = 48.0;		//Z20 0
	else if (daoBingName.Compare(L"Z20") == 0)		length = 51.0;		//Z25 0
	else if (daoBingName.Compare(L"Z25") == 0)		length = 57.0;		//Z32 0
	else if (daoBingName.Compare(L"Z32") == 0)		length = 61.0;		//Z40 0
	return length;
}
//根据刀柄的名称获取一个距离
double GetDisByDBName(CString name)
{
	double dis = 22.0;
	if (name.Compare(L"BT30") == 0)			dis = 22.0;		//BT30 22
	else if (name.Compare(L"BT40") == 0)	dis = 27.0;		//BT40 27
	else if (name.Compare(L"BT50") == 0)	dis = 38.0;		//BT50 38
	else if (name.Compare(L"HSK-A40") == 0)	dis = 20.0 + 15.0;		//HSK-A40 20
	else if (name.Compare(L"HSK-A50") == 0)	dis = 25.95 + 16.05;	//HSK-A50 25.95
	else if (name.Compare(L"HSK-A63") == 0)	dis = 25.95 + 16.05;	//HSK-A63 25.95
	else if (name.Compare(L"HSK-A80") == 0)	dis = 25.95 + 16.05;	//HSK-A80 25.95
	else if (name.Compare(L"HSK-A100") == 0)dis = 28.95 + 16.05;	//HSK-A100 28.95
	else if (name.Compare(L"SK30") == 0)	dis = 19.05;	//SK30 19.05
	else if (name.Compare(L"SK40") == 0)	dis = 19.05;	//SK40 19.05
	else if (name.Compare(L"SK50") == 0)	dis = 19.05;	//SK50 19.05
	else if (name.Compare(L"W16") == 0)		dis = 0.0;		//W16 0
	else if (name.Compare(L"W20") == 0)		dis = 0.0;		//W20 0
	else if (name.Compare(L"W25") == 0)		dis = 0.0;		//W25 0
	else if (name.Compare(L"W32") == 0)		dis = 0.0;		//W32 0
	else if (name.Compare(L"W40") == 0)		dis = 0.0;		//W40 0
	else if (name.Compare(L"Z16") == 0)		dis = 0.0;		//Z16 0
	else if (name.Compare(L"Z20") == 0)		dis = 0.0;		//Z20 0
	else if (name.Compare(L"Z25") == 0)		dis = 0.0;		//Z25 0
	else if (name.Compare(L"Z32") == 0)		dis = 0.0;		//Z32 0
	else if (name.Compare(L"Z40") == 0)		dis = 0.0;		//Z40 0
	return dis;
}

//根据刀尖获取 lf2的值
double GetLf2ByDiameter(double diameter)
{
	double Lf2 = 2.5f; //设置默认为2.5
	if (diameter > 4 && diameter <= 6.1)
	{
		Lf2 = 2.5f;
	}
	else if (diameter > 6.1 && diameter <= 8.6)
	{
		Lf2 = 3.0f;
	}
	else if (diameter > 8.6 && diameter <= 10.6)
	{
		Lf2 = 3.5f;
	}
	else if (diameter > 10.6 && diameter <= 12.1)
	{
		Lf2 = 4.5f;
	}
	else if (diameter > 12.1 && diameter <= 15.1)
	{
		Lf2 = 5.0f;
	}
	else if (diameter > 15.1 && diameter <= 17.1)
	{
		Lf2 = 6.0f;
	}
	else if (diameter > 17.1)
	{
		Lf2 = 6.5f;
	}
	return Lf2;
}
//根据刀尖直径获取刀片的宽
double GetHeightByDiameter(double diameter)
{
	double Height = 2.0f; //设置默认为2.5
	if (diameter > 4 && diameter <= 6.1)
	{
		Height = 2.0f;
	}
	else if (diameter > 6.1 && diameter <= 8.6)
	{
		Height = 2.5f;
	}
	else if (diameter > 8.6 && diameter <= 10.6)
	{
		Height = 3.0f;
	}
	else if (diameter > 10.6 && diameter <= 12.1)
	{
		Height = 3.5f;
	}
	else if (diameter > 12.1 && diameter <= 15.1)
	{
		Height = 4.0f;
	}
	else if (diameter > 15.1 && diameter <= 17.1)
	{
		Height = 4.5f;
	}
	else if (diameter > 17.1)
	{
		Height = 5.0f;
	}
	return Height;
}
//根据刀尖直径获取刀片的半径
double GetRadiusByDiameter(double diameter)
{
	double radius = 3.2f; //设置默认为2.5
	if (diameter > 4 && diameter <= 12.1)
	{
		radius = 3.2f;
	}
	else if (diameter >12.1)
	{
		radius = 4.2f;
	}
	return radius;
}

bool TY_LoadTyTool()
{
	CString tytoolDll = TY_GetDllFilePathName();
	//dll资源加载  
	s_gTyToolInst = LoadLibrary(tytoolDll);
	if (s_gTyToolInst == NULL)
		return false;
	return true;
}

bool TY_FreeTyTool()
{
	if (s_gTyToolInst != NULL)
	{
		FreeLibrary(s_gTyToolInst);
	}
	return 0;
}

typedef void(_stdcall *pfnTYTOOL_Progress_Init)();
typedef void(_stdcall *pfnTYTOOL_Progress_setinfo)(int all, int cur);
typedef void(_stdcall *pfnTYTOOL_Progress_Close)();

void TY_Progress_Init()
{
	long lStatus = 0;
	pfnTYTOOL_Progress_Init Func = (pfnTYTOOL_Progress_Init)GetProcAddress(s_gTyToolInst, "?TYTOOL_Progress_Init@@YAXXZ");
	if (Func)
	{
		Func();
	}
}

int TY_SetProgress(int all, int cur)
{
	long lStatus = 0;
	pfnTYTOOL_Progress_setinfo Func = (pfnTYTOOL_Progress_setinfo)GetProcAddress(s_gTyToolInst, "?TYTOOL_Progress_setinfo@@YAHHH@Z");
	if (Func)
	{
		Func(all, cur);
	}
	return 0;
}

void TY_Progress_Close()
{
	long lStatus = 0;
	pfnTYTOOL_Progress_Close Func = (pfnTYTOOL_Progress_Close)GetProcAddress(s_gTyToolInst, "?TYTOOL_Progress_Close@@YAXXZ");
	if (Func)
	{
		Func();
	}
}

int JHCOM_ExtrudeAlongPath2(AcDbObjectId regionid, AcDbObjectId path, AcDbObjectId &id)
{
	AcDbEntity* pEnt = 0;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, path, AcDb::kForWrite);
	AcDbCurve *pHelix = dynamic_cast<AcDbCurve *>(pEnt);



	AcDbEntity* pEnt2 = 0;
	es = acdbOpenAcDbEntity(pEnt2, regionid, AcDb::kForWrite);
	AcDbRegion *pRegion = dynamic_cast<AcDbRegion *>(pEnt2);



	/*AcDbEntity* pEntRegion = 0;
	acdbOpenAcDbEntity(pEntRegion, secRegion, AcDb::kForRead);
	AcDbRegion *pRegion = dynamic_cast<AcDbRegion *>(pEntRegion);
	pEntRegion->close();*/


	es = Acad::eOk;
	if (pHelix != NULL && pRegion != NULL)
	{
		AcDb3dSolid * pSolid = new AcDb3dSolid();
		//es = pSolid->extrudeAlongPath(pRegion,pCurve,0); //GetExAngle()*acos(0.0)/90
		AcDbSweepOptions options;
		es = pSolid->createSweptSolid(pRegion, pHelix, options);
		if (es == Acad::eOk)
		{
			id = CDwgDatabaseUtil::PostToModelSpace(pSolid);
			pSolid->close();
		}

		pEnt->close();
		pEnt2->close();
	}
	return 0;
}



/*----------------------投影开始------------------*/
#include <Hlr.h>
#include <dbxutil.h>
#include <geassign.h>
#include <core_rxmfcapi.h>

class HLRInit
{
public:
	HLRInit()
	{
	
		//----- We need the HLR engine loaded
		if (!acrxServiceIsRegistered(AsdkHlrApiServiceName))
		{ 
#ifdef ARX_2014
			acrxLoadModule(TY_GetHLRFilePathName(), false, true);
#elif ARX_2017
			acrxLoadModule(TY_GetHLRFilePathName(), false, true);
#endif
		}
	}
};
static HLRInit instance;


void progress1(int percent_completed) {
	//acedSetStatusBarProgressMeterPos(percent_completed);
}

void progress2(int percent_completed) {
	//acedSetStatusBarProgressMeterPos(100 + percent_completed);
}

void progress3(int percent_completed) {
	//acedSetStatusBarProgressMeterPos(200 + percent_completed);
}

void TY_Project3DSolidTo2D(AcDbObjectId solid3d,
	bool hidLines,
	bool  internalVisibility,
	bool calculateSilhouettes,
	bool preUnit)
{
	//----- Select the entities

	//----- Append entity IDs to the collector
	AsdkHlrCollector collector;
	collector.setDeleteState(true);
	collector.addEntity(solid3d);

	//----- Get current viewport settings
	struct resbuf rb;
	acedGetVar(ACRX_T(/*NOXLATE*/"viewdir"), &rb);
	ads_point dirPt;
	acdbUcs2Wcs(rb.resval.rpoint, dirPt, Adesk::kTrue);
	acedGetVar(ACRX_T(/*NOXLATE*/"target"), &rb);
	ads_point tarPt;
	acdbUcs2Wcs(rb.resval.rpoint, tarPt, Adesk::kFalse);


	//----- Process hidden line removal
	//注意这里的参数非常重要，也是控制投影的关键参数
	//kProject 20200626控制是否投影成平面图 否则会是一个带Z值的图
	AsdkHlrEngine hlr(
		asPnt3d(tarPt), asVec3d(dirPt),
		kEntity | kBlock | kSubentity | kProgress | kProject
		| (internalVisibility ? kHonorInternals : 0)
		| (hidLines ? kShowAll : 0)
		| (calculateSilhouettes ? kMeshSilhouettes : 0)
		| (preUnit ? kUnite : 0)
	);
	hlr.setAcisConversionProgressCallBack(progress1);
	hlr.setAhlProgressCallBack(progress2);
	hlr.setAcadConversionProgressCallBack(progress3);

	acedSetStatusBarProgressMeter(ACRX_T("HLR running: "), 0, 300);
	hlr.run(collector);
	acedRestoreStatusBar();

	int n = 0;
	//----- Assign color to the resulting entities
	//----- red for visible edges
	//----- blue for non-visible edges
	//----- yellow for internal edges
	n = collector.mOutputData.logicalLength();
	for (int i = 0; i < n; i++)
	{
		AsdkHlrData *p = collector.mOutputData[i];

		AcDbEntity *pEnt = p->getResultEntity();
		AsdkHlrData::Visibility vis = p->getVisibility();
		if (vis == AsdkHlrData::kVisible)
		{
			pEnt->setColorIndex(7); //----- Read
		}
		else if (vis == AsdkHlrData::kInternallyHidden)
		{
			if (p->getHlrVisibility() == AsdkHlrData::kVisible)
				pEnt->setColorIndex(2); //----- Yellow
			else
				pEnt->setColorIndex(3); //----- Green
		}
		else
		{
			pEnt->setColorIndex(5); //----- Blue
		}

		solid3d = CDwgDatabaseUtil::PostToModelSpace(pEnt);
		if (solid3d == 0)
		{
			acutPrintf(_T("Failed to add entity to current space.\n"));
			break;
		}
		if (acdbOpenAcDbEntity(pEnt, solid3d, AcDb::kForRead) != Acad::eOk) 
		{
			acutPrintf(_T("Failed to open last added outputed curve.\n"));
			break;
		}

		//----- Entity originator path for block reference entities
		AcDbObjectIdArray ids = p->getObjectIds();
		if (ids.logicalLength() > 0)
		{
			acutPrintf(ACRX_T("\n%lx, "), pEnt->objectId().asOldId());
			if (p->getSubentId().type() != AcDb::kNullSubentType)
				acutPrintf(ACRX_T("[%ld, %ld], "), p->getSubentId().type(), p->getSubentId().index());
			for (int j = 0; j < ids.logicalLength(); j++)
				acutPrintf(ACRX_T("%lx, "), ids.at(j).asOldId());
			AcDbObjectId id = ids.last();
			if (!id.isNull()) {
				AcDbEntity *ent = NULL;
				acdbOpenAcDbEntity(ent, id, AcDb::kForRead);
				id = ent->linetypeId();
				ent->close();
				if (pEnt->upgradeOpen() == Acad::eOk)
					pEnt->setLinetype(id);
			}
		}

		pEnt->close();
	}
}
/*----------------------投影结束------------------*/


AcDbObjectId MD2010_AddAngleDimension2(AcGePoint3d centerPoint, AcGePoint3d xLine1Point, AcGePoint3d xLine2Point, AcGePoint3d arcPoint, const WCHAR * entryname, const ACHAR* newLayer)
{
	//AcGePoint3d aas =AcGePoint3d(0,0,0);
	if (CGePointUtil::PointDistance(xLine1Point, xLine2Point) <= USER_TOL * 1000000)//小于1的不标注
		return 0;

	AcDb2LineAngularDimension  *pDim = new AcDb2LineAngularDimension(centerPoint, xLine1Point, centerPoint, xLine2Point, arcPoint);


	//pDim->setDimltype();
	//AcDbObjectId dimSty = CDimensionUtil::GetDimstylerID(L"ZQV");
	//pDim->setDimensionStyle(dimSty);
	//pDim->setColorIndex(3);
	AcDbObjectId dimID = CDwgDatabaseUtil::PostModalToBlockTable(pDim);

	//pDim->setLayer(newLayer);
	//pDim->setColorIndex(3);
	pDim->close();
	return dimID;
}

AcDbObjectId MD2010_AddAngleDimension3(AcGePoint3d centerPoint1, AcGePoint3d xLine1Point,
	AcGePoint3d centerPoint2, AcGePoint3d xLine2Point,
	AcGePoint3d arcPoint, AcGePoint3d textPoint,
	CString replaceText, const WCHAR * entryname, const ACHAR* newLayer)
{
	//AcGePoint3d aas =AcGePoint3d(0,0,0);
	if (CGePointUtil::PointDistance(xLine1Point, xLine2Point) <= USER_TOL*100 * 10000)//小于1的不标注
		return 0;

	AcDb2LineAngularDimension  *pDim = new AcDb2LineAngularDimension(centerPoint1, xLine1Point, centerPoint2, xLine2Point, arcPoint);


	//pDim->setDimltype();
	//AcDbObjectId dimSty = CDimensionUtil::GetDimstylerID(L"ZQV");
	//默认样式
	//AcDbObjectId dimSty = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
	//pDim->setDimensionStyle(dimSty);
	//pDim->setColorIndex(3);

	if (replaceText.GetLength() > 0)
	{
		pDim->setDimensionText(replaceText);
		pDim->setTextPosition(textPoint);
	}

	AcDbObjectId dimID = CDwgDatabaseUtil::PostModalToBlockTable(pDim);

	//pDim->setLayer(newLayer);
	//pDim->setColorIndex(3);
	pDim->close();
	return dimID;
}

AcDbObjectId MD2010_AddAlignedDimension_GongCha2(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, double gc1, double gc2, const WCHAR * preStr, const WCHAR * entryname, const ACHAR* newLayer, CString replaceText, double roa)
{
	if (CGePointUtil::PointDistance(start, end) <= USER_TOL * 100 * 10000)//小于1的不标注
		return 0;

	//AcDbAlignedDimension  *pDim = new AcDbAlignedDimension(start,end,dimlinpnt);
	AcDbRotatedDimension  *pDim = new AcDbRotatedDimension(roa, start, end, dimlinpnt);

	//pDim->setColorIndex(3);
	pDim->setDimtol(true);
	pDim->setDimtm(gc1);
	pDim->setDimtp(gc2);
	pDim->setPrefix(AcString(preStr));//前缀
	pDim->setLayer(newLayer);
	pDim->setDimtdec(1);//三个小数点


	pDim->setToleranceSuppressTrailingZeros(false);//后续0
	pDim->setToleranceSuppressLeadingZeros(false);//前导0
												  //修改标注风格
	//AcDbObjectId dimSty = CDimensionUtil::GetDimstylerID(L"ZQH");
	//pDim->setDimensionStyle(dimSty);

	if (replaceText.GetLength() > 0)
	{
		pDim->setDimensionText(replaceText);
		pDim->setTextPosition(dimlinpnt);
	}

	AcDbObjectId dimID = CDwgDatabaseUtil::PostModalToBlockTable(pDim);

	pDim->close();
	return dimID;
}

//带公差----  注意 AcDbDimension类 包含了 头文件  #include "dbdimvar.h"//这里面有好多函数
AcDbObjectId MD2010_AddAlignedDimension_GongCha(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, double gc1, double gc2, const WCHAR * preStr, const WCHAR * entryname, const ACHAR* newLayer, CString replaceText, double roa)
{
	if (CGePointUtil::PointDistance(start, end) <= CMathUtil::PI * 10000)//小于1的不标注
		return 0;

	AcDbRotatedDimension  *pDim = new AcDbRotatedDimension(roa, start, end, dimlinpnt);

	pDim->setColorIndex(3);
	pDim->setDimtol(true);
	pDim->setDimtm(0 - gc1);
	pDim->setDimtp(gc2);
	pDim->setPrefix(AcString(preStr));//前缀
	pDim->setLayer(newLayer);
	pDim->setDimtdec(1);//保留一位小数


	pDim->setToleranceSuppressTrailingZeros(false);//后续0
	pDim->setToleranceSuppressLeadingZeros(false);//前导0

	AcDbObjectId dimSty = CDimensionUtil::GetDimstylerID(L"ZQH");
	pDim->setDimensionStyle(dimSty);

	if (replaceText.GetLength() > 0)
	{
		pDim->setDimensionText(replaceText);
		pDim->setTextPosition(dimlinpnt);
	}

	AcDbObjectId dimID = CDwgDatabaseUtil::PostModalToBlockTable(pDim);

	pDim->close();
	return dimID;
}