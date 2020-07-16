// ZTInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZTInfoDlg.h"
#include "afxdialogex.h"


// CZTInfoDlg 对话框

IMPLEMENT_DYNAMIC(CZTInfoDlg, CDialogEx)

CZTInfoDlg::CZTInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CWDR, pParent)
	, m_TotalLength(0)
	, m_VertAngle(0)
	, m_DrNum(0)
{

}

CZTInfoDlg::~CZTInfoDlg()
{
}
BOOL CZTInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitGridCtrl();
	//设置默认数据
	InitDefaultPara();

	LoadGridData();

	return 0;
}
//初始化GridCtrl控件
void CZTInfoDlg::InitGridCtrl()
{
	m_djInfoCtrl.LoadDefaltSettings();
	m_djInfoCtrl.SetEditable(TRUE);
	m_djInfoCtrl.SetRowCount(m_alldjInfos.size() + 1);
	m_djInfoCtrl.SetHeaderText(L"刃径;刃段长度;阶梯角1;阶梯长1");
	m_djInfoCtrl.SetHeaderWidth(L"25;25;25;25");
	m_djInfoCtrl.SetFixedRowCount(1);
	if (m_alldjInfos.size() != 0)
	{
		SetCellHight();
		
	}
}
//设置每个小格的高
void CZTInfoDlg::SetCellHight()
{
	for (int i=1;i <= m_alldjInfos.size();++i)
	{
		m_djInfoCtrl.SetRowHeight(i, 25);
	}
}

void CZTInfoDlg::InitDefaultPara()
{
	UpdateData(TRUE);
	int defaultIndex = 0;
	//刃数
	m_ui_DrNumCtrl.AddString(L"2");
	m_ui_DrNumCtrl.SetCurSel(defaultIndex);
	//顶角
	CString temp;
	m_VertAngle = 140.0;
	temp.Format(L"%.1f", m_VertAngle);

	m_VertexEdit.SetWindowTextW(temp);
	//总长
	m_TotalLength = 200.0;
	temp.Format(L"%.1f", m_TotalLength);
	m_TotalLenEdit.SetWindowTextW(temp);

	m_StepNum.AddString(L"1");
	m_StepNum.AddString(L"2");
	m_StepNum.AddString(L"3");
	m_StepNum.AddString(L"4");
	m_StepNum.SetCurSel(defaultIndex);
	UpdateData();
}
MultiRowData CZTInfoDlg::getDefaultGridData(int index)
{
	MultiRowData vec;
	OneRowData defaultText;
	CString str;
;
	switch (index)
	{
	case 0:
		defaultText.clear();
		str.Format(L"%.3f",10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f",25.0);
		defaultText.push_back(str);
		str.Format(L"");
		defaultText.push_back(str);
		str.Format(L"");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 1:
		defaultText.clear();
		str.Format(L"%.3f",10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f",25.0);
		defaultText.push_back(str);
		str.Format(L"");
		defaultText.push_back(str);
		str.Format(L"");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"130");
		defaultText.push_back(str);
		str.Format(L"3");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 2:
		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 25.0);
		defaultText.push_back(str);
		str.Format(L"");
		defaultText.push_back(str);
		str.Format(L"");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"130");
		defaultText.push_back(str);
		str.Format(L"3");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"130");
		defaultText.push_back(str);
		str.Format(L"3");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 3:
		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 25.0);
		defaultText.push_back(str);
		str.Format(L"");
		defaultText.push_back(str);
		str.Format(L"");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"130");
		defaultText.push_back(str);
		str.Format(L"3");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"130");
		defaultText.push_back(str);
		str.Format(L"3");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		defaultText.clear();
		str.Format(L"%.3f", 10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"130");
		defaultText.push_back(str);
		str.Format(L"3");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	default:
		break;
	}
	
	return vec;
}
//填充默认数据
void CZTInfoDlg::LoadGridData()
{
	int index = m_StepNum.GetCurSel();
	m_alldjInfos = getDefaultGridData(index);
	m_djInfoCtrl.FillTable(m_alldjInfos);
}

void CZTInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_DJINFO, m_djInfoCtrl);
	DDX_Control(pDX, IDC_COMBO_STEPNUM, m_StepNum);
	DDX_Control(pDX, IDC_COMBO_REN_NUM, m_ui_DrNumCtrl);
	DDX_Control(pDX, IDC_COMBO_REN_STYLE, m_ui_DrStyleCtrl);
	DDX_Control(pDX, IDC_EDIT_ALLLENGTH, m_TotalLenEdit);
	DDX_Control(pDX, IDC_EDIT_VERTEX_ANGLE, m_VertexEdit);
}

LRESULT CZTInfoDlg::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;;
}


BEGIN_MESSAGE_MAP(CZTInfoDlg, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CZTInfoDlg::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BTNCANCLE, &CZTInfoDlg::OnBnClickedBtncancle)
	ON_CBN_SELCHANGE(IDC_COMBO_STEPNUM, &CZTInfoDlg::OnCbnSelchangeComboStepnum)
END_MESSAGE_MAP()


// CZTInfoDlg 消息处理程序


void CZTInfoDlg::OnBnClickedBtncancle()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CZTInfoDlg::OnCbnSelchangeComboStepnum()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadGridData();
}
