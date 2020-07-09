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

//刀具库存储目录
CString TY_GetDwgFolder()
{
#ifdef DEV_TEST_ENV
	return TY_GetAppPath() + "\\Support\\Walter\\DaoJuXingHao\\";
#else
	return L"G:\\Departments\\TT\\WCN Database\\10_CAD Block\\Walter\\dwg\\";
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
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"请选择一个插入点:", pnt);
	CDocLock lock;	//如果读取文件失败，可能是因为之前没有加锁

	//插入对应的刀身
	CString daoShenFilePath = TY_GetDwgFolder();
	CString str;
	str.Format(L"%d", m_stepNum);
	daoShenFilePath.Append(L"DaoShen\\PCD铰刀模板X" + str + L".dwg");
	AcDbObjectId daoShenID = CBlockUtil::InsertDwgAsBlockRef(daoShenFilePath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);

	//插入对应的刀柄,并设置参数
	CString daoBingFilePath = TY_GetDwgFolder();
	daoBingFilePath.Append(L"DaoBing\\" + m_daoBing + L".dwg");
	double MaxLLen;
	CDynamicBlockUtil::GetDynamicBlockData(daoShenID, L"L", MaxLLen);
	AcGePoint3d insertPiont(pnt.x - MaxLLen, pnt.y, 0);
	CBlockUtil::InsertDwgAsBlockRef(daoBingFilePath, NULL, ACDB_MODEL_SPACE, insertPiont, 0, 1);
	switch (m_stepNum)
	{
	case 1:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	case 2:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	case 3:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 3].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 3].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[m_stepNum - 2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[m_stepNum - 2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	case 4:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 4].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 4].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[m_stepNum - 3].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[m_stepNum - 3].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[m_stepNum - 2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[m_stepNum - 2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D4", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	default:
		break;
	}

	CViewUtil::ZoomExtent();
	return 0;
}