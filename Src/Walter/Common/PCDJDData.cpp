#include "stdafx.h"
#include "PCDJDData.h"
#include "Entity/DynamicBlockUtil.h"
#include "Com.h"

static HINSTANCE s_gTyToolInst = 0;

int SPCDJDData::Draw()
{
	//��һ�����û�ѡ��һ�������
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"��ѡ��һ�������:", pnt);
	CDocLock lock;	//�����ȡ�ļ�ʧ�ܣ���������Ϊ֮ǰû�м���

	//�����Ӧ�ĵ���
	CString daoShenFilePath = TY_GetDynamicBlockFolder();
	CString str;
	str.Format(L"%d", m_stepNum);
	daoShenFilePath.Append(L"PCD�µ�ģ��X" + str + L".dwg");
	AcDbObjectId daoShenID = CBlockUtil::InsertDwgAsBlockRef(daoShenFilePath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);

	//�����Ӧ�ĵ���,�����ò���
	CString daoBingFilePath = TY_GetDaoBingFolder();
	daoBingFilePath.Append(m_daoBing + L".dwg");
	double MaxLLen = m_stepDatas[m_stepDatas.size() - 1].m_stepLength;
	AcGePoint3d insertPiont(pnt.x - MaxLLen, pnt.y, 0);
	CBlockUtil::InsertDwgAsBlockRef(daoBingFilePath, NULL, ACDB_MODEL_SPACE, insertPiont, 0, 1);

	double Lf2;
	switch (m_stepNum)
	{
	case 1:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X1", L"����_1_T", L"An", m_stepDatas[0].m_angle);
		break;
	}
	case 2:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);
		
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X2", L"����_1_T", L"An", m_stepDatas[0].m_angle);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[ 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X2", L"����_2_T", L"An", m_stepDatas[1].m_angle);
		break;
	}
	case 3:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_1_T", L"An", m_stepDatas[0].m_angle);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_2_T", L"An", m_stepDatas[1].m_angle);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[2].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_3_T", L"An", m_stepDatas[2].m_angle);
		break;
	}
	case 4:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
		
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_1_T", L"An", m_stepDatas[0].m_angle);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_2_T", L"An", m_stepDatas[1].m_angle);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[2].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_3_T", L"An", m_stepDatas[2].m_angle);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D4", m_stepDatas[3].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L4", m_stepDatas[3].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_4_T", L"An", m_stepDatas[3].m_angle);
		break;
	}
	default:
		break;
	}

	CViewUtil::ZoomExtent();
	return 0;
}