// DlgPcdZt.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPcdZt.h"
#include "afxdialogex.h"


// CDlgPcdZt 对话框

IMPLEMENT_DYNAMIC(CDlgPcdZt, CDialogEx)

CDlgPcdZt::CDlgPcdZt(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PCD_ZT, pParent)
	, m_totalLen(0)
	, m_VertexAngle(0)
{

}

CDlgPcdZt::~CDlgPcdZt()
{
}

BOOL CDlgPcdZt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int defaultindex = 0;
	m_StepNumCtrl.AddString(L"1");
	m_StepNumCtrl.AddString(L"2");
	m_StepNumCtrl.AddString(L"3");
	m_StepNumCtrl.AddString(L"4");
	m_StepNumCtrl.SetCurSel(defaultindex);

	//设置默认顶角
	m_VertexAngle = 140;
	//设置默认悬伸长度
	m_totalLen = 160;
	//设置刀柄
	vector<CString> daobings = GetAllDwgFile(TY_GetDaoBingFolder());
	for (auto&& d : daobings)
	{
		m_ui_HandleCtrl.AddString(d);
	}
	//m_ui_HandleCtrl.AddString(L"Z06");
	//m_ui_HandleCtrl.AddString(L"Z08");
	//m_ui_HandleCtrl.AddString(L"Z10");
	//m_ui_HandleCtrl.AddString(L"Z12");
	//m_ui_HandleCtrl.AddString(L"Z16");
	//m_ui_HandleCtrl.AddString(L"Z18");
	//m_ui_HandleCtrl.AddString(L"Z20");
	//m_ui_HandleCtrl.AddString(L"Z25");
	//m_ui_HandleCtrl.AddString(L"Z32");
	m_ui_HandleCtrl.SetCurSel(defaultindex);

	InitGridCtrl();
	UpdateData(FALSE);
	return TRUE;
}

LRESULT CDlgPcdZt::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

void CDlgPcdZt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PCD_ZT, m_gridctrl);
	DDX_Control(pDX, IDC_COMBO1, m_ui_HandleCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_totalLen);
	DDX_Control(pDX, IDC_COMBO2, m_StepNumCtrl);
	DDX_Text(pDX, IDC_EDIT2, m_VertexAngle);
}

void CDlgPcdZt::InitGridCtrl()
{
	m_gridctrl.LoadDefaltSettings();
	m_gridctrl.SetEditable(TRUE);
	m_gridctrl.SetRowCount((int)m_allrowData.size() + 1);
	m_gridctrl.SetHeaderText(L"阶梯号;直径D/mm;阶梯长度L/mm;阶梯角A/°");
	m_gridctrl.SetHeaderWidth(L"25;25;25;25");
	m_gridctrl.SetFixedRowCount(1);

	loadGridData();
	if (m_allrowData.size() != 0)
	{
		SetCellHight();
	}
}


void CDlgPcdZt::loadGridData()
{
	if (m_allrowData.size() > 0) {
		m_gridctrl.SetContentItemEditable((int)m_allrowData.size() - 1, (int)m_allrowData[m_allrowData.size() - 1].size() - 2, true);
		m_gridctrl.SetContentItemEditable((int)m_allrowData.size() - 1, (int)m_allrowData[m_allrowData.size() - 1].size() - 1, true);
	}
	int index = m_StepNumCtrl.GetCurSel();
	m_allrowData = getDefaultData(index);
	m_gridctrl.FillTable(m_allrowData);
	m_gridctrl.SetContentItemEditable((int)m_allrowData.size() - 1, (int)m_allrowData[m_allrowData.size() - 1].size() - 2, false);
	m_gridctrl.SetContentItemEditable((int)m_allrowData.size() - 1, (int)m_allrowData[m_allrowData.size() - 1].size() - 1, false);
	m_gridctrl.Refresh();
}

