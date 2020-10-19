// DlgPcdXd.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPcdXd.h"
#include "afxdialogex.h"
#include "Com.h"

// CDlgPcdXd 对话框

IMPLEMENT_DYNAMIC(CDlgPcdXd, CDialogEx)

CDlgPcdXd::CDlgPcdXd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2_PCD_XD, pParent)
	, m_DiameterOfKnife(0)
	, m_lengthOfKnife(0)
	, m_BladeLength(0)
{

}

CDlgPcdXd::~CDlgPcdXd()
{
}

void CDlgPcdXd::InitParameters()
{
	m_BladeLength = 30;
	m_DiameterOfKnife = 41;
	vector<CString> daobings = GetAllDwgFile(TY_GetDaoBingFolder());
	for (auto&& daobing : daobings)
	{
		if (daobing.Find(L"GR") == -1)  //去掉法兰刀柄
			m_ui_HandleCtrl.AddString(daobing);
	}
	m_ui_HandleCtrl.SetCurSel(0);
	m_lengthOfKnife = 120;
	UpdateData(FALSE);
}

void CDlgPcdXd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ui_HandleCtrl);
	DDX_Text(pDX, IDC_EDIT2, m_DiameterOfKnife);
	DDX_Text(pDX, IDC_EDIT1, m_lengthOfKnife);
	DDX_Text(pDX, IDC_EDIT3, m_BladeLength);
}

LRESULT CDlgPcdXd::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;;
}
BOOL CDlgPcdXd::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//初始化参数
	InitParameters();


	return TRUE;
}
BEGIN_MESSAGE_MAP(CDlgPcdXd, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CDlgPcdXd::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgPcdXd::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgPcdXd 消息处理程序


void CDlgPcdXd::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if (m_DiameterOfKnife < 5)
	{
		MessageBox(L"最小直径为5mm");
		return;
	}
	m_Data.SetDiameter(m_DiameterOfKnife);
	m_Data.SetBladeLength(m_BladeLength);
	m_Data.SetTotalLength(m_lengthOfKnife);
	int sel = m_ui_HandleCtrl.GetCurSel();
	CString temp;
	m_ui_HandleCtrl.GetLBText(sel, temp);
	m_Data.SetHandleName(temp);
	ShowWindow(SW_HIDE);
	m_Data.Draw();
	// TODO: 在此添加控件通知处理程序代码
}
