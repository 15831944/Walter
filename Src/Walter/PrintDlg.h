#pragma once
#include "afxwin.h"
#include "Entity/BlockUtil.h"
#include "PrintDwg.h"
// CDlgPcdXd 对话框

class CPrintDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrintDlg)

public:
	CPrintDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrintDlg();
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);

	virtual BOOL OnInitDialog();
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PRINTER };
#endif



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	// 所有图框
	BOOL m_AllBlock;
	// 部分图框
	BOOL m_partBlock;
	// 单页
	BOOL m_SinglePaper;
	// 多页
	BOOL m_multiPaper;
	// A3
	BOOL m_A3;
	// A4
	BOOL m_A4;
	CPrintDwg m_printer; //进行打印工作
	afx_msg void OnBnClickedRadio2();
	BOOL m_A2; //A2
};
