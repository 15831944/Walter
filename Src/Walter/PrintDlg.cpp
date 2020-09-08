// DlgPcdXd.cpp : 实现文件
//

#include "stdafx.h"
#include "PrintDlg.h"
#include "afxdialogex.h"
// CPrintDlg 对话框

IMPLEMENT_DYNAMIC(CPrintDlg, CDialogEx)

CPrintDlg::CPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PRINTER, pParent)
	, m_AllBlock(FALSE)
	, m_partBlock(FALSE)
	, m_SinglePaper(FALSE)
	, m_multiPaper(FALSE)
	, m_A3(FALSE)
	, m_A4(FALSE)
	, m_A2(FALSE)
{

}

CPrintDlg::~CPrintDlg()
{
}

void CPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_AllBlock);
	DDX_Radio(pDX, IDC_RADIO2, m_partBlock);
	DDX_Radio(pDX, IDC_RADIO3, m_SinglePaper);
	DDX_Radio(pDX, IDC_RADIO4, m_multiPaper);
	DDX_Radio(pDX, IDC_RADIO5, m_A3);
	DDX_Radio(pDX, IDC_RADIO6, m_A4);
	DDX_Radio(pDX, IDC_RADIO7, m_A2);
}

LRESULT CPrintDlg::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;;
}
BOOL CPrintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//初始化参数
	m_AllBlock = true;
	m_SinglePaper = true;
	m_A2 = true;
	UpdateData(FALSE);
	return TRUE;
}
BEGIN_MESSAGE_MAP(CPrintDlg, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CPrintDlg::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &CPrintDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO2, &CPrintDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()



void CPrintDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);

	//设置纸张类型
	if (m_A2)
		m_printer.SetPaperType(P_A2);
	else if (m_A3)
		m_printer.SetPaperType(P_A3);
	else
		m_printer.SetPaperType(P_A4);
	//单张还是多张 true 表示单张 false表示多张
	if (m_SinglePaper)
		m_printer.SetSingle(m_SinglePaper);
	else
		m_printer.SetSingle(false);

	//读取范围 暂时还未设置
	{

	}
	//
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = m_hWnd;
	TCHAR dirPath[MAX_PATH];
	bInfo.lpszTitle = _T("选择要保存的文件目录");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST lpDlist;
	lpDlist = SHBrowseForFolder(&bInfo);
	if (lpDlist != NULL)
	{
		SHGetPathFromIDList(lpDlist, dirPath);
		m_printer.setDirPath(dirPath);
	}

	ShowWindow(SW_HIDE);
	//最后导出pdf
	m_printer.ExportToPdf();
	CDialogEx::OnOK();
}

//框选
void CPrintDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	
}



