// PrintDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PrintDlg.h"
#include "afxdialogex.h"


// CPrintDlg �Ի���

IMPLEMENT_DYNAMIC(CPrintDlg, CDialogEx)

CPrintDlg::CPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PRINTER, pParent)
{

}

CPrintDlg::~CPrintDlg()
{
}

LRESULT CPrintDlg::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

void CPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPrintDlg, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS,&CPrintDlg::OnAcadKeepFocus)
END_MESSAGE_MAP()


// CPrintDlg ��Ϣ�������
