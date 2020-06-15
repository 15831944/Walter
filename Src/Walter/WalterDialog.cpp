// WalterDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "WalterDialog.h"
#include "afxdialogex.h"
#include "OpenExcelDialog.h"
#include "CutterTool.h"
#include "Com.h"
#include "../Inc/Entity/BlockUtil.h"
#include "../Inc/Interaction/GetInputUtil.h"
#include "Others/ToolingUtil.h"
#include "Others/XDataUtil.h"
#include "Entity/ObjectUtil.h"
#include "Document/DocLock.h"

WalterDialog* g_Walter = NULL;
CString g_filePath;
CString g_frame;
CString g_draw;
CString g_designer;
CString g_desDate;
CString g_checker;
CString g_checkDate;

// WalterDialog �Ի���

IMPLEMENT_DYNAMIC(WalterDialog, CDialogEx)

WalterDialog::WalterDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(WalterDialog::IDD, pParent)
	, FilePath(_T(""))
	, DrawNumber(_T(""))
	, Designer(_T(""))
	, DesDate(_T(""))
	, AuditStaff(_T(""))
	, AudDate(_T(""))
	, DrawFrame(_T(""))
	, flag(0)
{

}

WalterDialog::~WalterDialog()
{
}

void WalterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FilePath, FilePath);
	DDX_Text(pDX, IDC_EDIT2, DrawNumber);
	DDX_Text(pDX, IDC_EDIT3, Designer);
	DDX_Text(pDX, IDC_EDIT4, DesDate);
	DDX_Text(pDX, IDC_EDIT5, AuditStaff);
	DDX_Text(pDX, IDC_EDIT6, AudDate);
	DDX_CBString(pDX, IDC_COMBO1, DrawFrame);
}
LRESULT WalterDialog::OnAcadKeepFocus(WPARAM, LPARAM)
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(WalterDialog, CDialogEx)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, &WalterDialog::OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BROWSE, &WalterDialog::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_DRAW, &WalterDialog::OnBnClickedDraw)
	ON_CBN_SELCHANGE(IDC_COMBO1, &WalterDialog::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_UpdateDesDateAndName, &WalterDialog::OnBnClickedUpdatedesdateandname)
	ON_BN_CLICKED(IDC_UpdateCheckDateAndName, &WalterDialog::OnBnClickedUpdatecheckdateandname)
	ON_BN_CLICKED(IDC_UpdateBomExcel, &WalterDialog::OnBnClickedUpdatebomexcel)
END_MESSAGE_MAP()


// WalterDialog ��Ϣ�������


//��ȡ�ļ�����
void WalterDialog::OnBnClickedBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vCString files= COpenExcelDialog::TYCOM_GetFilesFromDialog();
	//CString fileName=CCommonUtil::FilePathToFileName(files[0]);
	if(files.size()==0)
	{
	    AfxMessageBox(L"û��ѡ���ļ�����ѡ���ļ���Ч");
	    return;
	}
	SetDlgItemText(IDC_FilePath,files[0]);
}

//�ж�����������Ƿ�Ϊ��
int WalterDialog::InputCheck()
{
	UpdateData();

	if(DrawNumber.IsEmpty())
	{
		MessageBox(L"ͼֽ�Ų���Ϊ��,�����룡");
		return 1;
	}
	else if(Designer.IsEmpty())
	{
		MessageBox(L"�����Ա����Ϊ�գ������룡");
		return 1;
	}
	else if(DesDate.IsEmpty())
	{
		MessageBox(L"������ڲ���Ϊ�գ������룡");
		return 1;
	}
	else if(AuditStaff.IsEmpty())
	{
		MessageBox(L"�����Ա����Ϊ�գ������룡");
		return 1;
	}
	else if(AudDate.IsEmpty())
	{
		MessageBox(L"������ڲ���Ϊ�գ������룡");
		return 1;
	}
	else if(DrawFrame.IsEmpty())
	{
		MessageBox(L"ͼ����Ϊ�գ������룡");
		return 1;
	}
	UpdateData(FALSE);
	return 0;
}

//�ж������Ա�Լ������Ƿ�Ϊ��
int WalterDialog::InputCheckDesDateOrName()
{
	UpdateData();
    if(Designer.IsEmpty())
	 {
		MessageBox(L"�����Ա����Ϊ�գ������룡");
		return 1;
	 }
	else if(DesDate.IsEmpty())
	 {
		MessageBox(L"������ڲ���Ϊ�գ������룡");
		return 1;
	 }
	UpdateData(FALSE);
	return 0;
}

