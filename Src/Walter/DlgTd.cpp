// DlgTd.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTd.h"
#include "afxdialogex.h"


// CDlgTd 对话框

IMPLEMENT_DYNAMIC(CDlgTd, CDialogEx)

CDlgTd::CDlgTd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TD, pParent)
	, m_totallength(0)
{

}

CDlgTd::~CDlgTd()
{
}

BOOL CDlgTd::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Init();
	InitGrid();
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

	DDX_Control(pDX, IDC_COMBO2, m_SegNumCtrl);
	DDX_Text(pDX, IDC_EDIT2, m_totallength);
	DDX_Control(pDX, IDC_COMBO1, m_ui_HandleCtrl);
}


BEGIN_MESSAGE_MAP(CDlgTd, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS,&CDlgTd::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTd::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgTd::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CDlgTd 消息处理程序


MultiRowText CDlgTd::GetTableData()
{

	MultiRowText vec;
	for (int i = 0; i < (int)m_tdSegDataCtrl.GetContentRowCount(); i++)
	{
		OneRowText oneRowText;
		for (int j = 0; j < 4; j++)
		{
			oneRowText.push_back(m_tdSegDataCtrl.GetContentItemText(i, j));
		}
		vec.push_back(std::move(oneRowText));
	}
	return vec;
}

MultiRowText CDlgTd::FillDefaultData(int index)
{
	OneRowText defaultText;
	MultiRowText vec;
	CString str;
	switch (index)
	{
	case 0:
		defaultText.clear();
		str.Format(L"1");
		defaultText.push_back(str);
		str.Format(L"50");
		defaultText.push_back(str);
		str.Format(L"0");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 1:
		defaultText.clear();
		str.Format(L"1");
		defaultText.push_back(str);
		str.Format(L"50");
		defaultText.push_back(str);
		str.Format(L"15");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"2");
		defaultText.push_back(str);
		str.Format(L"60");
		defaultText.push_back(str);
		str.Format(L"30");
		defaultText.push_back(str);
		str.Format(L"45");
		defaultText.push_back(str);
		vec.push_back(defaultText);


		break;
	case 2:
		defaultText.clear();
		str.Format(L"1");
		defaultText.push_back(str);
		str.Format(L"50");
		defaultText.push_back(str);
		str.Format(L"15");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"2");
		defaultText.push_back(str);
		str.Format(L"60");
		defaultText.push_back(str);
		str.Format(L"30");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"3");
		defaultText.push_back(str);
		str.Format(L"70");
		defaultText.push_back(str);
		str.Format(L"0");
		defaultText.push_back(str);
		str.Format(L"45");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		break;
	case 3:
		defaultText.clear();
		str.Format(L"1");
		defaultText.push_back(str);
		str.Format(L"50");
		defaultText.push_back(str);
		str.Format(L"15");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"2");
		defaultText.push_back(str);
		str.Format(L"60");
		defaultText.push_back(str);
		str.Format(L"30");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"3");
		defaultText.push_back(str);
		str.Format(L"70");
		defaultText.push_back(str);
		str.Format(L"45");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"4");
		defaultText.push_back(str);
		str.Format(L"80");
		defaultText.push_back(str);
		str.Format(L"0");
		defaultText.push_back(str);
		str.Format(L"45");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 4: 
		defaultText.clear();
		str.Format(L"1");
		defaultText.push_back(str);
		str.Format(L"50");
		defaultText.push_back(str);
		str.Format(L"15");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"2");
		defaultText.push_back(str);
		str.Format(L"60");
		defaultText.push_back(str);
		str.Format(L"30");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"3");
		defaultText.push_back(str);
		str.Format(L"70");
		defaultText.push_back(str);
		str.Format(L"45");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"4");
		defaultText.push_back(str);
		str.Format(L"80");
		defaultText.push_back(str);
		str.Format(L"63");
		defaultText.push_back(str);
		str.Format(L"85");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"5");
		defaultText.push_back(str);
		str.Format(L"86");
		defaultText.push_back(str);
		str.Format(L"0");
		defaultText.push_back(str);
		str.Format(L"45");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	default:
		break;
	}
	return vec;
}

