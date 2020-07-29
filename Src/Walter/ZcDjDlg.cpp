// ZcDjDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZcDjDlg.h"
#include "afxdialogex.h"
#include "Com.h"

// CZcDjDlg 对话框

IMPLEMENT_DYNAMIC(CZcDjDlg, CDialogEx)

CZcDjDlg::CZcDjDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DCD, pParent)
	, m_DaoBingName(_T(""))
	, m_totalLength(0)
	, m_VertexAngle(0)
{

}

CZcDjDlg::~CZcDjDlg()
{
}

void CZcDjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ZCDJINFO, m_ZcDjJTInfoCtrl);
	DDX_CBString(pDX, IDC_ZCDJ_COMBOX, m_DaoBingName);
	DDX_Control(pDX, IDC_ZCDJ_COMBOX, m_DaoBingCtrl);
	DDX_Text(pDX, IDC_ZC_EDIT, m_totalLength);
	DDX_Text(pDX, IDC_E, m_VertexAngle);
	DDX_Control(pDX, IDC_COMBO2, m_StepNumCtrl);

}

LRESULT CZcDjDlg::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}


BEGIN_MESSAGE_MAP(CZcDjDlg, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CZcDjDlg::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_ZCDRAW, &CZcDjDlg::OnBnClickedZcdraw)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CZcDjDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CZcDjDlg 消息处理程序


BOOL CZcDjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//设置默认参数
	InitParamers();
	InitGridCtrl();
	LoadGridData();
	//加载刀柄
	loadDaoBing();
	return TRUE;
}


void CZcDjDlg::OnBnClickedZcdraw()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	//读取数据
	m_allStepData = getTableData();
	m_data.ClearCutterSegs();
	for (int i = 0; i < m_allStepData.size(); i++)
	{
		OneRowText RowData = m_allStepData[i];
		m_segdata.m_diameter = _ttof(RowData[0]); //刃径
		m_segdata.m_length = _ttof(RowData[1]); //刃段长度
		m_segdata.m_ladderAngle1 = _ttof(RowData[2]); //阶梯角
	
		m_data.AddCutterSegData(m_segdata);
	}
	//阶梯数量
	m_data.SetLadderCount((int)m_allStepData.size() - 1);
	//顶角
	m_data.m_topAngle = m_VertexAngle;
	//总长
	m_data.m_totalLength = m_totalLength;
	//刃数

	m_data.SetDaoBingName(m_DaoBingName);

	//隐藏窗口
	ShowWindow(SW_HIDE);
	m_data.Draw(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
//初始化默认参数
void CZcDjDlg::InitParamers()
{
	UpdateData(TRUE);


	
	int defaultIndex = 0;
	//顶角
	m_VertexAngle = 140.0;
	////初始化图框
	//m_tukuang.AddString(L"A3");
	//m_tukuang.AddString(L"空");
	//m_tukuang.SetCurSel(defaultIndex);
	////初始化比例
	//m_proportion.AddString(L"1:1");
	//m_proportion.AddString(L"2:1");
	//m_proportion.AddString(L"2:3");
	//m_proportion.AddString(L"空");
	//m_proportion.SetCurSel(defaultIndex);
	//总长
	m_totalLength = 160.0;

	//阶数设置
	m_StepNumCtrl.AddString(L"1");
	m_StepNumCtrl.AddString(L"2");
	m_StepNumCtrl.AddString(L"3");
	m_StepNumCtrl.AddString(L"4");
	m_StepNumCtrl.SetCurSel(defaultIndex);
	UpdateData(FALSE);
}

//初始化MFCGRid控件
void CZcDjDlg::InitGridCtrl()
{
	m_ZcDjJTInfoCtrl.LoadDefaltSettings();
	m_ZcDjJTInfoCtrl.SetEditable(TRUE);
	m_ZcDjJTInfoCtrl.SetRowCount((int)m_allStepData.size() + 1);
	m_ZcDjJTInfoCtrl.SetHeaderText(L"刃径;刃段长度;阶梯角1");
	m_ZcDjJTInfoCtrl.SetHeaderWidth(L"33;33;34");
	m_ZcDjJTInfoCtrl.SetFixedRowCount(1);
	if (m_allStepData.size() != 0)
	{
		SetCellHight();

	}
}
//设置每一个cell的高
void CZcDjDlg::SetCellHight()
{
	for (int i = 1; i <= m_allStepData.size(); ++i)
	{
		m_ZcDjJTInfoCtrl.SetRowHeight(i, 25);
	}
}
//将数据加载到桌面中
void CZcDjDlg::LoadGridData()
{
	int index = m_StepNumCtrl.GetCurSel();
	m_allStepData = getDefaultGridData(index);
	m_ZcDjJTInfoCtrl.FillTable(m_allStepData);
}
//设置默认数据
MultiRowText CZcDjDlg::getDefaultGridData(int index)
{
	m_ZcDjJTInfoCtrl.SetEditable(TRUE);
	MultiRowText vec;
	OneRowText defaultText;
	CString str;
	switch (index)
	{
	case 0:
		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 25.0);
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 1:
		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 25.0);
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 12.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"90");

		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 2:
		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 25.0);
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 12.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 14.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 3:
		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 25.0);
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 12.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 14.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 16.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
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

MultiRowText CZcDjDlg::getTableData()
{
	MultiRowText vec;
	for (int i = 0; i < m_ZcDjJTInfoCtrl.GetContentRowCount(); i++)
	{
		OneRowText oneRowText;
		for (int j = 0; j < m_ZcDjJTInfoCtrl.GetColumnCount(); j++)
		{
			oneRowText.push_back(m_ZcDjJTInfoCtrl.GetContentItemText(i, j));
		}
		vec.push_back(oneRowText);
	}
	return vec;
}

void CZcDjDlg::loadDaoBing()
{
	CString dirpath = TY_GetDaoBingSFolder();
	vector<CString> daobing = GetAllDwgFile(dirpath);
	for (int i = 0; i < daobing.size(); i++)
	{
		m_DaoBingCtrl.AddString(daobing[i]);
	}
	m_DaoBingCtrl.SetCurSel(0);
}


void CZcDjDlg::OnCbnSelchangeCombo2()
{
	LoadGridData();
}
