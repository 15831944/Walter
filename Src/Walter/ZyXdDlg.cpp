// ZyXdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZyXdDlg.h"
#include "afxdialogex.h"
#include "Com.h"

// CZyXdDlg �Ի���

IMPLEMENT_DYNAMIC(CZyXdDlg, CDialogEx)

CZyXdDlg::CZyXdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ZYXD, pParent)
	, m_diameter(0)
	, m_lengthOfKnife(0)
	, m_ArcOfknife(0)
	, m_lengthOfHang(0)
{

}

CZyXdDlg::~CZyXdDlg()
{
}

BOOL CZyXdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitDefaultPara();
	return TRUE;
}

//����Ĭ�ϲ���
void CZyXdDlg::InitDefaultPara()
{
	int defaultIndex = 0;
	//���õ���
	m_ui_DbCtrl.AddString(L"Z03");
	m_ui_DbCtrl.AddString(L"Z06");
	m_ui_DbCtrl.AddString(L"Z08");
	m_ui_DbCtrl.AddString(L"Z10");
	m_ui_DbCtrl.AddString(L"Z12");
	m_ui_DbCtrl.AddString(L"Z14");
	m_ui_DbCtrl.AddString(L"Z16");
	m_ui_DbCtrl.AddString(L"Z18");
	m_ui_DbCtrl.AddString(L"Z20");
	m_ui_DbCtrl.AddString(L"Z25");
	m_ui_DbCtrl.AddString(L"Z32");
	m_ui_DbCtrl.SetCurSel(defaultIndex);
	//Ĭ��ֱ��
	m_diameter = 10.0f;
	//Ĭ���г�
	m_lengthOfKnife =25.0f;
	//Ĭ�ϵ���Բ��
	m_ArcOfknife = 0.0f;
	//Ĭ�����쳤��
	m_lengthOfHang = 160.0f;
	UpdateData(FALSE);
}

void CZyXdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ui_DbCtrl);
	DDX_Text(pDX, IDC_EDIT3, m_diameter);
	DDX_Text(pDX, IDC_EDIT4, m_lengthOfKnife);
	DDX_Text(pDX, IDC_EDIT1, m_ArcOfknife);
	DDX_Text(pDX, IDC_EDIT2, m_lengthOfHang);
}

LRESULT CZyXdDlg::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}


BEGIN_MESSAGE_MAP(CZyXdDlg, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CZyXdDlg::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &CZyXdDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CZyXdDlg ��Ϣ�������


void CZyXdDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	m_ThreadData.ClearCutterSegs();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���һ������
	m_ThreadSegData.m_diameter = m_diameter;
	m_ThreadSegData.m_length = m_lengthOfKnife;
	m_ThreadSegData.m_ladderAngle1 = 90.0;
	//����ǵ���Բ����0�򵶼�Ӧ����ֱ��
	if (m_ArcOfknife == 0) 
		m_ThreadData.m_topAngle = 180;
	//�����û�����
	m_ThreadData.m_cuttingEdgeCount = 6;
	//����
	int sel = m_ui_DbCtrl.GetCurSel();
	CString daobing;
	m_ui_DbCtrl.GetLBText(sel, daobing);
	m_ThreadData.m_daobing = daobing;
	//�ܳ�
	double dis = GetHandleLengthFromDaoBing(daobing);
	m_ThreadData.m_handleLength = dis;
	m_ThreadData.m_totalLength = m_lengthOfHang - dis;
	//��������
	m_ThreadData.m_daoJianType = E_DaoJian_ƽ��;
	//����������м�۳�
	m_ThreadData.m_GrooveLength = m_lengthOfKnife;
	//
	m_ThreadData.AddCutterSegData(m_ThreadSegData);
	m_ThreadData.SetLadderCount(0);
	ShowWindow(SW_HIDE);
	//����
	m_ThreadData.Draw(false);
}
