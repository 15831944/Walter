#pragma once
#include <vector>

#define DEV_TEST_ENV//�������Ի���
enum TUKuangType{A1 = 0, A2, A3};

CString TY_GetAppPath();
//���ݵ������ƻ�ȡdistance
double GetDisByDBName(CString name);
//���ݵ���ֱ����ȡ lf2��ֵ
double GetLf2ByDiameter(double diameter);
double GetRadiusByDiameter(double diameter);
double GetHeightByDiameter(double diameter);
//���߿�洢Ŀ¼
CString TY_GetDwgFolder();
CString TY_GetDynamicBlockFolder();
CString TY_GetDaoBingFolder();
//����һ�ֵ���
CString TY_GetDaoBingSFolder();
//�õ�ͼ��洢·��
CString TY_GetFrameFolder();

CString TY_GetDllFilePathName();
//���ݵ������ƻ�ȡ��������
double GetHandleLengthFromDaoBing(CString daoBingName);
//��ȡ�ļ����µ�����dwg�ļ�
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

//�Ƕȱ�ע2
AcDbObjectId MD2010_AddAngleDimension2(AcGePoint3d centerPoint, AcGePoint3d xLine1Point, AcGePoint3d xLine2Point, AcGePoint3d arcPoint,
	const WCHAR * entryname = ACDB_MODEL_SPACE, const ACHAR* newLayer = L"0");

//�Ƕȱ�ע3
AcDbObjectId MD2010_AddAngleDimension3(AcGePoint3d centerPoint1, AcGePoint3d xLine1Point,
	AcGePoint3d centerPoint2, AcGePoint3d xLine2Point,
	AcGePoint3d arcPoint, AcGePoint3d textPoint,
	CString replaceText = L"", const WCHAR * entryname = ACDB_MODEL_SPACE, const ACHAR* newLayer = L"0");

AcDbObjectId MD2010_AddAlignedDimension_GongCha2(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, double gc1, double gc2, const WCHAR * preStr, const WCHAR * entryname, const ACHAR* newLayer, CString replaceText, double roa);

AcDbObjectId MD2010_AddAlignedDimension_GongCha(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, double gc1, double gc2,
	const WCHAR * preStr = L"", const WCHAR * entryname = ACDB_MODEL_SPACE, const ACHAR* newLayer = L"0", CString replaceText = L"", double roa = 0);

void AN_Project3DSolidTo2D(AcDbObjectId solid3d);