#pragma once
#include "Com.h"
#include "GridCtrl/GridCellCheckEx.h"
#include "TDData.h"
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
	
	
	TDSegData m_tdSegData;
	CTDData m_tdData;
};
