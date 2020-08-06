#pragma once
#include "afxwin.h"
#include "ThreadData.h"

// CZyXdDlg 对话框

class CZyXdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZyXdDlg)

public:
	CZyXdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZyXdDlg();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButton1();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ZYXD };
#endif


private:
	void InitDefaultPara(); //初始化参数


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	// 刀柄
	CComboBox m_ui_DbCtrl;
	// 铣刀直径
	double m_diameter;
	//刃长
	double m_lengthOfKnife;
	// 刀尖圆角
	double m_ArcOfknife;
	// 悬伸长度
	double m_lengthOfHang;
	
	//ThreadData && ThreadSegData
	CThreadData m_ThreadData;
	CThreadSegData m_ThreadSegData;
};
