// ZyXdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZyXdDlg.h"
#include "afxdialogex.h"
#include "Com.h"

// CZyXdDlg 对话框

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

//设置默认参数
void CZyXdDlg::InitDefaultPara()
{
	int defaultIndex = 0;
	//设置刀柄
	vector<CString> dwgfiles = GetAllDwgFile(TY_GetDaoBingSFolder());
	for (int i = 0; i < dwgfiles.size(); i++)
	{
		m_ui_DbCtrl.AddString(dwgfiles[i]);
	}
	m_ui_DbCtrl.SetCurSel(defaultIndex);
	m_ui_DbCtrl.SetCurSel(defaultIndex);
	//默认直径
	m_diameter = 10.0f;
	//默认刃长
	m_lengthOfKnife =25.0f;
	//默认刀尖圆角
	m_ArcOfknife = 0.0f;
	//默认悬伸长度
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


// CZyXdDlg 消息处理程序


void CZyXdDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	m_ThreadData.ClearCutterSegs();
	// TODO: 在此添加控件通知处理程序代码
	//添加一阶数据
	m_ThreadSegData.m_diameter = m_diameter;
	m_ThreadSegData.m_length = m_lengthOfKnife;
	m_ThreadSegData.m_ladderAngle1 = 90.0;
	//如果是刀尖圆角是0则刀尖应该是直角
	if (m_ArcOfknife == 0) 
		m_ThreadData.m_topAngle = 180;
	//不由用户设置
	m_ThreadData.m_cuttingEdgeCount = 6;
	//刀柄
	int sel = m_ui_DbCtrl.GetCurSel();
	CString daobing;
	m_ui_DbCtrl.GetLBText(sel, daobing);
	m_ThreadData.m_daobing = daobing;
	//总长
	double dis = GetHandleLengthFromDaoBing(daobing);
	m_ThreadData.m_handleLength = dis;
	m_ThreadData.m_totalLength = m_lengthOfHang - dis;
	//刀尖类型
	m_ThreadData.m_daoJianType = E_DaoJian_平底;
	//必须设置排屑槽长，排屑槽长要大于刃径的0.5倍
	m_ThreadData.m_GrooveLength = m_lengthOfKnife * 0.5 ;
	//
	m_ThreadData.AddCutterSegData(m_ThreadSegData);
	m_ThreadData.SetLadderCount(0);
	ShowWindow(SW_HIDE);
	//绘制
	m_ThreadData.Draw(false);
}
