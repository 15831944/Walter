#include "stdafx.h"
#include "ZYDJData.h"
#include "Entity/DimensionUtil.h"
#include "Entity/DynamicBlockUtil.h"

CZYDJData::CZYDJData()
{
}


CZYDJData::~CZYDJData()
{
}

void CZYDJData::Draw()
{
	CDocLock lock;
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"���������㣺", pnt);

	//���õ��������ļ�
	CString labberCountStr;
	labberCountStr.Format(L"%d", m_DjLabberCount);
	CString DaoShenFullPath;
	CString DaoBingFullPath = TY_GetDaoBingSFolder() + m_DaoBingName + L".dwg";
	if (m_IsKKD)
		DaoShenFullPath = TY_GetDynamicBlockFolder() + L"��Ӳ���׵�ģ��X"+ labberCountStr + L".dwg";
	else
		DaoShenFullPath = TY_GetDynamicBlockFolder() + L"��Ӳ�µ�ģ��X"+ labberCountStr + L".dwg";
	
	AcDbObjectId DaoBingId = CBlockUtil::InsertDwgAsBlockRef(DaoBingFullPath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);
	//���㵶������
	double dis = GetHandleLengthFromDaoBing(m_DaoBingName);
	AcGePoint3d ptInsert(pnt);
	ptInsert.x += m_totalLength + dis;
	AcDbObjectId DaoShenId = CBlockUtil::InsertDwgAsBlockRef(DaoShenFullPath, NULL, ACDB_MODEL_SPACE, ptInsert, 0, 1);

	//�����ܳ�
	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L", m_totalLength);
	// ��������׵���Ҫ����Ԥ��ֱ��
	if (m_IsKKD)
	{
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D0",m_Prediameter);
	}
	//���ò�������
	switch (m_DjLabberCount)
	{
	case 1:
	{
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L1", m_StepData[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"A1", m_StepData[0].m_angle);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D1", m_StepData[0].m_diameter);
		break;
	}
	case 2:
	{
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L1", m_StepData[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"A1", m_StepData[0].m_angle);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D1", m_StepData[0].m_diameter);

		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L2", m_StepData[1].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"A2", m_StepData[1].m_angle);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D2", m_StepData[1].m_diameter);
		break;
	}
	case 3:
	{
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L1", m_StepData[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"A1", m_StepData[0].m_angle);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D1", m_StepData[0].m_diameter);

		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L2", m_StepData[1].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"A2", m_StepData[1].m_angle);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D2", m_StepData[1].m_diameter);

		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L3", m_StepData[2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"A3", m_StepData[2].m_angle);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D3", m_StepData[2].m_diameter);
		break;
	}
	case 4:
	{
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L1", m_StepData[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"A1", m_StepData[0].m_angle);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D1", m_StepData[0].m_diameter);

		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L2", m_StepData[1].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"A2", m_StepData[1].m_angle);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D3", m_StepData[1].m_diameter);

		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L3", m_StepData[2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"A3", m_StepData[2].m_angle);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D3", m_StepData[2].m_diameter);

		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L4", m_StepData[3].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"A4", m_StepData[3].m_angle);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D4", m_StepData[3].m_diameter);
		break;
	}
	default:
		break;
	}
	
	CViewUtil::ZoomExtent();
}

void CZYDJData::SetStepData(vector<ZYXDStepData> const& data)
{
	m_StepData = data;
}

void CZYDJData::SetPreDiameter(double diameter)
{
	m_Prediameter = diameter;
}

void CZYDJData::SetDaobing(CString Daobing)
{
	m_DaoBingName = Daobing;
}

void CZYDJData::SetLabberCount(int labberCount)
{
	m_DjLabberCount = labberCount;
}
