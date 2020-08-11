#pragma once

#include "GridCtrl/GridCtrlEx.h"
#include "res/resource.h"
#include "afxwin.h"
#include "PCDJDData.h"
#include "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxwin.h"

typedef vector<CString> OneRowData;
typedef vector<OneRowData> MultiRowData;


typedef struct JDData
{
	CString m_uiData_hiltChoose;	//ui界面——刀柄选择
	CString m_uiData_stepNumChoose;		//ui界面——阶梯数量选择
	MultiRowData m_uiData_allListData;	//点击接替数量后自动生成的表数据
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
	CComboBox m_ui_stepNum;		//阶梯选择的下拉框
	CComboBox m_ui_hiltChoose;		//刀柄选择的下拉框
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonGenerateDwg();
	afx_msg void OnCbnSelchangeComboStepNum();
	void InitGridCtrl();
	void FillTable(MultiRowText data);
	MultiRowData GetTable();
	MultiRowData FillDefaultData(int rowCount, int tableColumn);
	void SetCellHeight();

	SPCDJDData m_data;

	double m_totalLength;
};
