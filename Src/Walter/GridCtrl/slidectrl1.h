#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CSlidectrl1 包装类

class CSlidectrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CSlidectrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xCF5702B6, 0x84F2, 0x4389, { 0xB9, 0x17, 0x76, 0xA9, 0xCF, 0x3C, 0x9D, 0x2E } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID, 
		CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
		UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
			pPersist, bStorage, bstrLicKey); 
	}

	// 特性
public:


	// 操作
public:

	// _DSlide

	// Functions
	//

	void SetSlide(LPUNKNOWN pSld)
	{
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pSld);
	}
	long Merge(LPCTSTR FileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FileName);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// Properties
	//

	CString GetFileName()
	{
		CString result;
		GetProperty(0x1, VT_BSTR, (void*)&result);
		return result;
	}
	void SetFileName(CString propVal)
	{
		SetProperty(0x1, VT_BSTR, propVal);
	}
	unsigned long GetBkColor()
	{
		unsigned long result;
		GetProperty(0x2, VT_UI4, (void*)&result);
		return result;
	}
	void SetBkColor(unsigned long propVal)
	{
		SetProperty(0x2, VT_UI4, propVal);
	}
	unsigned long GetFrColor()
	{
		unsigned long result;
		GetProperty(0x3, VT_UI4, (void*)&result);
		return result;
	}
	void SetFrColor(unsigned long propVal)
	{
		SetProperty(0x3, VT_UI4, propVal);
	}
	BOOL GetBlackAndWhite()
	{
		BOOL result;
		GetProperty(0x4, VT_BOOL, (void*)&result);
		return result;
	}
	void SetBlackAndWhite(BOOL propVal)
	{
		SetProperty(0x4, VT_BOOL, propVal);
	}
	BOOL GetDrawBorder()
	{
		BOOL result;
		GetProperty(0x5, VT_BOOL, (void*)&result);
		return result;
	}
	void SetDrawBorder(BOOL propVal)
	{
		SetProperty(0x5, VT_BOOL, propVal);
	}
	BOOL GetRespectRatio()
	{
		BOOL result;
		GetProperty(0x6, VT_BOOL, (void*)&result);
		return result;
	}
	void SetRespectRatio(BOOL propVal)
	{
		SetProperty(0x6, VT_BOOL, propVal);
	}
	BOOL GetUseFullSpace()
	{
		BOOL result;
		GetProperty(0x7, VT_BOOL, (void*)&result);
		return result;
	}
	void SetUseFullSpace(BOOL propVal)
	{
		SetProperty(0x7, VT_BOOL, propVal);
	}
	BOOL GetAboutDisplay()
	{
		BOOL result;
		GetProperty(0x8, VT_BOOL, (void*)&result);
		return result;
	}
	void SetAboutDisplay(BOOL propVal)
	{
		SetProperty(0x8, VT_BOOL, propVal);
	}
	CString GetTooltipText()
	{
		CString result;
		GetProperty(0xb, VT_BSTR, (void*)&result);
		return result;
	}
	void SetTooltipText(CString propVal)
	{
		SetProperty(0xb, VT_BSTR, propVal);
	}
	BOOL GetShowTooltip()
	{
		BOOL result;
		GetProperty(0xc, VT_BOOL, (void*)&result);
		return result;
	}
	void SetShowTooltip(BOOL propVal)
	{
		SetProperty(0xc, VT_BOOL, propVal);
	}
	BOOL GetEnabled()
	{
		BOOL result;
		GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
		return result;
	}
	void SetEnabled(BOOL propVal)
	{
		SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
	}
};
