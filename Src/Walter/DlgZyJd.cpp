// DlgZyJd.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgZyJd.h"
#include "afxdialogex.h"


// CDlgZyJd 对话框

IMPLEMENT_DYNAMIC(CDlgZyJd, CDialogEx)

CDlgZyJd::CDlgZyJd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PCD_XD, pParent)
	, m_diameter(0)
	, m_totalLength(0)
{

}

BOOL CDlgZyJd::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitDefaultPara();
	InitGrid();
	return TRUE;
}

CDlgZyJd::~CDlgZyJd()
{
}

void CDlgZyJd::InitDefaultPara()
{
	UpdateData(TRUE);
	m_diameter = 8.0f;
	m_totalLength = 113.0f;
	//默认为3
	int defaultSel = 3;
	m_ui_LabberCtrl.AddString(L"1");
	m_ui_LabberCtrl.AddString(L"2");
	m_ui_LabberCtrl.AddString(L"3");
	m_ui_LabberCtrl.AddString(L"4");
	m_ui_LabberCtrl.SetCurSel(defaultSel);

	//刀柄
	//m_ui_DbCtrl.AddString(L"Z03");
	//m_ui_DbCtrl.AddString(L"Z06");
	//m_ui_DbCtrl.AddString(L"Z08");
	//m_ui_DbCtrl.AddString(L"Z10");
	//m_ui_DbCtrl.AddString(L"Z12");
	//m_ui_DbCtrl.AddString(L"Z16");
	//m_ui_DbCtrl.AddString(L"Z18");
	//m_ui_DbCtrl.AddString(L"Z20");
	//m_ui_DbCtrl.AddString(L"Z25");
	//m_ui_DbCtrl.AddString(L"Z32");
	vector<CString> dwgfiles = GetAllDwgFile(TY_GetDaoBingZyFolder());
	for (int i = 0; i < dwgfiles.size(); i++)
	{
		m_ui_DbCtrl.AddString(dwgfiles[i]);
	}

	m_ui_DbCtrl.SetCurSel(0);
	//刀具设置
	m_SelKnifeClass.AddString(L"整硬铰刀");
	m_SelKnifeClass.AddString(L"整硬扩孔刀");
	m_SelKnifeClass.SetCurSel(1);
	m_ZyDjData.SetKKD(true);
	//总长

	UpdateData(FALSE);
}

void CDlgZyJd::InitGrid()
{
	
	m_XdLabberDataCtrl.LoadDefaltSettings();
	m_XdLabberDataCtrl.SetEditable(TRUE);
	m_XdLabberDataCtrl.SetRowCount((int)m_allrowData.size() + 1);
	m_XdLabberDataCtrl.SetHeaderText(L"阶梯号;直径D/mm;阶梯长度L/mm;阶梯主偏角A/°");
	m_XdLabberDataCtrl.SetHeaderWidth(L"25;25;25;25");
	m_XdLabberDataCtrl.SetFixedRowCount(1);
	SetGridData();
	if (m_allrowData.size() != 0)
	{
		SetCellHight();
	
	}
}

void CDlgZyJd::SetCellHight()
{
	for (size_t i=0;i < m_allrowData.size() ; ++i)
	{
		m_XdLabberDataCtrl.SetRowHeight(i, 30);
	}
}

MultiRowText CDlgZyJd::GetDefaultGridData(int rowCount)
{
	OneRowText defaultText;
	MultiRowText vec;
	CString str;
	switch (rowCount)
	{
	case 0:
		defaultText.clear();
		str.Format(L"1");
		defaultText.push_back(str);
		str.Format(L"20");
		defaultText.push_back(str);
		str.Format(L"20");
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 1:
		defaultText.clear();
		str.Format(L"1");
		defaultText.push_back(str);
		str.Format(L"20");
		defaultText.push_back(str);
		str.Format(L"20");
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"2");
		defaultText.push_back(str);
		str.Format(L"26");
		defaultText.push_back(str);
		str.Format(L"40");
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 2:
		defaultText.clear();
		str.Format(L"1");
		defaultText.push_back(str);
		str.Format(L"20");
		defaultText.push_back(str);
		str.Format(L"20");
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"2");
		defaultText.push_back(str);
		str.Format(L"26");
		defaultText.push_back(str);
		str.Format(L"40");
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"3");
		defaultText.push_back(str);
		str.Format(L"32");
		defaultText.push_back(str);
		str.Format(L"60");
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 3:
		defaultText.clear();
		str.Format(L"1");
		defaultText.push_back(str);
		str.Format(L"20");
		defaultText.push_back(str);
		str.Format(L"20");
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"2");
		defaultText.push_back(str);
		str.Format(L"26");
		defaultText.push_back(str);
		str.Format(L"40");
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"3");
		defaultText.push_back(str);
		str.Format(L"32");
		defaultText.push_back(str);
		str.Format(L"60");
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"4");
		defaultText.push_back(str);
		str.Format(L"38");
		defaultText.push_back(str);
		str.Format(L"80");
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	default:
		break;
	}
	return vec;
}

