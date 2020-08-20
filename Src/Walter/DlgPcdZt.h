#pragma once
#include "Com.h"
#include "GridCtrl/GridCtrlEx.h"
#include "afxwin.h"
#include "PCDZTData.h"
// CDlgPcdZt �Ի���

class CDlgPcdZt : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPcdZt)

public:
	CDlgPcdZt(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPcdZt();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PCD_ZT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	void InitGridCtrl();
	void SetCellHight();
	void loadGridData();
	MultiRowText getDefaultData(int rowCount);
	MultiRowText getTableData();
private:
	CGridCtrlEx m_gridctrl;
	CComboBox m_ui_HandleCtrl; //����
	double m_totalLen; //���쳤��
	CComboBox m_StepNumCtrl; // ��������

	CPCDZTData m_pcdZtData;
	

	MultiRowText m_allrowData;
public:
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton1();
};