//�ж������Ա�Լ�����������Ƿ�Ϊ��
int WalterDialog::InputCheckAtuDateOrName()
{
	UpdateData();
	if(AuditStaff.IsEmpty())
	{
		MessageBox(L"�����Ա����Ϊ�գ������룡");
		return 1;
	}
	else if(AudDate.IsEmpty())
	{
		MessageBox(L"������ڲ���Ϊ�գ������룡");
		return 1;
	}
	UpdateData(FALSE);
	return 0;
}

//���ÿ�Ĭ������
int WalterDialog::SetBlockAttribute(AcDbObjectId blkId)
{
    UpdateData();
	AcDbEntityPointer pEnt(blkId, AcDb::kForWrite);
	if (pEnt.openStatus() == Acad::eOk)
	{
		AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
		if (pBlkRef == NULL)
		{
			if(pEnt != NULL)
				pEnt->close();
			return -1;
		}

		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_VERSION_1", L"00");//��һ�������ǿ���գ��ڶ��������Ա�ǣ�������������ֵ
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_NUMMER_1", L"REV00");
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_DATUM_1", DesDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_NAME_1", Designer);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"DWG_ERST_DATUM", DesDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"DWG_ERST_INIT",Designer);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"AT17",AudDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"AT18",AuditStaff);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"!ALT_WZG_BEZ4",L"machining plan");
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"MAT_BEZ",DrawNumber);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"DOKTYPE",L"D10");
		pBlkRef->close();
	}
	
	UpdateData(FALSE);
	return 0;

}

//�޸������Ա�����Լ���������
int WalterDialog::SetDesBlockAttribute(AcDbObjectId blkId)
{
	UpdateData();
	AcDbEntityPointer pEnt(blkId, AcDb::kForWrite);
	if (pEnt.openStatus() == Acad::eOk)
	{
		AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
		if (pBlkRef == NULL)
		{
			return -1;
		}
		CBlockUtil::GetAttributeValue(pBlkRef,L"L_AE_VERSION_1",flagNum);

		if(flagNum.Left(1)==L"0")
			flagNum =flagNum.Right(1);

		flag =_ttoi(flagNum);
		flag++;
		CString version;
		CString nunner;
		version.Format(L"0%2d",flag);
		nunner.Format(L"REV0%d",flag);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_VERSION_1",version);//��һ�������ǿ���գ��ڶ��������Ա�ǣ�������������ֵ
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_NUMMER_1",nunner);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_DATUM_1", DesDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_NAME_1", Designer);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"DWG_ERST_DATUM", DesDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"DWG_ERST_INIT",Designer);
		pBlkRef->close();
	}

	UpdateData(FALSE);
	return 0;

}

//�޸������Ա�����Լ���������
int WalterDialog::SetAutBlockAttribute(AcDbObjectId blkId)
{
	UpdateData();
	AcDbEntityPointer pEnt(blkId, AcDb::kForWrite);
	if (pEnt.openStatus() == Acad::eOk)
	{
		AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
		if (pBlkRef == NULL)
		{
			return -1;
		}
		CBlockUtil::GetAttributeValue(pBlkRef,L"L_AE_VERSION_1",flagNum);

		if(flagNum.Left(1)==L"0")
			flagNum =flagNum.Right(1);

		flag =_ttoi(flagNum);
		flag++;
		CString version;
		CString nunner;
		version.Format(L"0%2d",flag);
		nunner.Format(L"REV0%d",flag);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_VERSION_1",version);//��һ�������ǿ���գ��ڶ��������Ա�ǣ�������������ֵ
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"L_AE_NUMMER_1", nunner);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"AT17",AudDate);
		CBlockUtil::SetBlockRefAttribute(pBlkRef, L"AT18",AuditStaff);
		pBlkRef->close();
	}

	UpdateData(FALSE);
	return 0;

}


