// DlgPcdXd.cpp : 实现文件
//

#include "stdafx.h"
#include "PrintDlg.h"
#include "afxdialogex.h"
#include "Com.h"
#include "Interaction/GetInputUtil.h"
// CPrintDlg 对话框

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

	//设置纸张类型
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
	//设置打印一张还是多张
	if (m_SinglePaper == 0) //打印单张
	{
		m_printer.SetIsSingle(TRUE);
	}
	else //打印多张
	{
		m_printer.SetIsSingle(FALSE);
	}
	if(m_printAll == 0) //如果是全部打印就是打印整个模型空间
	{
		AcDbBlockTable *pBlkTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);

		// 获得模型空间的块表记录
		AcDbBlockTableRecord *pBlkTblRcd = NULL;
		pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
		pBlkTbl->close();

		AcDbExtents extent;
		Acad::ErrorStatus es = extent.addBlockExt(pBlkTblRcd);
		pBlkTblRcd->close();
		Rect PrintRect;
		PrintRect.LB = extent.minPoint();
		PrintRect.RT = extent.maxPoint();
		m_printer.SetRect(PrintRect);
	}
	else //部分打印应该框选 框选的时候需要设置读取图框
	{
		CDocLock lock;
		ShowWindow(SW_HIDE);
		//直接让用户选择要打印的区域
		AcGePoint3d minPoint;
		AcGePoint3d maxPoint;
		vector<AcDbObjectId> vid;
		CSelectUtil::SelectMany(vid);
		for (int i = 0; i < vid.size(); i++)
		{
			AcDbEntity *pEntity = NULL;
			if (acdbOpenAcDbEntity(pEntity, vid[i], AcDb::kForRead) == Acad::eOk)
			{
				AcDbExtents extent;
				pEntity->getGeomExtents(extent);
				if (i == 0)
				{
					minPoint = extent.minPoint();
					maxPoint = extent.maxPoint();
				}
				else
				{
					minPoint.x = minPoint.x <= extent.minPoint().x ? minPoint.x : extent.minPoint().x;
					minPoint.y = minPoint.y <= extent.minPoint().y ? minPoint.y : extent.minPoint().y;
					maxPoint.x = maxPoint.x >= extent.maxPoint().x ? maxPoint.x : extent.maxPoint().x;
					maxPoint.y = maxPoint.y >= extent.maxPoint().y ? maxPoint.y : extent.maxPoint().y;
				}
			}
		}

		m_printer.SetRect(minPoint, maxPoint);
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
		ShowWindow(SW_HIDE);
		//需要使用进度条
		TY_Progress_Init();
		m_printer.ExportToPdf();
		TY_Progress_Close();
	}
	//最后导出pdf
	CDialogEx::OnOK();
}




