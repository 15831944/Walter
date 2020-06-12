#pragma once
#include "StdAfx.h"
#include "res/resource.h"
#include "AcExtensionModule.h"
#include "Com.h"
#include "CutterTool.h"

// WalterDialog 对话框

class WalterDialog : public CDialogEx
{
	DECLARE_DYNAMIC(WalterDialog)

public:
	WalterDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WalterDialog();

// 对话框数据
	enum { IDD = IDD_Walter };

	CAcModuleResourceOverride m_override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT OnAcadKeepFocus(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedDraw();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedUpdatedesdateandname();

	void Draw(vCString toolDill,const AcGePoint3d& point,const CString filename);
	int InsertDwgsAccordingToCutterTools(CString  cadPath,CString tuKuangFileName, TUKuangType tktype, std::vector<CCutterTool> &cutterTools);
	int InputCheckDesDateOrName();
	int InputCheckAtuDateOrName();
	int SetBlockAttribute(AcDbObjectId blkId);
	int SetDesBlockAttribute(AcDbObjectId blkId);
	int SetAutBlockAttribute(AcDbObjectId blkId);
	int InputCheck();
	virtual BOOL OnInitDialog();
    void UpdateDesignerAttr();
	void UpdateAtuAttr();
	void UpdateBomExcel();
	void PostCommandToCAD(CString p_cmd);
	vAcDbObjectId  GetToolsObjectId();



public:
	CString FilePath;
	CString DrawNumber;
	CString Designer;
	CString DesDate;
	CString AuditStaff;
	CString AudDate;
	CString DrawFrame;
	int flag;
	CString flagNum;


	afx_msg void OnBnClickedUpdatecheckdateandname();
	afx_msg void OnBnClickedUpdatebomexcel();
};

extern WalterDialog* g_Walter;
extern Adesk::Boolean acedPostCommand(const ACHAR*);