//����ͼ����ͼֽ
int WalterDialog::InsertDwgsAccordingToCutterTools(CString  cadPath,CString tuKuangFileName, TUKuangType tktype, std::vector<CCutterTool> &cutterTools)
{
	CDocLock lock;

	const double A1_wide=880;
	const double A2_wide=630;
	const double A3_wide=450;
	const double A1_height=650;
	const double A2_height=470;
	const double A3_height=350;
	//ͼ������
	AcGePoint3d pnt(0,0,0);
	
	for (int i = 0; i < cutterTools.size(); i++)
	{
		int Px = i / 20;
		int Py = i % 20;

		if (tktype == A1)
		{
			//ͼ��Ĳ�������ֵ
			AcGePoint3d pnt1(pnt.x + Py*A1_wide, pnt.y - Px*A1_height, pnt.z);
			//����ͼ��
			bool isExistTK = CCommonUtil::IsFileExist(tuKuangFileName);
			if (!isExistTK)
			{
				acutPrintf(L"File Name:" + tuKuangFileName + " not exist\n");
			}
			else
			{
				cutterTools[i].m_tkId = CBlockUtil::InsertDwgAsBlockRef(tuKuangFileName, L"A1", ACDB_MODEL_SPACE, pnt1, 0, 1);
			}

			for (int j = 0; j < cutterTools[i].toType.size(); j++)
			{
					//��ȡͼֽ
					int zPx=j/2;
					int zPy=j%2;
					CString tzFilename=cadPath + "\\Support\\Walter\\DaoJuXingHao\\"+cutterTools[i].toType[j]+".dwg";
					bool isExist = CCommonUtil::IsFileExist(tzFilename);
					if (!isExist)
					{
						acutPrintf(L"File Name:"+tzFilename+"not exist\n");
						continue;
					}
					AcDbObjectId tzId;
					//����ͼֽ��������ֵ
					AcGePoint3d pnt2(pnt1.x+((A1_wide)/3)+zPy*350,pnt1.y+(A1_height*2/3.0)-zPx*200,pnt.z);
					//���뵶��ͼֽ
				    tzId =CBlockUtil::InsertDwgAsBlockRef(tzFilename, NULL, ACDB_MODEL_SPACE, pnt2, 0, 1);
					 AcGeVector3d vect = CBlockUtil::GetBlkVectorFromMiddlePointToOri(tzId);

					AcDbEntityPointer pEnt(tzId, AcDb::kForWrite);
					if (pEnt.openStatus() == Acad::eOk)
					{
						AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
						if (pBlkRef == NULL)
						{
							return -1;
						}

						AcGeMatrix3d matrix;
						matrix.setTranslation(vect);
						pBlkRef->transformBy(matrix);
					}
				}
			}
		else if(tktype==A2)
		{
			AcGePoint3d pnt1(pnt.x+Py*A2_wide,pnt.y-Px*A2_height,pnt.z);
			//����ͼ��
			bool isExistTK = CCommonUtil::IsFileExist(tuKuangFileName);
			if (!isExistTK)
			{
				acutPrintf(L"File Name:" + tuKuangFileName + "not exist\n");
			}
			else
			    cutterTools[i].m_tkId = CBlockUtil::InsertDwgAsBlockRef(tuKuangFileName, L"A2", ACDB_MODEL_SPACE, pnt1, 0, 1);

			
			for (int j = 0; j < cutterTools[i].toType.size(); j++)
			{
				//��ȡͼֽ
				int zPx=j/2;
				int zPy=j%2;
				CString tzFilename=cadPath + "\\Support\\Walter\\DaoJuXingHao\\"+cutterTools[i].toType[j]+".dwg";
				bool isExist = CCommonUtil::IsFileExist(tzFilename);
				if (!isExist)
				{
					acutPrintf(L"File Name:"+tzFilename+"not exist\n");
					continue;
				}

				//����ͼֽ��������ֵ
				AcGePoint3d pnt2(pnt1.x+((A2_wide)/3)+zPy*200,pnt1.y+(A2_height*2/3.0)-zPx*150,pnt.z);
				AcDbObjectId tzId;
				//���뵶��ͼֽ
				tzId = CBlockUtil::InsertDwgAsBlockRef(tzFilename, NULL, ACDB_MODEL_SPACE, pnt2, 0, 1);
				AcGeVector3d vect = CBlockUtil::GetBlkVectorFromMiddlePointToOri(tzId);

				AcDbEntityPointer pEnt(tzId, AcDb::kForWrite);
				if (pEnt.openStatus() == Acad::eOk)
				{
					AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
					if (pBlkRef == NULL)
					{
						return -1;
					}

					AcGeMatrix3d matrix;
					matrix.setTranslation(vect);
					pBlkRef->transformBy(matrix);
				}
			}
		}
		else
		{
			AcGePoint3d pnt1(pnt.x+Py*A3_wide,pnt.y-Px*A3_height,pnt.z);
			//����ͼ��
			bool isExistTK = CCommonUtil::IsFileExist(tuKuangFileName);
			if (!isExistTK)
			{
				acutPrintf(L"File Name:" + tuKuangFileName + "not exist\n");
			}
			else
			cutterTools[i].m_tkId = CBlockUtil::InsertDwgAsBlockRef(tuKuangFileName, L"A3", ACDB_MODEL_SPACE, pnt1, 0, 1);

			for (int j = 0; j < cutterTools[i].toType.size(); j++)
			{
				//��ȡͼֽ
				int zPx = j/2;
				int zPy = j%2;
				CString tzFilename=cadPath + "\\Support\\Walter\\DaoJuXingHao\\"+cutterTools[i].toType[j]+".dwg";
				bool isExist = CCommonUtil::IsFileExist(tzFilename);
				if (!isExist)
				{
					acutPrintf(L"File Name:"+tzFilename+"not exist\n");
					continue;
				}

				//����ͼֽ��������ֵ
				AcGePoint3d pnt2(pnt1.x+((A3_wide)/3)+zPy*100,pnt1.y+(A3_height*2/3.0)-zPx*100,pnt.z);
				//���뵶��ͼֽ
				AcDbObjectId tzId;
				//���뵶��ͼֽ
				tzId = CBlockUtil::InsertDwgAsBlockRef(tzFilename, NULL, ACDB_MODEL_SPACE, pnt2, 0, 1);
				AcGeVector3d vect = CBlockUtil::GetBlkVectorFromMiddlePointToOri(tzId);

				AcDbEntityPointer pEnt(tzId, AcDb::kForWrite);
				if (pEnt.openStatus() == Acad::eOk)
				{
					AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
					if (pBlkRef == NULL)
					{
						return -1;
					}

					AcGeMatrix3d matrix;
					matrix.setTranslation(vect);
					pBlkRef->transformBy(matrix);
				}
			}
		}
	}
	return 0;
}

