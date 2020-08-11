#pragma once

#include "GridCtrl/GridCtrlEx.h"
#include "res/resource.h"
#include "afxwin.h"
#include "PCDJDData.h"
#include "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxwin.h"

typedef vector<CString> OneRowData;
typedef vector<OneRowData> MultiRowData;


typedef struct JDData
{
	CString m_uiData_hiltChoose;	//ui���桪������ѡ��
	CString m_uiData_stepNumChoose;		//ui���桪����������ѡ��
	MultiRowData m_uiData_allListData;	//��������������Զ����ɵı�����
}s_JDData;


// DlgPcdJD �Ի���

class DlgPcdJD : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPcdJD)

public:
	DlgPcdJD(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgPcdJD();

// �Ի�������
	enum { IDD = IDD_DIALOG_PCD_JD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	s_JDData jdData;
	CGridCtrlEx m_gridCtrl;
	MultiRowData m_allListData;
	virtual BOOL OnInitDialog();
	CComboBox m_ui_stepNum;		//����ѡ���������
	CComboBox m_ui_hiltChoose;		//����ѡ���������
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonGenerateDwg();
	afx_msg void OnCbnSelchangeComboStepNum();
	void InitGridCtrl();
	void FillTable(MultiRowText data);
	MultiRowData GetTable();
	MultiRowData FillDefaultData(int rowCount, int tableColumn);
	void SetCellHeight();

	SPCDJDData m_data;

	double m_totalLength;
};
