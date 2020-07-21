#pragma once
#include <vector>

#define DEV_TEST_ENV//开发测试环境
enum TUKuangType{A1 = 0, A2, A3};

CString TY_GetAppPath();
//根据刀柄名称获取distance
double GetDisByDBName(CString name);
//根据刀具直径获取 lf2的值
double GetLf2ByDiameter(double diameter);
double GetRadiusByDiameter(double diameter);
double GetHeightByDiameter(double diameter);
//刀具库存储目录
CString TY_GetDwgFolder();
CString TY_GetDynamicBlockFolder();
CString TY_GetDaoBingFolder();
//另外一种刀具
CString TY_GetDaoBingSFolder();
//得到图框存储路径
CString TY_GetFrameFolder();

CString TY_GetDllFilePathName();
//根据刀柄名称获取刀柄长度
double GetHandleLengthFromDaoBing(CString daoBingName);
//获取文件夹下的所有dwg文件
vector<CString> GetAllDwgFile(const CString& dirPath);


bool TY_LoadTyTool();
bool TY_FreeTyTool();
int TY_SetProgress(int all, int cur);
void TY_Progress_Close();
void TY_Progress_Init();
int JHCOM_ExtrudeAlongPath2(AcDbObjectId regionid, AcDbObjectId path, AcDbObjectId &id);

void TY_Project3DSolidTo2D(AcDbObjectId solid3d,
	bool hidLines,
	bool  internalVisibility,
	bool calculateSilhouettes,
	bool preUnit);

//角度标注2
AcDbObjectId MD2010_AddAngleDimension2(AcGePoint3d centerPoint, AcGePoint3d xLine1Point, AcGePoint3d xLine2Point, AcGePoint3d arcPoint,
	const WCHAR * entryname = ACDB_MODEL_SPACE, const ACHAR* newLayer = L"0");

//角度标注3
AcDbObjectId MD2010_AddAngleDimension3(AcGePoint3d centerPoint1, AcGePoint3d xLine1Point,
	AcGePoint3d centerPoint2, AcGePoint3d xLine2Point,
	AcGePoint3d arcPoint, AcGePoint3d textPoint,
	CString replaceText = L"", const WCHAR * entryname = ACDB_MODEL_SPACE, const ACHAR* newLayer = L"0");

AcDbObjectId MD2010_AddAlignedDimension_GongCha2(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, double gc1, double gc2, const WCHAR * preStr, const WCHAR * entryname, const ACHAR* newLayer, CString replaceText, double roa);

AcDbObjectId MD2010_AddAlignedDimension_GongCha(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, double gc1, double gc2,
	const WCHAR * preStr = L"", const WCHAR * entryname = ACDB_MODEL_SPACE, const ACHAR* newLayer = L"0", CString replaceText = L"", double roa = 0);

void AN_Project3DSolidTo2D(AcDbObjectId solid3d);