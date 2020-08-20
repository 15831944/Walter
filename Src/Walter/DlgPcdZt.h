#pragma once
#include "Com.h"
#include "GridCtrl/GridCtrlEx.h"
#include "afxwin.h"
#include "PCDZTData.h"
// CDlgPcdZt 对话框

class CDlgPcdZt : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPcdZt)

public:
	CDlgPcdZt(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPcdZt();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PCD_ZT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void InitGridCtrl();
	void SetCellHight();
	void loadGridData();
	MultiRowText getDefaultData(int rowCount);
	MultiRowText getTableData();
private:
	CGridCtrlEx m_gridctrl;
	CComboBox m_ui_HandleCtrl; //刀柄
	double m_totalLen; //悬伸长度
	CComboBox m_StepNumCtrl; // 刀刃数量

	CPCDZTData m_pcdZtData;
	

	MultiRowText m_allrowData;
public:
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton1();
};
