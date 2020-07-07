#include "stdafx.h"
#include "Com.h"

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
	return TY_GetAppPath()+ "\\Support\\Walter\\DaoJuXingHao\\";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\dwg\\";
#endif // DEV_TEST_ENV
}

//得到图框存储路径
CString TY_GetFrameFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath()+ "\\Support\\Walter\\TuKuang\\";
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

int SPCDJDData::Draw()
{
	//第一步：用户选择一个输入点

	//第二步 插入刀具主体动态块

	//第三步：定位和插入刀柄

	//第四步：设置8个数据
	return 0;
}