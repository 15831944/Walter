#pragma once
#include <vector>
//#define DEV_TEST_ENV//开发测试环境
enum TUKuangType{A1 = 0, A2, A3};

#define DIMLAYERNAME L"2" //标注所在图层
#define DIMSTYLENAME L"WALTER_1" // 标注样式
const size_t N = 2; //小数点后的有效数字
//技术部门或者应用部门
//#define APPLY_DEPARTMENT

//#define TY_DRAW_3D

//判断路径是否存在 如不存在则不可使用 
const BOOL WALTER =  PathFileExists(L"G:\\");

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
CString TY_GetDaoBingZtFolder();
CString TY_GetDaoBingZyFolder();
//得到图框存储路径
CString TY_GetFrameFolder();
CString TY_GetDllFilePathName();
CString TY_GetHLRFilePathName();
//根据刀柄名称获取刀柄长度
double GetHandleLengthFromDaoBing(CString daoBingName);
//获取文件夹下的所有dwg文件
vector<CString> GetAllDwgFile(const CString& dirPath);
//去掉浮点数无效的0
CString removeLastZero(double num);
//获取镜像点，输入 一个点和过这个点的一个空间向量确定一个平面 得到关于这个平面的对称的点
AcGePoint3d getMirrorPoint(AcGePoint3d point,AcGePoint3d const& ptbase,AcGeVector3d const& line);



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
	bool preUnit,AcDbObjectIdArray& idarr);

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

