#pragma once

#include "GridCtrl/GridCtrlEx.h"
#include "res/resource.h"
#include "afxwin.h"
#include "GridCtrl/GridCtrlEx.h"
#include "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxwin.h"

typedef vector<CString>	OneRowData;
typedef vector<OneRowData> MultiRowData;
// CZTInfoDlg 对话框

class CZTInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZTInfoDlg)

public:
	CZTInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZTInfoDlg();
	virtual BOOL OnInitDialog();


private:
	void InitGridCtrl();
	void SetCellHight();
	void InitDefaultPara();
	void LoadGridData();
	MultiRowData getDefaultGridData(int index);
// 对话框数据
#if ARX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CWDR };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	CGridCtrlEx m_djInfoCtrl;
	double m_TotalLength;
	double m_VertAngle;
	CComboBox m_StepNum;
	CComboBox m_ui_DrNumCtrl;
	CComboBox m_ui_DrStyleCtrl;
	MultiRowData m_alldjInfos;
	double m_DrNum;
	CEdit m_TotalLenEdit;
	CEdit m_VertexEdit;
	afx_msg void OnBnClickedBtncancle();
	afx_msg void OnCbnSelchangeComboStepnum();
};
