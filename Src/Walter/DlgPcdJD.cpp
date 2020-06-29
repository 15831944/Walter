// DlgPcdJD.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPcdJD.h"
#include "afxdialogex.h"



// DlgPcdJD 对话框

IMPLEMENT_DYNAMIC(DlgPcdJD, CDialogEx)

DlgPcdJD::DlgPcdJD(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgPcdJD::IDD, pParent)
{

}

DlgPcdJD::~DlgPcdJD()
{
}

void DlgPcdJD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_gridCtrl);
	DDX_Control(pDX, IDC_COMBO_STEP_NUM, m_ui_stepNum);
	DDX_Control(pDX, IDC_COMBO_HILT_CHOOSE, m_ui_hiltChoose);
}


BEGIN_MESSAGE_MAP(DlgPcdJD, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE_DWG, &DlgPcdJD::OnBnClickedButtonGenerateDwg)
	ON_CBN_SELCHANGE(IDC_COMBO_STEP_NUM, &DlgPcdJD::OnCbnSelchangeComboStepNum)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &DlgPcdJD::OnAcadKeepFocus)
END_MESSAGE_MAP()

LRESULT DlgPcdJD::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

// DlgPcdJD 消息处理程序


BOOL DlgPcdJD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_gridCtrl.SetDefaultFormat(1);

	m_ui_stepNum.AddString(L"1");
	m_ui_stepNum.AddString(L"2");
	m_ui_stepNum.AddString(L"3");
	m_ui_stepNum.AddString(L"4");

	m_ui_hiltChoose.AddString(L"Z16");
	m_ui_hiltChoose.AddString(L"Z20");
	m_ui_hiltChoose.AddString(L"Z25");
	m_ui_hiltChoose.AddString(L"Z32");
	m_ui_hiltChoose.AddString(L"Z40");
	m_ui_hiltChoose.AddString(L"W16");
	m_ui_hiltChoose.AddString(L"W20");
	m_ui_hiltChoose.AddString(L"W25");
	m_ui_hiltChoose.AddString(L"W32");
	m_ui_hiltChoose.AddString(L"W40");
	m_ui_hiltChoose.AddString(L"BT30");
	m_ui_hiltChoose.AddString(L"BT40");
	m_ui_hiltChoose.AddString(L"BT50");
	m_ui_hiltChoose.AddString(L"JT30");
	m_ui_hiltChoose.AddString(L"JT40");
	m_ui_hiltChoose.AddString(L"JT50");
	m_ui_hiltChoose.AddString(L"HSK-A40");
	m_ui_hiltChoose.AddString(L"HSK-A50");
	m_ui_hiltChoose.AddString(L"HSK-A63");
	m_ui_hiltChoose.AddString(L"HSK-A80");
	m_ui_hiltChoose.AddString(L"HSK-A100");

	int defaultValue1 = 0, defaultValue2 = 2;
	int tableColumnCount = 3;
	m_ui_hiltChoose.SetCurSel(defaultValue1);
	m_ui_stepNum.SetCurSel(defaultValue2);
	m_allListData = FillDefaultData(defaultValue2, tableColumnCount);
	
	InitGridCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void DlgPcdJD::OnBnClickedButtonGenerateDwg()
{
	CString indexText;
	int index;
	index = ((CComboBox*)GetDlgItem(IDC_COMBO_HILT_CHOOSE))->GetCurSel();
	if (index != -1)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_HILT_CHOOSE))->GetLBText(index, indexText);
	}
	jdData.m_uiData_hiltChoose = indexText;
	index = ((CComboBox*)GetDlgItem(IDC_COMBO_STEP_NUM))->GetCurSel();
	if (index != -1)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_STEP_NUM))->GetLBText(index, indexText);
	}
	jdData.m_uiData_stepNumChoose = indexText;

	jdData.m_uiData_allListData = GetTable();
	return;
}



void DlgPcdJD::OnCbnSelchangeComboStepNum()
{
	int tableColumnCount = 3;
	int index;
	index = ((CComboBox*)GetDlgItem(IDC_COMBO_STEP_NUM))->GetCurSel();
	m_gridCtrl.SetEditable(TRUE);

	switch (index)
	{
	case 0:
		m_gridCtrl.SetRowCount(index + 2);
		m_allListData = FillDefaultData(index, tableColumnCount);
		m_gridCtrl.FillTable(m_allListData);
		break;
	case 1:
		m_gridCtrl.SetRowCount(index + 2);
		m_allListData = FillDefaultData(index, tableColumnCount);
		m_gridCtrl.FillTable(m_allListData);
		break;
	case 2:
		m_gridCtrl.SetRowCount(index + 2);
		m_allListData = FillDefaultData(index, tableColumnCount);
		m_gridCtrl.FillTable(m_allListData);
		break;
	case 3:
		m_gridCtrl.SetRowCount(index + 2);
		m_allListData = FillDefaultData(index, tableColumnCount);
		m_gridCtrl.FillTable(m_allListData);
		break;
	default:
		break;
	}

	m_gridCtrl.Refresh();
}

void DlgPcdJD::InitGridCtrl()
{
	m_gridCtrl.LoadDefaltSettings();
	m_gridCtrl.SetEditable(TRUE);
	m_gridCtrl.SetRowCount((int)m_allListData.size() + 1);
	m_gridCtrl.SetHeaderText(L"阶梯号;直径D/mm;阶梯长度L/mm;阶梯主偏角A/°");
	m_gridCtrl.SetHeaderWidth(L"25;25;25;25");
	m_gridCtrl.SetFixedRowCount(1);

	if (m_allListData.size() != 0)
	{
		m_gridCtrl.FillTable(m_allListData);
	}
}


MultiRowData DlgPcdJD::GetTable()
{
	MultiRowText vec;
	for (UINT i = 0; i < (int)m_gridCtrl.GetContentRowCount(); i++)
	{
		OneRowText oneRowText;
		for (UINT j = 0; j < 3;j++)
		{
			oneRowText.push_back(m_gridCtrl.GetContentItemText(i, j));
		}
		vec.push_back(oneRowText);
	}
	return vec;
}

MultiRowData DlgPcdJD::FillDefaultData(int rowCount, int columnCount)
{
	MultiRowData vec;
	for (int i = 0; i <= rowCount; i++)
	{
		OneRowText defaultText;
		CString str;
		for (int j = 0; j <= columnCount; j++)
		{
			switch (j)
			{
			case 0:
				str.Format(_T("%d"), i+1);
				defaultText.push_back(str);
				break;
			case 1:
				defaultText.push_back(L"10");
				break;
			case 2:
				defaultText.push_back(L"20");
				break;
			case 3:
				defaultText.push_back(L"3");
				break;
			default:
				break;
			}
		}
		vec.push_back(defaultText);
	}
	return vec;
}