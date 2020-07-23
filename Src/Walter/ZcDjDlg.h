#pragma once
#include "GridCtrl/GridCtrlEx.h"
#include "ThreadData.h"
#include "afxwin.h"
#include "res/resource.h"
#include "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxwin.h"

// CZcDjDlg 对话框

class CZcDjDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZcDjDlg)

public:
	CZcDjDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZcDjDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DCD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedZcdraw();
	afx_msg void OnCbnSelchangeCombo2();

private:
	void InitParamers();
	//初始化自定义控件
	void InitGridCtrl();
	//设置每个格子的高
	void SetCellHight();
	void LoadGridData();
	MultiRowText getDefaultGridData(int index);
	MultiRowText getTableData();
	void loadDaoBing();

public:
	MultiRowText m_allStepData;

	CThreadData m_data;
	CString m_DaoBingName;
	CComboBox m_DaoBingCtrl;
	double m_totalLength;
	double m_VertexAngle;
	CComboBox m_StepNumCtrl;
	CGridCtrlEx m_ZcDjJTInfoCtrl;

	CThreadSegData m_segdata;
	
	CComboBox m_tukuang;		//图框
	CComboBox m_proportion;		//比例
	CString m_designer;			//设计人员
	CString m_degineDate;		//设计日期
	CString m_knifeCLass;		//刀具类别
	CString m_SapNum;			//SAP号
};
