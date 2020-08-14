#pragma once
#include "Com.h"
#include "GridCtrl/GridCellCheckEx.h"
#include "TDData.h"
// CDlgTd 对话框

class CDlgTd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTd)

public:
	CDlgTd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTd();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CGridCtrlEx m_tdSegDataCtrl;
	
	
	TDSegData m_tdSegData;
	CTDData m_tdData;
};
