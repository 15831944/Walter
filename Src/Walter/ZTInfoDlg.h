#pragma once
#include "GridCtrl/GridCtrlEx.h"
#include "res/resource.h"
#include "afxwin.h"
#include "ThreadData.h"

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
	MultiRowData getTableData();
// 对话框数据
#if ARX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CWDR };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedBtnok();
	afx_msg void OnCbnSelchangeComboStepnum();
	
public:
	CGridCtrlEx m_djInfoCtrl;
	double m_TotalLength; //总长
	double m_VertAngle;  //顶角
	CComboBox m_StepNum;  //阶梯数量
	CComboBox m_ui_DrNumCtrl; //刀刃数量
	MultiRowData m_alldjInfos; // 刀具所有的信息
	CEdit m_TotalLenEdit; //总长
	CEdit m_VertexEdit; //

	CThreadSegData m_segdata; //刀刃数据
	CThreadData m_data; 
	CComboBox m_DaoBingCtrl; //刀柄选择

	CComboBox m_DrillSel;
	afx_msg void OnCbnSelchangeCombo1();
	double m_GrooveLenth; //排屑槽长

};