MultiRowText CDlgPcdZt::getTableData()
{
	MultiRowText vec;
	for (int i = 0; i < (int)m_gridctrl.GetContentRowCount(); i++)
	{
		OneRowText oneRowText;
		for (int j = 0; j < 4; j++)
		{
			oneRowText.push_back(m_gridctrl.GetContentItemText(i, j));
		}
		vec.push_back(std::move(oneRowText));
	}
	return vec;
}
MultiRowText CDlgPcdZt::getDefaultData(int rowCount)
{
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
			str.Format(L"26");
			defaultText.push_back(str);
			str.Format(L"0");
			defaultText.push_back(str);
			str.Format(L"0");
			defaultText.push_back(str);
			vec.push_back(defaultText);
			break;
		case 1:
			defaultText.clear();
			str.Format(L"1");
			defaultText.push_back(str);
			str.Format(L"26");
			defaultText.push_back(str);
			str.Format(L"22");
			defaultText.push_back(str);
			str.Format(L"90");
			defaultText.push_back(str);
			vec.push_back(defaultText);

			defaultText.clear();
			str.Format(L"2");
			defaultText.push_back(str);
			str.Format(L"32");
			defaultText.push_back(str);
			str.Format(L"0");
			defaultText.push_back(str);
			str.Format(L"0");
			defaultText.push_back(str);
			vec.push_back(defaultText);
			break;
		case 2:
			defaultText.clear();
			str.Format(L"1");
			defaultText.push_back(str);
			str.Format(L"26");
			defaultText.push_back(str);
			str.Format(L"22");
			defaultText.push_back(str);
			str.Format(L"90");
			defaultText.push_back(str);
			vec.push_back(defaultText);

			defaultText.clear();
			str.Format(L"2");
			defaultText.push_back(str);
			str.Format(L"32");
			defaultText.push_back(str);
			str.Format(L"45");
			defaultText.push_back(str);
			str.Format(L"90");
			defaultText.push_back(str);
			vec.push_back(defaultText);

			defaultText.clear();
			str.Format(L"3");
			defaultText.push_back(str);
			str.Format(L"38");
			defaultText.push_back(str);
			str.Format(L"0");
			defaultText.push_back(str);
			str.Format(L"0");
			defaultText.push_back(str);
			vec.push_back(defaultText);
			break;
		case 3:
			defaultText.clear();
			str.Format(L"1");
			defaultText.push_back(str);
			str.Format(L"26");
			defaultText.push_back(str);
			str.Format(L"22");
			defaultText.push_back(str);
			str.Format(L"90");
			defaultText.push_back(str);
			vec.push_back(defaultText);

			defaultText.clear();
			str.Format(L"2");
			defaultText.push_back(str);
			str.Format(L"32");
			defaultText.push_back(str);
			str.Format(L"45");
			defaultText.push_back(str);
			str.Format(L"90");
			defaultText.push_back(str);
			vec.push_back(defaultText);

			defaultText.clear();
			str.Format(L"3");
			defaultText.push_back(str);
			str.Format(L"38");
			defaultText.push_back(str);
			str.Format(L"70");
			defaultText.push_back(str);
			str.Format(L"90");
			defaultText.push_back(str);
			vec.push_back(defaultText);

			defaultText.clear();
			str.Format(L"4");
			defaultText.push_back(str);
			str.Format(L"44");
			defaultText.push_back(str);
			str.Format(L"0");
			defaultText.push_back(str);
			str.Format(L"0");
			defaultText.push_back(str);
			vec.push_back(defaultText);
			break;
		default:
			break;
		}
		return vec;
	}
}
void CDlgPcdZt::SetCellHight()
{
	for (int i = 0; i < m_allrowData.size(); ++i)
	{
		m_gridctrl.SetRowHeight(i, 30);
	}
}
BEGIN_MESSAGE_MAP(CDlgPcdZt, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS,&CDlgPcdZt::OnAcadKeepFocus)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDlgPcdZt::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgPcdZt::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgPcdZt 消息处理程序


void CDlgPcdZt::OnCbnSelchangeCombo2()
{
	loadGridData();
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgPcdZt::OnBnClickedButton1()
{
	UpdateData(TRUE);
	//设置刀柄
	int sel = m_ui_HandleCtrl.GetCurSel();
	CString temp;
	m_ui_HandleCtrl.GetLBText(sel, temp);
	m_pcdZtData.SetHandleName(temp);
	//加载阶梯数据
	MultiRowText alldata = getTableData();
	for (int i=0; i < alldata.size();++i)
	{
		OneRowText oneRowData = alldata[i];
		ZtSegData onedata;
		onedata.diameter =  _ttof(oneRowData[1]);
		onedata.len = _ttof(oneRowData[2]);
		onedata.angle = i ==0 ? m_VertexAngle : _ttof(alldata[i-1][3]);
		m_pcdZtData.AddSegData(std::move(onedata));
	}
	//设置悬伸长度
	m_pcdZtData.SetTotalLen(m_totalLen);
	ShowWindow(SW_HIDE);
	m_pcdZtData.Draw();
	CDialogEx::OnOK();
}
