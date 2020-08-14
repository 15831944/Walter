#pragma once
#include "Com.h"
#include "GridCtrl/GridCtrlEx.h"
#include "TDData.h"
#include "afxwin.h"
// CDlgTd �Ի���

class CDlgTd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTd)

public:
	CDlgTd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTd();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CGridCtrlEx m_tdSegDataCtrl;
	
	
	
	CTDData m_tdData;
public:
	MultiRowText GetTableData();
	MultiRowText FillDefaultData(int index);
	MultiRowText m_tdAllSegData;
	void Init();
	void InitGrid();
	void SetGridHeight();
	CComboBox m_SegNumCtrl;
	// ���쳤��
	double m_totallength;
	CComboBox m_ui_HandleCtrl;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo2();
	int m_labberCount;
};