//����ͼֽ
void WalterDialog::OnBnClickedDraw()
{
	CDocLock lock;
	UpdateData();
	g_filePath = FilePath;
	g_frame = DrawFrame;
	g_draw = DrawNumber;
	g_designer = Designer;
	g_desDate = DesDate;
	g_checker = AuditStaff;
	g_checkDate = AudDate;
	//��һ�����������
	if(InputCheck() != 0)
	{
		return;
	}
	
	//�ȹرնԻ���
	WalterDialog::OnOK();
	
    //�ڶ�������ȡexcel����
	std::vector<CCutterTool> cutterTools;
	TY_ReadCutterToolsFromExcel(FilePath, cutterTools);
	
	//�����������ݶ�ȡ��excel���ݲ����Ӧ��ͼ��
	//��ȡͼ���ļ���
	//��ȡCAD��װ�Ǹ�·��
	CString cadPath = TY_GetAppPath();
	
	//��ȡͼֽ��
	CString tkFilename= cadPath + "\\Walter\\TuKuang\\"+DrawFrame+".dwg";

	TUKuangType type;
	if (DrawFrame == L"A1")
	{
		type = A1;
	}
	else if(DrawFrame == L"A2")
	{
		type=A2;
	}
	else
	{
		type=A3;
	}
	CDocLock lock;
	//���Ĳ�����ͼ���Լ�ͼֽ
	InsertDwgsAccordingToCutterTools(cadPath,tkFilename, type, cutterTools);

	//4.1 д��ͼ������
	//���Ĳ������Ʊ��
	for (int i = 0; i < cutterTools.size(); i++)
	{

		cutterTools[i].m_tkType = type;
		AcDbObjectId tableId = cutterTools[i].CreateTable();
		CXDataUtil::AddEntityInfo(tableId, L"BOM_TABLE", cutterTools[i].toolNumber);

		//����ʵ��ID�õ�ָ��
		int result=SetBlockAttribute(cutterTools[i].m_tkId);

		if(result==-1)
		{
			acutPrintf(L"���ÿ�����ֵʧ�ܣ�");
			continue;
		}	
	}
	UpdateData(FALSE);
}


void WalterDialog::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}


