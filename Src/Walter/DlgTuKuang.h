#pragma once
#include "afxwin.h"


// CDlgTuKuang �Ի���

class CDlgTuKuang : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTuKuang)

public:
	CDlgTuKuang(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTuKuang();
	virtual BOOL OnInitDialog();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TUKUANG };
#endif

private:
	void Init();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:




	// ͼ��ѡ��
	CComboBox m_tukuang;
	// ����
	CComboBox m_proportion;
	CString m_designer;
	CString m_SapNum;
	afx_msg void OnBnClickedButton1();
	CString m_designDate;
	CComboBox m_KnifeClassSel;
};