void CDlgZyJd::SetGridData()
{
	m_allrowData = GetDefaultGridData(3);
	m_XdLabberDataCtrl.FillTable(m_allrowData);
}

void CDlgZyJd::ReloadGridData()
{
	int selectIndex = m_ui_LabberCtrl.GetCurSel();
	m_allrowData = GetDefaultGridData(selectIndex);
	m_XdLabberDataCtrl.FillTable(m_allrowData);
}

MultiRowText CDlgZyJd::GetGridData()
{
	
	MultiRowText nRowdata;
	for (size_t i = 0; i < m_XdLabberDataCtrl.GetRowCount();i++ )
	{
		OneRowText rowText;
		for (size_t j=0;j < m_XdLabberDataCtrl.GetColumnCount(); ++j)
		{
			rowText.push_back(m_XdLabberDataCtrl.GetContentItemText(i, j));
		}
		nRowdata.push_back(rowText);
	}
	return nRowdata;
}

void CDlgZyJd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ui_DbCtrl);
	DDX_Control(pDX, IDC_COMBO2, m_ui_LabberCtrl);
	DDX_Control(pDX, IDC_CUSTOM1_XD, m_XdLabberDataCtrl);
	DDX_Text(pDX, IDC_KKD_DIA, m_diameter);
	DDX_Control(pDX, IDC_COMBO3, m_SelKnifeClass);
	DDX_Text(pDX, IDC_EDIT2, m_totalLength);
}

LRESULT CDlgZyJd::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}


BEGIN_MESSAGE_MAP(CDlgZyJd, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CDlgZyJd::OnAcadKeepFocus)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgZyJd::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgZyJd::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgZyJd::OnCbnSelchangeCombo3)
END_MESSAGE_MAP()


// CDlgZyJd 消息处理程序


void CDlgZyJd::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	ReloadGridData();
}


void CDlgZyJd::OnBnClickedButton1()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	//设置阶梯数量
	m_ZyDjData.SetTotalLength(m_totalLength);
	CString labberCountStr;
	int selectIndex = m_ui_LabberCtrl.GetCurSel();
	m_ui_LabberCtrl.GetLBText(selectIndex, labberCountStr);
	m_ZyDjData.SetLabberCount(_ttoi(labberCountStr));
	// 设置刀柄
	CString Daobing;
	selectIndex = m_ui_DbCtrl.GetCurSel();
	m_ui_DbCtrl.GetLBText(selectIndex, Daobing);
	m_ZyDjData.SetDaobing(Daobing);
	//设置数据
	m_allrowData = GetGridData();
	vector<ZYXDStepData> allStepData;
	for (size_t i = 0; i < _ttoi(labberCountStr); i++)
	{
		ZYXDStepData oneStepData;
		oneStepData.index = _ttof(m_allrowData[i][0]);
		oneStepData.m_diameter = _ttof(m_allrowData[i][1]);
		oneStepData.m_stepLength = _ttof(m_allrowData[i][2]);
		oneStepData.m_angle = _ttof(m_allrowData[i][3]);
		allStepData.push_back(oneStepData);
	}
	m_ZyDjData.SetStepData(allStepData);
	ShowWindow(SW_HIDE);
	//扩孔刀则需设置预孔直径
	if (m_diameter == 0 && m_ZyDjData.GetIsKKD())
	{
		MessageBox(L"预孔直径不能为0");
	}
	else
	{
		m_ZyDjData.SetPreDiameter(m_diameter);
		//插入图纸
		m_ZyDjData.Draw();
	}
	CDialogEx::OnOK();
}



void CDlgZyJd::OnCbnSelchangeCombo3()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	int selectIndex = m_SelKnifeClass.GetCurSel();
	CString knifeClass;
	m_SelKnifeClass.GetLBText(selectIndex, knifeClass);
	//实现图片的切换
	CStatic* pstatic;
	CBitmap cbitMap;
	pstatic = (CStatic*)GetDlgItem(IDC_STATIC_ZY);
	pstatic->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
	if (knifeClass.Compare(L"整硬扩孔刀") == 0)
	{
		m_ZyDjData.SetKKD(true);
		((CStatic*)GetDlgItem(IDC_KKD_PAR))->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem(IDC_KKD_DIA))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_KKD_STATIC))->ShowWindow(SW_SHOW);
		cbitMap.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP8));

	}
	else
	{
		m_ZyDjData.SetKKD(false);
		//将预孔直径参数隐藏
		((CStatic*)GetDlgItem(IDC_KKD_PAR))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_KKD_DIA))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_KKD_STATIC))->ShowWindow(SW_HIDE);
		cbitMap.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP7));
	}
	pstatic->SetBitmap(cbitMap);
	UpdateData(FALSE);
}
