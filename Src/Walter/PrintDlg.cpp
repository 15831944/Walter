// DlgPcdXd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PrintDlg.h"
#include "afxdialogex.h"
// CPrintDlg �Ի���

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
	//��ʼ������
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

	//����ֽ������
	if (m_A2)
		m_printer.SetPaperType(P_A2);
	else if (m_A3)
		m_printer.SetPaperType(P_A3);
	else
		m_printer.SetPaperType(P_A4);
	//���Ż��Ƕ��� true ��ʾ���� false��ʾ����
	if (m_SinglePaper)
		m_printer.SetSingle(m_SinglePaper);
	else
		m_printer.SetSingle(false);

	//��ȡ��Χ ��ʱ��δ����
	{

	}
	//
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = m_hWnd;
	TCHAR dirPath[MAX_PATH];
	bInfo.lpszTitle = _T("ѡ��Ҫ������ļ�Ŀ¼");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST lpDlist;
	lpDlist = SHBrowseForFolder(&bInfo);
	if (lpDlist != NULL)
	{
		SHGetPathFromIDList(lpDlist, dirPath);
		m_printer.setDirPath(dirPath);
	}

	ShowWindow(SW_HIDE);
	//��󵼳�pdf
	m_printer.ExportToPdf();
	CDialogEx::OnOK();
}

//��ѡ
void CPrintDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}



