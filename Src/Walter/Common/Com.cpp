#include "stdafx.h"
#include "Com.h"
#include "Entity/DynamicBlockUtil.h"
#include "DlgPcdJD.h"
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
//���ݵ��������ƻ�ȡһ������
double GetDisByDBName(CString name)
{
	double dis = 22.0;
	if (name.Compare(L"BT30") == 0)			dis = 22.0;		//BT30 22
	else if (name.Compare(L"BT40") == 0)	dis = 27.0;		//BT40 27
	else if (name.Compare(L"BT50") == 0)	dis = 38.0;		//BT50 38
	else if (name.Compare(L"HSK-A40")==0)	dis = 20.0;		//HSK-A40 20
	else if (name.Compare(L"HSK-A50") == 0)	dis = 25.95;	//HSK-A50 25.95
	else if (name.Compare(L"HSK-A63") == 0)	dis = 25.95;	//HSK-A63 25.95
	else if (name.Compare(L"HSK-A80") == 0)	dis = 25.95;	//HSK-A80 25.95
	else if (name.Compare(L"HSK-A100") == 0)dis = 28.95;	//HSK-A100 28.95
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

//���߿�洢Ŀ¼
CString TY_GetDwgFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath() + "\\Support\\Walter\\DaoJuXingHao\\";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\dwg\\";
#endif // DEV_TEST_ENV
}


//�õ�ͼ��洢·��
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
	return "C:\\TYTool.dll";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\frame1\\TYTool.dll";
#endif // DEV_TEST_ENV
}


//���߿�洢Ŀ¼
CString TY_GetDynamicBlockFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath() + "\\Support\\Walter\\DynamicBlocks\\";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\DynamicBlocks\\";
#endif // DEV_TEST_ENV
}

//���߿�洢Ŀ¼
CString TY_GetDaoBingFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath() + "\\Support\\Walter\\DaoBing\\";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\DaoBing\\";
#endif // DEV_TEST_ENV
}

//���ݵ����ȡ lf2��ֵ
double GetLf2ByDiameter(double diameter)
{
	double Lf2 = 2.5f; //����Ĭ��Ϊ2.5
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
//���ݵ���ֱ����ȡ��Ƭ�Ŀ�
double GetHeightByDiameter(double diameter)
{
	double Height = 2.0f; //����Ĭ��Ϊ2.5
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
//���ݵ���ֱ����ȡ��Ƭ�İ뾶
double GetRadiusByDiameter(double diameter)
{
	double radius = 3.2f; //����Ĭ��Ϊ2.5
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
	//dll��Դ����  
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
