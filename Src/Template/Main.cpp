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

#include "DialogFileName.h"
#include "AcExtensionModule.h"
#include <rxmfcapi.h>
#include <dbobjptr.h>
#include <rxregsvc.h>
#include <rxboiler.h>
#include <AcDblClkEdit.h>

#include "Entity/LineUtil.h"
#include "Common/CommonUtil.h"
#include "Others/XDataUtil.h"
#include "Entity/ObjectUtil.h"
#include "VBA/VBA_Inc.h"
#include "Common/CommonUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern "C" HWND adsw_acadMainWnd();

/////////////////////////////////////////////////////////////////////////////
// Define the sole extension module object.

ACRX_DEFINE_MEMBERS(AcDbDoubleClickEdit);
enum Acad::ErrorStatus __stdcall LoadManagedDll(ACHAR const *path);

AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);

void dialogCreate()
{
	AcDbObjectId id = CLineUtil::CreateLine(AcGePoint3d(0,0,0), AcGePoint3d(100, 0, 0));

	//CCommonUtil::IsFileExist(L"");
	// Modal
	AsdkAcUiDialogSample dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	INT_PTR nReturnValue = dlg.DoModal();

}

static void initApp()
{

	CAcModuleResourceOverride resOverride;

	acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
		_T("ASDKACUISAMPLE"),
		_T("ACUISAMPLE"),
		ACRX_CMD_MODAL,
		dialogCreate,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());


	//�Զ������ĳ�ʼ������
	//ZffDwgScale::rxInit();
	//acrxBuildClassHierarchy();
	//acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE));

	//ɾ��˫���¼�
	//LoadManagedDll(CCommonUtil::GetAppPath() + L"\\support\\rcdc.dll");
}


static void unloadApp()
{

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

	CString cstrMenuName = _T("ͼԴ����");//�Զ������˵�

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
		CAcadPopupMenu IPop1(IPopUpMenu.AddSubMenu(index, _T("&ʾ��1")));//�Ӳ˵�


		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = MenuIndex++;
		IPop1.AddMenuItem(index, _T("&���˵�1"), _T("_ZT1 "));//�Ӳ˵�

		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = MenuIndex++;
		IPop1.AddMenuItem(index, _T("&�Ӳ˵�ʾ��2"), _T("_ZT2 "));//�Ӳ˵�



		IPopUpMenu.AddMenuItem(index, _T("&���˵�2"), _T("_GYT1 "));//�Ӳ˵�


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

