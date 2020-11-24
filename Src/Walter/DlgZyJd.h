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

	void SetKnifeClass(bool iskkd) { m_isKKd = iskkd; }
//��ʼ������
private:
	void InitDefaultPara();
	void InitGrid();
	void SetCellHight();
	MultiRowText GetDefaultGridData(int rowCount);
	void SetGridData();
	MultiRowText GetGridData();
	void ReloadGridData();
	
	//���¼���ʾ��ͼ
	void ReloadPic();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ZYKKD };
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

	double m_totalLength;
	//�ж�����Ӳ�µ��������׵�
	bool m_isKKd;
};
