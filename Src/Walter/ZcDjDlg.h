#pragma once
#include "GridCtrl/GridCtrlEx.h"
#include "ThreadData.h"
#include "afxwin.h"
#include "res/resource.h"
#include "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxwin.h"

// CZcDjDlg �Ի���

class CZcDjDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZcDjDlg)

public:
	CZcDjDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZcDjDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DCD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedZcdraw();
	afx_msg void OnCbnSelchangeCombo2();

private:
	void InitParamers();
	//��ʼ���Զ���ؼ�
	void InitGridCtrl();
	//����ÿ�����ӵĸ�
	void SetCellHight();
	void LoadGridData();
	MultiRowText getDefaultGridData(int index);
	MultiRowText getTableData();
	void loadDaoBing();

public:
	MultiRowText m_allStepData;

	CThreadData m_data;
	CString m_DaoBingName;
	CComboBox m_DaoBingCtrl;
	double m_totalLength;
	double m_VertexAngle;
	CComboBox m_StepNumCtrl;
	CGridCtrlEx m_ZcDjJTInfoCtrl;

	CThreadSegData m_segdata;
	
	CComboBox m_tukuang;		//ͼ��
	CComboBox m_proportion;		//����
	CString m_designer;			//�����Ա
	CString m_degineDate;		//�������
	CString m_knifeCLass;		//�������
	CString m_SapNum;			//SAP��
};
