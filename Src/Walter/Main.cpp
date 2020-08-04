//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2013 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//
// AsdkAcUiSample.cpp : Defines the initialization routines for the DLL.
//
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include "Main.h"

#include "AcExtensionModule.h"
#include <rxmfcapi.h>
#include <dbobjptr.h>
#include <rxregsvc.h>
#include <rxboiler.h>
#include <AcDblClkEdit.h>

#include "Entity/LineUtil.h"
#include "Common/CommonUtil.h"
#include "Document/DwgDatabaseUtil.h"
#include "Others/XDataUtil.h"
#include "Entity/ObjectUtil.h"
#include "VBA/VBA_Inc.h"
//#include "TYProgress.h"
#include "Head.h"
#include "Others/ToolingUtil.h"

#include "WalterDialog.h"
#include "DlgPcdJD.h"
#include "ThreadData.h"
#include "ZcDjDlg.h"
#include "ZTInfoDlg.h"
#include "DlgTuKuang.h"
#include "DlgZyJd.h"
HINSTANCE g_tytoolInst = 0;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//extern "C" HWND adsw_acadMainWnd();

/////////////////////////////////////////////////////////////////////////////
// Define the sole extension module object.

ACRX_DEFINE_MEMBERS(AcDbDoubleClickEdit);
enum Acad::ErrorStatus __stdcall LoadManagedDll(ACHAR const *path);

AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);

void dialogCreate()
{
	g_Walter = new WalterDialog(acedGetAcadFrame());
	g_Walter->Create(IDD_Walter);
	g_Walter->ShowWindow(SW_SHOW);
}

DlgPcdJD* g_dlgPcdJd1 = NULL;
void CMD_PCDJD1()
{
	CAcModuleResourceOverride resOverride;
	g_dlgPcdJd1 = new DlgPcdJD(acedGetAcadFrame());
	g_dlgPcdJd1->Create(IDD_DIALOG_PCD_JD);
	g_dlgPcdJd1->ShowWindow(SW_SHOW);
}
//��ͷ��Ϣ
CZTInfoDlg *g_ztInfoDlg = NULL;
void CMD_CWDR()
{
	CAcModuleResourceOverride resOverride;
	g_ztInfoDlg = new CZTInfoDlg(acedGetAcadFrame());
	g_ztInfoDlg->Create(IDD_DIALOG_CWDR);
	g_ztInfoDlg->ShowWindow(SW_SHOW);
}
//ֱ�۵���
CZcDjDlg *gZcDjDlg = NULL;
void CMD_ZCDJ()
{
	CAcModuleResourceOverride resOverride;
	gZcDjDlg = new CZcDjDlg(acedGetAcadFrame());
	gZcDjDlg->Create(IDD_DIALOG_DCD);
	gZcDjDlg->ShowWindow(SW_SHOW);
}
//��Ӳ���׵�
CDlgZyJd *gZyXd = NULL;
void CMD_ZYKKD()
{
	CAcModuleResourceOverride resOverride;
	gZyXd = new CDlgZyJd(acedGetAcadFrame());
	gZyXd->SetKnifeClass(true);
	gZyXd->Create(IDD_DIALOG_PCD_XD);
	gZyXd->ShowWindow(SW_SHOW);
}
//��Ӳϳ��
void CMD_ZYJD()
{
	CAcModuleResourceOverride resOverride;
	gZyXd = new CDlgZyJd(acedGetAcadFrame());
	gZyXd->SetKnifeClass(false);
	gZyXd->Create(IDD_DIALOG_PCD_XD);
	gZyXd->ShowWindow(SW_SHOW);
}
//ͼ��
CDlgTuKuang *gTuKuang = NULL;
void CMD_INTK()
{
	CAcModuleResourceOverride resOverride;
	gTuKuang = new CDlgTuKuang(acedGetAcadFrame());
	gTuKuang->Create(IDD_DIALOG_TUKUANG);
	gTuKuang->ShowWindow(SW_SHOW);
}
//���������Ա
void updateAttr()
{
	g_Walter->UpdateDesignerAttr();
}

//���������Ա
void UpadeAtuAttr()
{
	g_Walter->UpdateAtuAttr();
}

void UpadenewBomExcel()
{
	g_Walter->UpdateBomExcel();
}

