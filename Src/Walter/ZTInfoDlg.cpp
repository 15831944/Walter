// ZTInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZTInfoDlg.h"
#include "afxdialogex.h"
#include "Com.h"

// CZTInfoDlg 对话框

IMPLEMENT_DYNAMIC(CZTInfoDlg, CDialogEx)

CZTInfoDlg::CZTInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CWDR, pParent)
	, m_TotalLength(0)
	, m_VertAngle(0)
	, m_GrooveLenth(0)
{

}

CZTInfoDlg::~CZTInfoDlg()
{
}
BOOL CZTInfoDlg::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();
	InitDefaultPara();
	InitGridCtrl();
	
	
	return TRUE;
}
//初始化GridCtrl控件
void CZTInfoDlg::InitGridCtrl()
{
	m_djInfoCtrl.LoadDefaltSettings();
	m_djInfoCtrl.SetEditable(TRUE);
	m_djInfoCtrl.SetRowCount((int)m_alldjInfos.size() + 1);
	m_djInfoCtrl.SetHeaderText(L"直径/mm;阶梯长度/mm;阶梯角/°");
	m_djInfoCtrl.SetHeaderWidth(L"33;33;34");
	m_djInfoCtrl.SetFixedRowCount(1);
	//设置数据
	UpdateData(TRUE);
	LoadGridData();
	if (m_alldjInfos.size() != 0)
	{
		SetCellHight();
		
	}
	
	UpdateData(FALSE);
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
	int defaultIndex = 0;
	//顶角
	m_VertAngle = 140.0;
	//temp.Format(L"%.1f", m_VertAngle);
	m_DrillSel.AddString(L"麻花钻");
	m_DrillSel.AddString(L"直槽钻");
	m_DrillSel.SetCurSel(defaultIndex);
	CString temp;
	m_DrillSel.GetLBText(defaultIndex, temp);
	// TODO: 在此添加控件通知处理程序代码

	if (temp.Compare(L"麻花钻") == 0)
	{
		GetDlgItem(IDC_CHECK_FORME)->ShowWindow(SW_HIDE);
	}
	
	//m_VertexEdit.SetWindowTextW(temp);
	m_ui_DrNumCtrl.AddString(L"2");
	m_ui_DrNumCtrl.AddString(L"3");
	m_ui_DrNumCtrl.SetCurSel(defaultIndex);
	//总长
	m_TotalLength = 160.0;
	//temp.Format(L"%.1f", m_TotalLength);
	//m_TotalLenEdit.SetWindowTextW(temp);

	m_StepNum.AddString(L"1");
	m_StepNum.AddString(L"2");
	m_StepNum.AddString(L"3");
	m_StepNum.AddString(L"4");
	m_StepNum.SetCurSel(defaultIndex);
	//加载刀柄信息
	m_djInfoCtrl.SetEditable(TRUE);
	vector<CString> dwgfiles = GetAllDwgFile(TY_GetDaoBingZyFolder());
	for (int i = 0; i< dwgfiles.size() ; i++)
	{
		m_DaoBingCtrl.AddString(dwgfiles[i]);
	}
	m_DaoBingCtrl.SetCurSel(defaultIndex);
	UpdateData(FALSE);

}
//初始化刀具阶梯信息
MultiRowData CZTInfoDlg::getDefaultGridData(int index)
{
	
	MultiRowData vec;
	OneRowData defaultText;
	CString str;
	switch (index)
	{
	case 0:
		defaultText.clear();
		str.Format(L"%.3f",10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f",25.0);
		defaultText.push_back(str);
		str.Format(L"0");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	case 1:
		defaultText.clear();
		str.Format(L"%.3f",10.0);
		defaultText.push_back(str);
		str.Format(L"%.3f",25.0);
		defaultText.push_back(str);
		str.Format(L"90");
		defaultText.push_back(str);
		vec.push_back(defaultText);

		defaultText.clear();
		str.Format(L"%.3f", 12.0);
		defaultText.push_back(str);
		str.Format(L"%.3f", 50.0);
		defaultText.push_back(str);
		str.Format(L"0");

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
		str.Format(L"0");
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
		str.Format(L"0");
		defaultText.push_back(str);
		vec.push_back(defaultText);
		break;
	default:
		break;
	}
	
	return vec;
}
//获取表格中的数据
MultiRowData CZTInfoDlg::getTableData()
{
	MultiRowText vec;
	for (int i = 0; i < (int)m_djInfoCtrl.GetContentRowCount(); i++)
	{
		OneRowText oneRowText;
		for (int j = 0; j < m_djInfoCtrl.GetColumnCount(); j++)
		{
			oneRowText.push_back(m_djInfoCtrl.GetContentItemText(i, j));
		}
		vec.push_back(std::move(oneRowText));
	}
	return vec;
}

//填充默认数据
void CZTInfoDlg::LoadGridData()
{
	int index = m_StepNum.GetCurSel();
	//将上一次设置的不可编辑设置为可编辑
	if (m_alldjInfos.size() > 0)
	{
		m_djInfoCtrl.SetContentItemEditable((int)m_alldjInfos.size() - 1,static_cast<int>( m_alldjInfos[0].size()) - 1, true);
	}
	m_alldjInfos = getDefaultGridData(index);
	m_djInfoCtrl.FillTable(m_alldjInfos);
	
	//设置最后一个不可编辑
	m_djInfoCtrl.SetContentItemEditable((int)m_alldjInfos.size() - 1, (int)m_alldjInfos[0].size() - 1, false);
	//CGridCellBase *cell = m_djInfoCtrl.GetCell(m_alldjInfos.size() - 1, m_alldjInfos[0].size() -1)
}

void CZTInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_DJINFO, m_djInfoCtrl);
	DDX_Control(pDX, IDC_COMBO_STEPNUM, m_StepNum);
	DDX_Control(pDX, IDC_COMBO_REN_NUM, m_ui_DrNumCtrl);
	DDX_Control(pDX, IDC_EDIT_ALLLENGTH, m_TotalLenEdit);
	DDX_Control(pDX, IDC_EDIT_VERTEX_ANGLE, m_VertexEdit);
	DDX_Text(pDX, IDC_EDIT_ALLLENGTH, m_TotalLength);
	DDX_Text(pDX, IDC_EDIT_VERTEX_ANGLE, m_VertAngle);
	DDX_Control(pDX, IDC_COMBO1_DJ, m_DaoBingCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_DrillSel);
}

