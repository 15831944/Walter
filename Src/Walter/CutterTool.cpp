#include "stdafx.h"
#include "CutterTool.h"
#include "res/resource.h"
#include "../Common/Excel/Excel.h"
#include "../Inc/Entity/TextUtil.h"
#include <dbtable.h>


CCutterTool::CCutterTool(void)
{
}


CCutterTool::~CCutterTool(void)
{
}
//���Ʊ��
AcDbObjectId CCutterTool::CreateTable()
{
	CDocLock lock;
	AcGePoint3d pnt(0,0,0);
	 AcGePoint3d insertPos;
	CBlockUtil::GetBlockReferenceInsertPoint(m_tkId, pnt);
	if(m_tkType==A1)
	{
     insertPos.x=pnt.x;
	 insertPos.y=pnt.y+574;
	 insertPos.z=pnt.z;
	}
	else if(m_tkType==A2)
	{
		insertPos.x=pnt.x;
		insertPos.y=pnt.y+400;
		 insertPos.z=pnt.z;
	}
	else
	{
		insertPos.x=pnt.x;
		insertPos.y=pnt.y+277;
		insertPos.z=pnt.z;
	}

	AcDbTable* pTable = new AcDbTable;
	pTable->setColorIndex(256);
	//pTable->suppressTitleRow(false);//���ⲻ��Ҫ����
	//pTable->suppressHeaderRow(false); //��ͷ����Ҫ

	
	//�����к�����Ŀ
	pTable->setNumColumns(4);
	
	pTable->setNumRows(int(toType.size()));


	//�����к��еĸ߶�
	pTable->rowHeight(8);

	pTable->setColumnWidth(0,40);
	pTable->setColumnWidth(1,90);
	pTable->setColumnWidth(2,90);
	pTable->setColumnWidth(3,80);

	// ����������ʽ
	AcDbObjectId stylerid =CTextUtil:: GetTextStyler(_T("Standard"));
	if (stylerid != AcDbObjectId::kNull)
	{
		pTable->setTextStyle(stylerid);
	}
	//pTable->generateLayout();

	pTable->setTextString(0,0,toolNumber);
	pTable->setTextHeight(0,0,3.5);
	for (int i = 0; i < toType.size(); i++)
	{ 
		pTable->setTextHeight(i,1,3.5);
		pTable->setTextHeight(i,2,3.5);
		pTable->setTextHeight(i,3,3.5);
		
		
		pTable->setTextString(i,1,proContent[i]);
		pTable->setTextString(i,2,toDescription[i]);
		pTable->setTextString(i,3, toType[i]);
	}
	pTable->setPosition(insertPos);
	AcDbObjectId tableId = CDwgDatabaseUtil::PostToModelSpace(pTable);
	return tableId;
}
//����excel���
int TY_ReadCutterToolsFromExcel(CString xlsFileName, std::vector<CCutterTool> &cutterTools)
{
	//���ļ�
	Excel::CExcelUtil openfile;

	bool isOpenOK = openfile.OpenExcel(xlsFileName,true);
	
	//�ж��ļ��Ƿ�򿪳ɹ�
	if(isOpenOK)
	{
		int row=14;
		int nOrder = -1;
		//��ȡ�ļ�������
		while(!openfile.GetCellValue(row,5).IsEmpty())
		{
			
			if(!openfile.GetCellValue(row,2).IsEmpty())
			{
				CCutterTool a;
				CString toolsConten;
				CString toolsDesp;
				CString toolsType;
				//��ȡ���ߺ�
				a.toolNumber=openfile.GetCellValue(row,2);
				//��ȡ��������
				toolsConten=openfile.GetCellValue(row,3);
				//��ȡ��������
				toolsDesp=openfile.GetCellValue(row,4);
				//��ȡ�����ͺ�
				toolsType=openfile.GetCellValue(row,5);
				a.proContent.push_back(toolsConten);
				a.toDescription.push_back(toolsDesp);
				a.toType.push_back(toolsType);
				cutterTools.push_back(a);
				nOrder++;
			}
			else
			{
				if (cutterTools.size() > nOrder)
				{
					cutterTools[nOrder].proContent.push_back(openfile.GetCellValue(row,3));
					cutterTools[nOrder].toDescription.push_back(openfile.GetCellValue(row,4));
					cutterTools[nOrder].toType.push_back(openfile.GetCellValue(row,5));
				}
			}
			++row;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}
