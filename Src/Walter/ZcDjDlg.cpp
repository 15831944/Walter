// ZcDjDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZcDjDlg.h"
#include "afxdialogex.h"
#include "Com.h"

// CZcDjDlg �Ի���

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


// CZcDjDlg ��Ϣ�������


BOOL CZcDjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//����Ĭ�ϲ���
	InitParamers();
	InitGridCtrl();
	LoadGridData();
	//���ص���
	loadDaoBing();
	return TRUE;
}


void CZcDjDlg::OnBnClickedZcdraw()
{
	UpdateData(TRUE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ����
	m_allStepData = getTableData();
	m_data.ClearCutterSegs();
	for (int i = 0; i < m_allStepData.size(); i++)
	{
		OneRowText RowData = m_allStepData[i];
		m_segdata.m_diameter = _ttof(RowData[0]); //�о�
		m_segdata.m_length = _ttof(RowData[1]); //�жγ���
		m_segdata.m_ladderAngle1 = _ttof(RowData[2]); //���ݽ�
	
		m_data.AddCutterSegData(m_segdata);
	}
	//��������
	m_data.SetLadderCount((int)m_allStepData.size() - 1);
	//����
	m_data.m_topAngle = m_VertexAngle;
	//�ܳ�
	m_data.m_totalLength = m_totalLength;
	//����

	m_data.SetDaoBingName(m_DaoBingName);

	//���ش���
	ShowWindow(SW_HIDE);
	m_data.Draw(TRUE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
//��ʼ��Ĭ�ϲ���
void CZcDjDlg::InitParamers()
{
	UpdateData(TRUE);


	
	int defaultIndex = 0;
	//����
	m_VertexAngle = 140.0;
	////��ʼ��ͼ��
	//m_tukuang.AddString(L"A3");
	//m_tukuang.AddString(L"��");
	//m_tukuang.SetCurSel(defaultIndex);
	////��ʼ������
	//m_proportion.AddString(L"1:1");
	//m_proportion.AddString(L"2:1");
	//m_proportion.AddString(L"2:3");
	//m_proportion.AddString(L"��");
	//m_proportion.SetCurSel(defaultIndex);
	//�ܳ�
	m_totalLength = 160.0;

	//��������
	m_StepNumCtrl.AddString(L"1");
	m_StepNumCtrl.AddString(L"2");
	m_StepNumCtrl.AddString(L"3");
	m_StepNumCtrl.AddString(L"4");
	m_StepNumCtrl.SetCurSel(defaultIndex);
	UpdateData(FALSE);
}

//��ʼ��MFCGRid�ؼ�
void CZcDjDlg::InitGridCtrl()
{
	m_ZcDjJTInfoCtrl.LoadDefaltSettings();
	m_ZcDjJTInfoCtrl.SetEditable(TRUE);
	m_ZcDjJTInfoCtrl.SetRowCount((int)m_allStepData.size() + 1);
	m_ZcDjJTInfoCtrl.SetHeaderText(L"�о�;�жγ���;���ݽ�1");
	m_ZcDjJTInfoCtrl.SetHeaderWidth(L"33;33;34");
	m_ZcDjJTInfoCtrl.SetFixedRowCount(1);
	if (m_allStepData.size() != 0)
	{
		SetCellHight();

	}
}
//����ÿһ��cell�ĸ�
void CZcDjDlg::SetCellHight()
{
	for (int i = 1; i <= m_allStepData.size(); ++i)
	{
		m_ZcDjJTInfoCtrl.SetRowHeight(i, 25);
	}
}
//�����ݼ��ص�������
void CZcDjDlg::LoadGridData()
{
	int index = m_StepNumCtrl.GetCurSel();
	m_allStepData = getDefaultGridData(index);
	m_ZcDjJTInfoCtrl.FillTable(m_allStepData);
}
//����Ĭ������
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
