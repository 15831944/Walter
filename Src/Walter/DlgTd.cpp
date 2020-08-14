// DlgTd.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTd.h"
#include "afxdialogex.h"


// CDlgTd 对话框

IMPLEMENT_DYNAMIC(CDlgTd, CDialogEx)

CDlgTd::CDlgTd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TD, pParent)
{

}

CDlgTd::~CDlgTd()
{
}

BOOL CDlgTd::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

LRESULT CDlgTd::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

void CDlgTd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TD, m_tdSegDataCtrl);

}


BEGIN_MESSAGE_MAP(CDlgTd, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS,&CDlgTd::OnAcadKeepFocus)
END_MESSAGE_MAP()


// CDlgTd 消息处理程序
