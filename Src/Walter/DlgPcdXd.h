#pragma once
#include "afxwin.h"
#include "PCDXDdata.h"

// CDlgPcdXd �Ի���

class CDlgPcdXd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPcdXd)

public:
	CDlgPcdXd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPcdXd();
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	virtual BOOL OnInitDialog();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2_PCD_XD };
#endif



private:
	void InitParameters();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	PCDXD m_Data;

	// ����
	CComboBox m_ui_HandleCtrl;
	//ֱ��
	double m_DiameterOfKnife;
	// ���쳤��
	double m_lengthOfKnife;
	// �г�
	double m_BladeLength;
	afx_msg void OnBnClickedButton1();
};
