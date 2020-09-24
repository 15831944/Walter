// DlgPcdXd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PrintDlg.h"
#include "afxdialogex.h"
#include "Com.h"
#include "Interaction/GetInputUtil.h"
// CPrintDlg �Ի���

IMPLEMENT_DYNAMIC(CPrintDlg, CDialogEx)

CPrintDlg::CPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PRINTER, pParent)
	, m_printAll(FALSE)
	, m_SinglePaper(0)
	, m_PaperClass(0)
{
}
CPrintDlg::~CPrintDlg()
{
}

void CPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO1, m_printAll);
	DDX_Radio(pDX, IDC_RADIO3, m_SinglePaper);
	DDX_Radio(pDX, IDC_RADIO5, m_PaperClass);
}

LRESULT CPrintDlg::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;;
}
BOOL CPrintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}
BEGIN_MESSAGE_MAP(CPrintDlg, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CPrintDlg::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &CPrintDlg::OnBnClickedButton1)
END_MESSAGE_MAP()



void CPrintDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);

	//����ֽ������
	switch (m_PaperClass)
	{
	case 0:
		m_printer.SetPaperType(P_A2);
		break; //A2
	case 1:
		m_printer.SetPaperType(P_A3);
		break; //A3
	case 2:
		m_printer.SetPaperType(P_A4);
		break; // A4
	default:
		break;
	}
	//���ô�ӡһ�Ż��Ƕ���
	if (m_SinglePaper == 0) //��ӡ����
	{
		m_printer.SetIsSingle(TRUE);
	}
	else //��ӡ����
	{
		m_printer.SetIsSingle(FALSE);
	}
	vCString tukuanNames = { CString(L"A1"),CString(L"A2"),CString(L"A3"),CString(L"RAHMEN_RAHM-A3Q"),CString(L"RAHMEN_RAHM-A2Q") };
	if(m_printAll == 0) //�����ȫ����ӡ���Ǵ�ӡ����ģ�Ϳռ�
	{
		AcDbBlockTable *pBlkTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

		// ���ģ�Ϳռ�Ŀ���¼
		AcDbBlockTableRecord *pBlkTblRcd = NULL;
		pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
		pBlkTbl->close();

		AcDbExtents extent;
		Acad::ErrorStatus es = extent.addBlockExt(pBlkTblRcd);
		pBlkTblRcd->close();
		m_printer.GetAllPrintRects(false,tukuanNames);
	}
	else //���ִ�ӡӦ�ÿ�ѡ ��ѡ��ʱ����Ҫ���ö�ȡͼ��
	{
		ShowWindow(SW_HIDE);
		m_printer.GetAllPrintRects(true, tukuanNames);
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
		ShowWindow(SW_HIDE);
		//��Ҫʹ�ý�����
		MessageBox(L"��ӡ��ʼ");
		m_printer.SetPlotDevice(L"PDFCreator");
		m_printer.SetPlotStyleSheet(L"WA_LASER.CTB");
		m_printer.SetPlotDevice(L"DWG To PDF.pc3");
		m_printer.ExportToPdf();
	}
	//��󵼳�pdf
	CDialogEx::OnOK();
}




