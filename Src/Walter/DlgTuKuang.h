#pragma once
#include "afxwin.h"


// CDlgTuKuang 对话框

class CDlgTuKuang : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTuKuang)

public:
	CDlgTuKuang(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTuKuang();
	virtual BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TUKUANG };
#endif

private:
	void Init();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:




	// 图框选择
	CComboBox m_tukuang;
	// 比例
	CComboBox m_proportion;
	CString m_designer;
	CString m_SapNum;
	afx_msg void OnBnClickedButton1();
	CString m_designDate;
	CComboBox m_KnifeClassSel;
};