void RepairDwgs()
{
	CString dir = CCommonUtil::GetFolderFromDialog(L"��ѡ����Ҫ�޸���dwg�ļ���");
	if (dir.GetLength() == 0)
	{
		acutPrintf(L"\nû��ѡ��dwg�ļ���\n",0);
		return;
	}

	CString dirOut = CCommonUtil::GetFolderFromDialog(L"��ѡ���´洢���ļ���");
	if (dirOut.GetLength() == 0)
	{
		acutPrintf(L"\nû��ѡ��dwg�ļ���\n", 0);
		return;
	}

	if (dirOut == dir)
	{
		acutPrintf(L"\n����ѡ��������ͬ���ļ���\n", 0);
		return;
	}

	vector<CString> allDwgFilesToRepair;
	CCommonUtil::FindAllFile(dir, allDwgFilesToRepair, _T("*.dwg"));

	CDocLock lock;
	CString info;
	TY_Progress_Init();
	for (int i = 0; i < allDwgFilesToRepair.size(); i++)
	{
		TY_SetProgress((int)allDwgFilesToRepair.size(), i + 1);
		info.Format(L"һ��%d���ļ������ڴ����%d��\n", allDwgFilesToRepair.size(), i + 1);
		acutPrintf(info);
		//Ԥ�Ȳ���һ��dwg�汾 ����oDocs.Open��ֱ�ӱ���
		AcDbDatabase* pDb = new AcDbDatabase(false);
		Acad::ErrorStatus es = pDb->readDwgFile(dir + L"\\" + allDwgFilesToRepair[i], AcDbDatabase::kForReadAndAllShare);
		if (es == Acad::eOk)
			es = es = pDb->saveAs(dirOut + L"\\" + allDwgFilesToRepair[i],false);
		delete pDb;
		pDb = 0;
	}
	TY_Progress_Close();
	//CDwgDatabaseUtil::OpenDocument(L"E:\\CAD����ͼֽ\\B3230.C8.290-360.Z1.CC06.dwg",true);
}

static void CMD_Test()
{
	int a = 0;
	{
		CThreadData threadData;
		CThreadSegData oneSeg;
		oneSeg.m_length = 30;
		oneSeg.m_diameter = 12;
		oneSeg.m_ladderAngle1 = 90;
		threadData.AddCutterSegData(oneSeg);

		oneSeg.m_diameter = 14;
		threadData.AddCutterSegData(oneSeg);

		oneSeg.m_diameter = 16;
		threadData.AddCutterSegData(oneSeg);

		threadData.SetLadderCount(3);

		AcDbObjectId id;
		threadData.CreateModel3D(AcGePoint2d(0, 0), id);
	}
	
	return;


	
	return;

	vAcDbObjectId ids;
	CToolingUtil::CycleAllTypedObjectsInAllLayer(CToolingUtil::ACDB_CURVE, ids);

	
		
	/*AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return;
	}

	AcDbDimension * pdim = AcDbDimension::cast(pEnt);
	bool isdynmic = pdim->isDynamicDimension();
	bool isCon = pdim->isConstraintDynamic();
	pEnt->close();*/
}

static void initApp()
{

	CAcModuleResourceOverride resOverride;

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("test"),
		_T("test"),
		ACRX_CMD_MODAL,
		CMD_Test,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("GYZ1"),
		_T("GYZ1"),
		ACRX_CMD_MODAL,
		dialogCreate,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("gx"),
		_T("gx"),
		ACRX_CMD_MODAL,
		updateAttr,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("setCheck"),
		_T("setCheck"),
		ACRX_CMD_MODAL,
		UpadeAtuAttr,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("setBom"),
		_T("setBom"),
		ACRX_CMD_MODAL,
		UpadenewBomExcel,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("WRepair"),
		_T("WRepair"),
		ACRX_CMD_MODAL,
		RepairDwgs,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("PCDJD1"),
		_T("PCDJD1"),
		ACRX_CMD_MODAL,
		CMD_PCDJD1,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("CWDR"),
		_T("CWDR"),
		ACRX_CMD_MODAL,
		CMD_CWDR,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("ZCDJ"),
		_T("ZCDJ"),
		ACRX_CMD_MODAL,
		CMD_ZCDJ,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("ZYKKD"),
		_T("ZYKKD"),
		ACRX_CMD_MODAL,
		CMD_ZYKKD,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("ZYJD"),
		_T("ZYJD"),
		ACRX_CMD_MODAL,
		CMD_ZYJD,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("INTK"),
		_T("INTK"),
		ACRX_CMD_MODAL,
		CMD_INTK,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());
	//ɾ��˫���¼�
	//LoadManagedDll(CCommonUtil::GetAppPath() + L"\\support\\rcdc.dll");
	TY_LoadTyTool();

	
}


