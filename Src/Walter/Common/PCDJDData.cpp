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
	CString daoShenFilePath = TY_GetDwgFolder();
	CString str;
	str.Format(L"%d", m_stepNum);
	daoShenFilePath.Append(L"DaoShen\\PCD�µ�ģ��X" + str + L".dwg");
	AcDbObjectId daoShenID = CBlockUtil::InsertDwgAsBlockRef(daoShenFilePath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);

	//�����Ӧ�ĵ���,�����ò���
	CString daoBingFilePath = TY_GetDwgFolder();
	daoBingFilePath.Append(L"DaoBing\\" + m_daoBing + L".dwg");
	double MaxLLen;
	CDynamicBlockUtil::GetDynamicBlockData(daoShenID, L"L", MaxLLen);
	AcGePoint3d insertPiont(pnt.x - MaxLLen, pnt.y, 0);
	CBlockUtil::InsertDwgAsBlockRef(daoBingFilePath, NULL, ACDB_MODEL_SPACE, insertPiont, 0, 1);
	switch (m_stepNum)
	{
	case 1:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	case 2:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	case 3:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 3].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 3].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[m_stepNum - 2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[m_stepNum - 2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	case 4:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 4].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 4].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[m_stepNum - 3].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[m_stepNum - 3].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[m_stepNum - 2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[m_stepNum - 2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D4", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	default:
		break;
	}

	CViewUtil::ZoomExtent();
	return 0;
}