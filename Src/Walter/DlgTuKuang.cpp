// DlgTuKuang.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgTuKuang.h"
#include "afxdialogex.h"
#include "Com.h"
#include "Interaction/UpdateUtil.h"
// CDlgTuKuang �Ի���

IMPLEMENT_DYNAMIC(CDlgTuKuang, CDialogEx)

CDlgTuKuang::CDlgTuKuang(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TUKUANG, pParent)
	, m_designer(_T(""))
	, m_knifClass(_T(""))
	, m_SapNum(_T(""))
	, m_designDate(_T(""))
{

}

CDlgTuKuang::~CDlgTuKuang()
{
}

BOOL CDlgTuKuang::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Init();
	return 0;
}

void CDlgTuKuang::Init()
{
	int defaultIndex = 0;
	//��ʼ��ͼ��
	m_tukuang.AddString(L"A3");
	m_tukuang.AddString(L"��");
	m_tukuang.SetCurSel(defaultIndex);
	//��ʼ������
	m_proportion.AddString(L"1:1");
	m_proportion.AddString(L"2:1");
	m_proportion.AddString(L"2:3");
	m_proportion.AddString(L"��");
	m_proportion.SetCurSel(defaultIndex);
}

void CDlgTuKuang::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_tukuang);
	DDX_Control(pDX, IDC_COMBO2, m_proportion);
	DDX_Text(pDX, IDC_EDIT3, m_designer);
	DDX_Text(pDX, IDC_EDIT5, m_knifClass);
	DDX_Text(pDX, IDC_EDIT6, m_SapNum);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_designDate);
}

LRESULT CDlgTuKuang::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}


BEGIN_MESSAGE_MAP(CDlgTuKuang, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &CDlgTuKuang::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTuKuang::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgTuKuang ��Ϣ�������


void CDlgTuKuang::OnBnClickedButton1()
{
	UpdateData(TRUE);
	ShowWindow(SW_HIDE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"��ѡ��һ�������:", pnt);
	
	//ѡ��ͼ������
	try
	{
		CDocLock doclock;
		int selectIndex = m_tukuang.GetCurSel();
		CString TuKuangName;
		m_tukuang.GetLBText(selectIndex, TuKuangName);
		CString TuKuangPath = TY_GetFrameFolder() + TuKuangName + L".dwg";
		CString blkName = CCommonUtil::GenStrByTime();
		AcDbObjectId tukuangId = CBlockUtil::InsertDwgAsBlockRef(TuKuangPath, blkName, ACDB_MODEL_SPACE, pnt, 0, 1);
		//�������Զ���
		//���
		CBlockUtil::SetBlockRefAttribute(tukuangId, L"!ALT_WZG_BEZ4", m_knifClass);
		//SAP��
		CBlockUtil::SetBlockRefAttribute(tukuangId, L"MAT_BEZ", m_SapNum);
		//ʱ��
		CBlockUtil::SetBlockRefAttribute(tukuangId, L"DWG_ERST_DATUM", m_designDate);
		//�����Ա
		CBlockUtil::SetBlockRefAttribute(tukuangId, L"DWG_ERST_INIT", m_designer);
		//����
		CString proportion;
		selectIndex = m_proportion.GetCurSel();
		m_proportion.GetLBText(selectIndex, proportion);
		CBlockUtil::SetBlockRefAttribute(tukuangId, L"MASSTAB", proportion);
		

	}
	catch (exception& e)
	{
		MessageBox(CString(e.what()));
	}


	CDlgTuKuang::OnOK();
}