static void unloadApp()
{
	TY_FreeTyTool();
	// Do other cleanup tasks here  
	acedRegCmds->removeGroup(_T("ASDK_ACUI_SAMPLE"));
	//delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE));
}



//////////////////////////////////////////////////////////////
//
// Entry points
//
//////////////////////////////////////////////////////////////

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		theArxDLL.AttachInstance(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		theArxDLL.DetachInstance();
	}
	return 1;   // ok
}


void menu()
{
	TRY
	{
	CAcadApplication IAcad(acedGetAcadWinApp()->GetIDispatch(TRUE));
	CAcadMenuBar IMenuBar(IAcad.get_MenuBar());

	long numberOfMenus;
	numberOfMenus = IMenuBar.get_Count();

	CAcadMenuGroups IMenuGroups(IAcad.get_MenuGroups());

	VARIANT index;
	VariantInit(&index);
	V_VT(&index) = VT_I4;
	V_I4(&index) = 0;

	CAcadMenuGroup IMenuGroup(IMenuGroups.Item(index));
	CAcadPopupMenus IPopUpMenus(IMenuGroup.get_Menus());

	CString cstrMenuName = _T("�߶��ع���");//�Զ������˵�

	VariantInit(&index);
	V_VT(&index) = VT_BSTR;
	V_BSTR(&index) = cstrMenuName.AllocSysString();

	IDispatch* pDisp = NULL;
	TRY{ pDisp = IPopUpMenus.Item(index); pDisp->AddRef(); } CATCH(COleDispatchException,e) {}END_CATCH;

	if (pDisp == NULL)
	{
		CAcadPopupMenu IPopUpMenu(IPopUpMenus.Add(cstrMenuName));
		UINT MenuIndex = 0;

		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = MenuIndex++;
		IPopUpMenu.AddMenuItem(index, _T("&���������׼ͼֽ"),_T("_GYZ1 "));

		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = MenuIndex++;
		IPopUpMenu.AddMenuItem(index, _T("&�߶���ͼֽ�޸�"), _T("_WREPAIR "));


		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = MenuIndex++;
		IPopUpMenu.AddMenuItem(index, _T("&PCD�µ�/��Ӳ�ʺϽ�"), _T("PCDJD1 "));

		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = MenuIndex++;
		IPopUpMenu.AddMenuItem(index, _T("&��ͷ���"), _T("CWDR "));

		/*VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = MenuIndex++;
		IPopUpMenu.AddMenuItem(index, _T("&ֱ�۵���"), _T("ZCDJ "));
*/
		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = MenuIndex++;
		IPopUpMenu.AddMenuItem(index, _T("&��Ӳ���׵�"), _T("ZYKKD "));

		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = MenuIndex++;
		IPopUpMenu.AddMenuItem(index, _T("&��Ӳ�µ�"), _T("ZYJD "));

		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = MenuIndex++;
		IPopUpMenu.AddMenuItem(index, _T("&����ͼ��"), _T("INTK "));

		pDisp = IPopUpMenu.m_lpDispatch;
		pDisp->AddRef();
	}

	CAcadPopupMenu IPopUpMenu(pDisp);
	if (!IPopUpMenu.get_OnMenuBar())
	{
		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = numberOfMenus;
		IPopUpMenu.InsertInMenuBar(index);
	}
	else
	{
		VariantInit(&index);
		V_VT(&index) = VT_BSTR;
		V_BSTR(&index) = cstrMenuName.AllocSysString();
		IPopUpMenus.RemoveMenuFromMenuBar(index);
		VariantClear(&index);
	}
	pDisp->Release();
	}
		CATCH(COleDispatchException, e)
	{
		e->ReportError();
		e->Delete();
	}
	END_CATCH;
}


extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
	switch (msg)
	{
	case AcRx::kInitAppMsg:
		acrxDynamicLinker->unlockApplication(appId);
		acrxDynamicLinker->registerAppMDIAware(appId);
		initApp();
		menu();
		break;
	case AcRx::kUnloadAppMsg:
		unloadApp();
		break;
	case AcRx::kInitDialogMsg:

		break;
	default:
		break;
	}
	return AcRx::kRetOK;
}

