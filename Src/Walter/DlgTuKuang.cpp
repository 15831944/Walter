// DlgTuKuang.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTuKuang.h"
#include "afxdialogex.h"
#include "Com.h"
#include "Interaction/UpdateUtil.h"
// CDlgTuKuang 对话框

IMPLEMENT_DYNAMIC(CDlgTuKuang, CDialogEx)

CDlgTuKuang::CDlgTuKuang(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TUKUANG, pParent)
	, m_designer(_T(""))
	, m_SapNum(_T(""))
	, m_designDate(_T(""))
{

}

CDlgTuKuang::~CDlgTuKuang()
{
}

BOOL CDlgTuKuang::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Init();
	return 0;
}

void CDlgTuKuang::Init()
{
	int defaultIndex = 0;
	//初始化图框
	m_tukuang.AddString(L"A3");
	m_tukuang.AddString(L"A1");
	m_tukuang.AddString(L"A2");
	m_tukuang.SetCurSel(defaultIndex);
	
	//初始化比例
	m_proportion.AddString(L"1:1");
	m_proportion.AddString(L"2:1");
	m_proportion.AddString(L"2:3");
	m_proportion.SetCurSel(defaultIndex);

	//刀具类别
	m_KnifeClassSel.AddString(L" ");
	m_KnifeClassSel.AddString(L"整硬钻头");
	m_KnifeClassSel.AddString(L"整硬铰刀");
	m_KnifeClassSel.AddString(L"整硬铣刀");
	m_KnifeClassSel.AddString(L"丝锥");
	m_KnifeClassSel.AddString(L"整硬扩孔刀");
	m_KnifeClassSel.AddString(L"焊接硬质合金扩孔刀");
	m_KnifeClassSel.AddString(L"PCD钻头");
	m_KnifeClassSel.AddString(L"PCD铰刀");
	m_KnifeClassSel.AddString(L"PCD铣刀");
	m_KnifeClassSel.AddString(L"可换刀片镗刀");
	m_KnifeClassSel.SetCurSel(defaultIndex);
}

void CDlgTuKuang::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_tukuang);
	DDX_Control(pDX, IDC_COMBO2, m_proportion);
	DDX_Text(pDX, IDC_EDIT3, m_designer);
	DDX_Text(pDX, IDC_EDIT6, m_SapNum);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_designDate);
	DDX_Control(pDX, IDC_COMBO3, m_KnifeClassSel);
}

LRESULT CDlgTuKuang::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}


BEGIN_MESSAGE_MAP(CDlgTuKuang, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CDlgTuKuang::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTuKuang::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgTuKuang 消息处理程序


void CDlgTuKuang::OnBnClickedButton1()
{
	UpdateData(TRUE);
	ShowWindow(SW_HIDE);
	// TODO: 在此添加控件通知处理程序代码
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"请选择一个插入点:", pnt);
	
	//选择图框名称
	try
	{
		CDocLock doclock;
		int selectIndex = m_tukuang.GetCurSel();
		CString TuKuangName;
		m_tukuang.GetLBText(selectIndex, TuKuangName);
		CString TuKuangPath = TY_GetFrameFolder() + TuKuangName + L".dwg";
		//CString blkName = CCommonUtil::GenStrByTime();

		//比例
		CString proportion;
		selectIndex = m_proportion.GetCurSel();
		m_proportion.GetLBText(selectIndex, proportion);
		AcDbObjectId tukuangId;
		if (proportion.Compare(L"2:1") == 0)
			tukuangId = CBlockUtil::InsertDwgAsBlockRef(TuKuangPath, NULL, ACDB_MODEL_SPACE, pnt, 0, 0.5);
		else if(proportion.Compare(L"1:1") == 0)
			tukuangId = CBlockUtil::InsertDwgAsBlockRef(TuKuangPath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);
		else if (proportion.Compare(L"2:3") == 0)
			tukuangId = CBlockUtil::InsertDwgAsBlockRef(TuKuangPath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1.5);
		//设置属性定义
		//类别
		int index = m_KnifeClassSel.GetCurSel();
		CString temp;
		m_KnifeClassSel.GetLBText(index, temp);
		CBlockUtil::SetBlockRefAttribute(tukuangId, L"!ALT_WZG_BEZ4", temp);
		//SAP号
		CBlockUtil::SetBlockRefAttribute(tukuangId, L"MAT_BEZ", m_SapNum);
		//时间
		CBlockUtil::SetBlockRefAttribute(tukuangId, L"DWG_ERST_DATUM", m_designDate);
		//设计人员
		CBlockUtil::SetBlockRefAttribute(tukuangId, L"DWG_ERST_INIT", m_designer);
		//比例
		CBlockUtil::SetBlockRefAttribute(tukuangId, L"MASSTAB", proportion);
		

	}
	catch (exception& e)
	{
		MessageBox(CString(e.what()));
	}


	CDlgTuKuang::OnOK();
}
