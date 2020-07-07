#pragma once

#include "GridCtrl/GridCtrlEx.h"
#include "res/resource.h"
#include "afxwin.h"
#include "Com.h"

typedef vector<CString> OneRowData;
typedef vector<OneRowData> MultiRowData;


typedef struct JDData
{
	CString m_uiData_hiltChoose;
	CString m_uiData_stepNumChoose;
	MultiRowData m_uiData_allListData;
}s_JDData;


// DlgPcdJD 对话框

class DlgPcdJD : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPcdJD)

public:
	DlgPcdJD(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgPcdJD();

// 对话框数据
	enum { IDD = IDD_DIALOG_PCD_JD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	s_JDData jdData;
	CGridCtrlEx m_gridCtrl;
	MultiRowData m_allListData;
	virtual BOOL OnInitDialog();
	CComboBox m_ui_stepNum;
	CComboBox m_ui_hiltChoose;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonGenerateDwg();
	afx_msg void OnCbnSelchangeComboStepNum();
	void InitGridCtrl();
	void FillTable(MultiRowText data);
	MultiRowData GetTable();
	MultiRowData FillDefaultData(int rowCount, int tableColumn);
	void SetCellHeight();

	SPCDJDData m_data;
};
