#pragma once
#include "afxwin.h"
#include "PCDXDdata.h"

// CDlgPcdXd 对话框

class CDlgPcdXd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPcdXd)

public:
	CDlgPcdXd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPcdXd();
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	virtual BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2_PCD_XD };
#endif



private:
	void InitParameters();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	PCDXD m_Data;

	// 刀柄
	CComboBox m_ui_HandleCtrl;
	//直径
	double m_DiameterOfKnife;
	// 悬伸长度
	double m_lengthOfKnife;
	// 刃长
	double m_BladeLength;
	afx_msg void OnBnClickedButton1();
};
