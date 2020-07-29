// ZTInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZTInfoDlg.h"
#include "afxdialogex.h"
#include "Com.h"

// CZTInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CZTInfoDlg, CDialogEx)

CZTInfoDlg::CZTInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CWDR, pParent)
	, m_TotalLength(0)
	, m_VertAngle(0)
	, m_daobing(_T(""))
{

}

CZTInfoDlg::~CZTInfoDlg()
{
}
BOOL CZTInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitGridCtrl();
	//����Ĭ������
	InitDefaultPara();
	LoadGridData();
	//���ص�����Ϣ
	loadDaoBing();
	return 0;
}
//��ʼ��GridCtrl�ؼ�
void CZTInfoDlg::InitGridCtrl()
{
	m_djInfoCtrl.LoadDefaltSettings();
	m_djInfoCtrl.SetEditable(TRUE);
	m_djInfoCtrl.SetRowCount((int)m_alldjInfos.size() + 1);
	m_djInfoCtrl.SetHeaderText(L"�о�;�жγ���;���ݽ�1");
	m_djInfoCtrl.SetHeaderWidth(L"33;33;34");
	m_djInfoCtrl.SetFixedRowCount(1);
	if (m_alldjInfos.size() != 0)
	{
		SetCellHight();
		
	}
}
//����ÿ��С��ĸ�
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
	
	
	//����
	m_VertAngle = 140.0;
	//temp.Format(L"%.1f", m_VertAngle);

	//m_VertexEdit.SetWindowTextW(temp);
	//�ܳ�
	m_TotalLength = 160.0;
	//temp.Format(L"%.1f", m_TotalLength);
	//m_TotalLenEdit.SetWindowTextW(temp);

	m_StepNum.AddString(L"1");
	m_StepNum.AddString(L"2");
	m_StepNum.AddString(L"3");
	m_StepNum.AddString(L"4");
	m_StepNum.SetCurSel(defaultIndex);
	UpdateData(FALSE);
}
//��ʼ�����߽�����Ϣ
MultiRowData CZTInfoDlg::getDefaultGridData(int index)
{
	m_djInfoCtrl.SetEditable(TRUE);
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
		str.Format(L"90");
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
//��ȡ����е�����
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
		vec.push_back(oneRowText);
	}
	return vec;
}
//��䵶������
void CZTInfoDlg::loadDaoBing()
{
	CString dirpath = TY_GetDaoBingSFolder();
	vector<CString> daobing = GetAllDwgFile(dirpath);
	for (int i=0;i < daobing.size();i++)
	{
		m_DaoBingCtrl.AddString(daobing[i]);
	}
	m_DaoBingCtrl.SetCurSel(0);
}
//���Ĭ������
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
	DDX_Control(pDX, IDC_EDIT_ALLLENGTH, m_TotalLenEdit);
	DDX_Control(pDX, IDC_EDIT_VERTEX_ANGLE, m_VertexEdit);
	DDX_Text(pDX, IDC_EDIT_ALLLENGTH, m_TotalLength);
	DDX_Text(pDX, IDC_EDIT_VERTEX_ANGLE, m_VertAngle);
	DDX_CBString(pDX, IDC_COMBO1_DJ, m_daobing);
	DDX_Control(pDX, IDC_COMBO1_DJ, m_DaoBingCtrl);
}

LRESULT CZTInfoDlg::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;;
}


BEGIN_MESSAGE_MAP(CZTInfoDlg, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CZTInfoDlg::OnAcadKeepFocus)
	ON_CBN_SELCHANGE(IDC_COMBO_STEPNUM, &CZTInfoDlg::OnCbnSelchangeComboStepnum)
	ON_BN_CLICKED(IDC_BTNOK, &CZTInfoDlg::OnBnClickedBtnok)
END_MESSAGE_MAP()


// CZTInfoDlg ��Ϣ�������

void CZTInfoDlg::OnCbnSelchangeComboStepnum()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LoadGridData();
}


void CZTInfoDlg::OnBnClickedBtnok()
{
	UpdateData(TRUE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ����
	m_alldjInfos = getTableData();
	m_data.ClearCutterSegs();
	for (int i=0;i < m_alldjInfos.size();i++)
	{
		OneRowText RowData = m_alldjInfos[i];
		m_segdata.m_diameter = _ttof(RowData[0]); //�о�
		m_segdata.m_length = _ttof(RowData[1]); //�жγ���
		m_segdata.m_ladderAngle1 = _ttof(RowData[2]); //���ݽ�
		m_data.AddCutterSegData(m_segdata);
	}
	//��������
	m_data.SetLadderCount((int)m_alldjInfos.size() - 1);
	//����
	m_data.m_topAngle = m_VertAngle;
	//�ܳ�
	m_data.m_totalLength = m_TotalLength;
	//����
	int CurSel = m_ui_DrNumCtrl.GetCurSel();
	CString temp;
	m_ui_DrNumCtrl.GetLBText(CurSel, temp);
	m_data.m_cuttingEdgeCount = _ttoi(temp);
	m_data.SetDaoBingName(m_daobing);

	//���ش���
	ShowWindow(SW_HIDE);
	m_data.Draw(false);
	CDialogEx::OnOK();
}