//��ʼ��������
BOOL WalterDialog::OnInitDialog()
{
	int initNum=0;
	CDialogEx::OnInitDialog();
	if(g_frame==L""||g_frame==L"A1")
	{
		((CComboBox *)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	}
	else if(g_frame==L"A2")
	{
		((CComboBox *)GetDlgItem(IDC_COMBO1))->SetCurSel(1);
	}
	else
	{
		((CComboBox *)GetDlgItem(IDC_COMBO1))->SetCurSel(2);
	}
	UpdateData(TRUE);
	//��ʼ��������
	FilePath = g_filePath;
	DrawNumber = g_draw;
	Designer = g_designer;
	DesDate = g_desDate;
	AuditStaff = g_checker;
	AudDate = g_checkDate;
	//�������Ӧ�ı���.SetCurSel(0);
	UpdateData(FALSE);
	return true;
}

//��ȡ����ͼ���ID
vAcDbObjectId WalterDialog::GetToolsObjectId()
{
	CDocLock lock;
	vAcDbObjectId blockIds;
	vAcDbObjectId vids;
	CString blockName;
	int num = CBlockUtil::CycleAllBlockReferences(vids);
	if(num>0)
	{
		for(int i=0;i<num;i++)
		{
			//��ȡ����Id��Ӧ��ʵ������
			CBlockUtil::GetBlockNameByBlockRef(vids[i],blockName);
			//��ͼ�����ƱȽϣ�����ͼ��Id
			if(blockName=="A1"||blockName=="A2"||blockName=="A3")
			{
				blockIds.push_back(vids[i]);

			}
		}
	}
	else
	{
		acutPrintf(L"δ�ҵ���Ӧ��ͼ�飡");
	}
	return blockIds;

}
 
//���������Ա�Լ�����
void WalterDialog::OnBnClickedUpdatedesdateandname()
{
	//WalterDialog::OnOK();
	UpdateData();
	g_designer = Designer;
	g_desDate = DesDate;
	UpdateData(FALSE);
	CGetInputUtil::SendCommandToCad(L"gx ");
}

void WalterDialog::UpdateDesignerAttr()
{
	//��һ������������Ƿ�Ϊ��
    if(InputCheckDesDateOrName()!=0) 
	{
		return ;
	}

	//�ڶ�������ȡ����ͼ��
	 vAcDbObjectId blockId = GetToolsObjectId();

	for (int i=0;i<blockId.size();i++)
	{
		int result=SetDesBlockAttribute(blockId[i]);

		if(result==-1)
		{
			acutPrintf(L"���ÿ�����ֵʧ�ܣ�");
			continue;
		}	
	}
}

//���������Ա�Լ�����

void WalterDialog::OnBnClickedUpdatecheckdateandname()
{
	UpdateData(TRUE);
	g_checker = AuditStaff;
	g_checkDate = AudDate;
	UpdateData(FALSE);
	//��һ������������Ƿ�Ϊ��
	CGetInputUtil::SendCommandToCad(L"setCheck ");
}

void WalterDialog::UpdateAtuAttr()
{
	if(InputCheckAtuDateOrName()!=0)
	{
		return;
	}
	//�ڶ�������ȡͼ��ID
	vAcDbObjectId blockId = GetToolsObjectId();

	for (int i=0;i<blockId.size();i++)
	{
		int result=SetAutBlockAttribute(blockId[i]);

		if(result==-1)
		{
			acutPrintf(L"���ÿ�����ֵʧ�ܣ�");
			continue;
		}	
	}
}

//����Bom���
void WalterDialog::OnBnClickedUpdatebomexcel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CGetInputUtil::SendCommandToCad(L"setBom ");
}

void WalterDialog::UpdateBomExcel()
{
	CDocLock lock;
	UpdateData();
	//��һ�����ҵ����е�table
	vAcDbObjectId tableIds;
	CToolingUtil::CycleAllTypedObjectsInAllLayer(CToolingUtil::ACDB_TABLE,tableIds);

	//�ڶ�����ɾ�����е�table
	for (int i = 0; i < tableIds.size(); i++)
	{
		CString value;
		if(CXDataUtil::ReadEntityInfo(tableIds[i], L"BOM_TABLE", value) == 0)
		    CObjectUtil::DeleteObject(tableIds[i]);
	}
	
	//������ ��ÿ��ͼ�����±�
	CString blkName;
	vAcDbObjectId blockIds = GetToolsObjectId(); 
	if(blockIds.empty())
	{
		return;
	}
	CBlockUtil::GetBlockNameByBlockRef(blockIds[0],blkName);
	//3.1 ��ȡexcel
	std::vector<CCutterTool> cutterTools;
	TY_ReadCutterToolsFromExcel(FilePath, cutterTools);
	//3.2 ���²������
	TUKuangType type;
	if (blkName == L"A1")
	{
		type = A1;
	}
	else if(blkName == L"A2")
	{
		type=A2;
	}
	else
	{
		type=A3;
	}
	for (int i = 0; i < cutterTools.size(); i++)
	{
		cutterTools[i].m_tkId=blockIds[i];
		cutterTools[i].m_tkType = type;
	    AcDbObjectId tableId = cutterTools[i].CreateTable();
		CXDataUtil::AddEntityInfo(tableId, L"BOM_TABLE", cutterTools[i].toolNumber);
	}
	UpdateData(FALSE);
}