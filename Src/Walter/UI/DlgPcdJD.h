#pragma once


// DlgPcdJD �Ի���

class DlgPcdJD : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPcdJD)

public:
	DlgPcdJD(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgPcdJD();

// �Ի�������
	enum { IDD = IDD_DIALOG_PCD_JD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
