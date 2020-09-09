#pragma once
#include "afxwin.h"
#include "Entity/BlockUtil.h"
#include "PrintDwg.h"
// CDlgPcdXd �Ի���

class CPrintDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPrintDlg)

public:
	CPrintDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPrintDlg();
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);

	virtual BOOL OnInitDialog();
	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PRINTER };
#endif



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	CPrintDwg m_printer; //���д�ӡ����

	BOOL m_printAll;
	int m_SinglePaper;
	int m_PaperClass;
};