void CDlgTd::Init()
{
	//悬伸长度
	m_totallength = 200;
	int defalutSel = 0;
	//设置阶段数量
	for (int i=1;i < 6;i++)
	{
		CString temp;
		temp.Format(L"%d", i);
		m_SegNumCtrl.AddString(std::move(temp));
	}
	m_SegNumCtrl.SetCurSel(defalutSel);
	//设置刀柄
	vector<CString> handles = GetAllDwgFile(TY_GetDaoBingFolder());
	for (auto && handleName : handles)
	{
		m_ui_HandleCtrl.AddString(handleName);
	}
	m_ui_HandleCtrl.SetCurSel(defalutSel);

	m_tdAllSegData = FillDefaultData(defalutSel);
	UpdateData(FALSE);
}

void CDlgTd::InitGrid()
{
	m_tdSegDataCtrl.LoadDefaltSettings();
	m_tdSegDataCtrl.SetEditable(TRUE);
	m_tdSegDataCtrl.SetRowCount((int)m_tdAllSegData.size() + 1);
	m_tdSegDataCtrl.SetHeaderText(L"阶梯号;直径D/mm;阶梯长度L/mm;阶梯主偏角A/°");
	m_tdSegDataCtrl.SetHeaderWidth(L"25;25;25;25");
	m_tdSegDataCtrl.SetFixedRowCount(1);

	if (m_tdAllSegData.size() != 0)
	{
		SetGridHeight();
		m_tdSegDataCtrl.FillTable(m_tdAllSegData);
		m_tdSegDataCtrl.SetContentItemEditable(m_tdAllSegData.size() - 1, m_tdAllSegData[0].size() - 2, false);
	}
}

void CDlgTd::SetGridHeight()
{

	for (int i = 1; i <= m_tdAllSegData.size(); i++)
	{
		m_tdSegDataCtrl.SetRowHeight(i, 25);
	}
}

void CDlgTd::OnBnClickedButton1()
{
	UpdateData(TRUE);

	m_tdAllSegData = GetTableData();
	for (auto && data : m_tdAllSegData)
	{
		TDSegData segdata;
		segdata.m_angle = _ttof(data[3]);
		segdata.m_diameter = _ttof(data[1]);
		segdata.m_len = _ttoi(data[2]) == 0 ? m_totallength : _ttoi(data[2]);
		m_tdData.AddSegData(std::move(segdata));
	}

	m_tdData.SetLabberCount(m_labberCount);
	//悬伸长度
	m_tdData.SetTotalLen(m_totallength);
	//刀柄名称
	int sel = m_ui_HandleCtrl.GetCurSel();
	CString temp;
	m_ui_HandleCtrl.GetLBText(sel, temp);
	m_tdData.SetHandleName(std::move(temp));

	ShowWindow(SW_HIDE);
	m_tdData.Draw();
}


void CDlgTd::OnCbnSelchangeCombo2()
{
	if (m_tdAllSegData.size() > 0) {
		m_tdSegDataCtrl.SetContentItemEditable(m_tdAllSegData.size() - 1, m_tdAllSegData[0].size() - 2, true);
	}
	// TODO: 在此添加控件通知处理程序代码
	int sel = m_SegNumCtrl.GetCurSel();
	m_labberCount = sel + 1;
	m_tdAllSegData = FillDefaultData(sel);
	m_tdSegDataCtrl.FillTable(m_tdAllSegData);
	m_tdSegDataCtrl.SetContentItemEditable(m_tdAllSegData.size() - 1, m_tdAllSegData[0].size() - 2, true);
	m_tdSegDataCtrl.Refresh();
}
