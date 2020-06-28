#pragma once


// DlgPcdJD 对话框

class DlgPcdJD : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPcdJD)

public:
	DlgPcdJD(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgPcdJD();

// 对话框数据
	enum { IDD = IDD_DIALOG_PCD_JD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