LRESULT CZTInfoDlg::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;;
}


BEGIN_MESSAGE_MAP(CZTInfoDlg, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CZTInfoDlg::OnAcadKeepFocus)
	ON_CBN_SELCHANGE(IDC_COMBO_STEPNUM, &CZTInfoDlg::OnCbnSelchangeComboStepnum)
	ON_BN_CLICKED(IDC_BTNOK, &CZTInfoDlg::OnBnClickedBtnok)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CZTInfoDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CZTInfoDlg 消息处理程序

void CZTInfoDlg::OnCbnSelchangeComboStepnum()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	LoadGridData();
	UpdateData(FALSE);
}


void CZTInfoDlg::OnBnClickedBtnok()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	//读取数据
	m_alldjInfos = getTableData();
	m_data.ClearCutterSegs();
	auto DrLength = [&](int i) 
	{
		return _ttof(m_alldjInfos[i][1]) - _ttof(m_alldjInfos[i-1][1]);
	 };
	//double sum;
	for (int i=0;i < m_alldjInfos.size();i++)
	{
		OneRowText RowData = m_alldjInfos[i];
		m_segdata.m_diameter = _ttof(RowData[0]); //刃径
		if (i != 0) 
		{
			double len = DrLength(i);
			m_segdata.m_length = len; //刃段长度 //需要修改
		}
		else
		{
			m_segdata.m_length = _ttof(RowData[1]);
		}
		m_segdata.m_ladderAngle1 = _ttof(RowData[2]); //阶梯角
		m_data.AddCutterSegData(std::move(m_segdata));
	}

	double maxLength = _ttof(m_alldjInfos[m_alldjInfos.size() - 1][1]); // 最大刃段长度
	double maxDia = _ttof(m_alldjInfos[m_alldjInfos.size() - 1][0]);	 //最大刃径
	m_GrooveLenth = maxLength + 2 * maxDia;
	//阶梯数量
	m_data.SetLadderCount((int)m_alldjInfos.size()-1 );
	//顶角
	m_data.m_topAngle = m_VertAngle;
	//刀柄
	int sel = m_DaoBingCtrl.GetCurSel();
	CString daobing;
	m_DaoBingCtrl.GetLBText(sel, daobing);
	
	double dis = GetHandleLengthFromDaoBing(daobing);
	//刀柄长度
	m_data.m_handleLength = dis;
	//总长
	m_data.m_totalLength = m_TotalLength - dis;
	//刃数
	int CurSel = m_ui_DrNumCtrl.GetCurSel();
	CString temp;
	m_ui_DrNumCtrl.GetLBText(CurSel, temp);
	m_data.m_cuttingEdgeCount = _ttoi(temp);
	//刀柄
	m_data.SetDaoBingName(daobing);
	//排屑槽长
	m_data.m_GrooveLength = m_GrooveLenth;
	//刀尖类型
	m_data.m_daoJianType = E_DaoJian_平底;

	//隐藏窗口
	ShowWindow(SW_HIDE);
	sel = m_DrillSel.GetCurSel();
	m_DrillSel.GetLBText(sel, temp);
	if (temp.Compare(L"麻花钻") == 0) 
	{
		m_data.Draw(false);
	}
	else 
	{
		CButton *pCheck = (CButton*)GetDlgItem(IDC_CHECK_FORME);
		int state = pCheck->GetCheck();
		if (state==1)
		{
			m_data.m_daoJianType = E_DaoJian_三尖;
			m_data.m_pointCenterDistance = 0.3 * m_data.m_cutterSegs[0].m_diameter;

		}
		m_data.Draw(true);
	}
	CDialogEx::OnOK();
}



void CZTInfoDlg::OnCbnSelchangeCombo1()
{
	UpdateData(TRUE);
	int sel = m_DrillSel.GetCurSel();
	CString temp;
	m_DrillSel.GetLBText(sel, temp);
	// TODO: 在此添加控件通知处理程序代码
	
	if (temp.Compare(L"麻花钻") == 0) {
		CStatic *pStatic = (CStatic*)GetDlgItem(IDC_ZT_BMP);
		CBitmap cbitmap;
		cbitmap.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP9));
		pStatic->SetBitmap(cbitmap);
		GetDlgItem(IDC_CHECK_FORME)->ShowWindow(SW_HIDE);
	}
	else
	{
		CStatic *pStatic = (CStatic*)GetDlgItem(IDC_ZT_BMP);
		CBitmap cbitmap;
		cbitmap.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP6));
		pStatic->SetBitmap(cbitmap);
		GetDlgItem(IDC_CHECK_FORME)->ShowWindow(SW_SHOW);
	}
	UpdateData(FALSE);
}
