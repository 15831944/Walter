#pragma once
#include "afxwin.h"
#include "GridCtrl/GridCtrlEx.h"
#include "Com.h"
#include "ZYDJData.h"

// CDlgZyJd �Ի���

class CDlgZyJd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgZyJd)

public:
	CDlgZyJd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual BOOL OnInitDialog();
	
	virtual ~CDlgZyJd();


//��ʼ������
private:
	void InitDefaultPara();
	void InitGrid();
	void SetCellHight();
	MultiRowText GetDefaultGridData(int rowCount);
	void SetGridData();
	MultiRowText GetGridData();
	void ReloadGridData();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PCD_XD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ui_DbCtrl;
	CComboBox m_ui_LabberCtrl;

	MultiRowText m_allrowData; //��gridctrl��Ӧ������
	CGridCtrlEx m_XdLabberDataCtrl;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton1();
	// ���׵�Ԥ��ֱ��
	double m_diameter;

	//�������
	CZYDJData m_ZyDjData;
	CComboBox m_SelKnifeClass;
	afx_msg void OnCbnSelchangeCombo3();
	double m_totalLength;
};
