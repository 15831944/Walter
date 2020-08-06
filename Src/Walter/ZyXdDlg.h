#pragma once
#include "afxwin.h"
#include "ThreadData.h"

// CZyXdDlg �Ի���

class CZyXdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZyXdDlg)

public:
	CZyXdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZyXdDlg();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButton1();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ZYXD };
#endif


private:
	void InitDefaultPara(); //��ʼ������


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	// ����
	CComboBox m_ui_DbCtrl;
	// ϳ��ֱ��
	double m_diameter;
	//�г�
	double m_lengthOfKnife;
	// ����Բ��
	double m_ArcOfknife;
	// ���쳤��
	double m_lengthOfHang;
	
	//ThreadData && ThreadSegData
	CThreadData m_ThreadData;
	CThreadSegData m_ThreadSegData;
};
