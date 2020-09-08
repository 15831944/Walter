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
	// ����ͼ��
	BOOL m_AllBlock;
	// ����ͼ��
	BOOL m_partBlock;
	// ��ҳ
	BOOL m_SinglePaper;
	// ��ҳ
	BOOL m_multiPaper;
	// A3
	BOOL m_A3;
	// A4
	BOOL m_A4;
	CPrintDwg m_printer; //���д�ӡ����
	afx_msg void OnBnClickedRadio2();
	BOOL m_A2; //A2
};
