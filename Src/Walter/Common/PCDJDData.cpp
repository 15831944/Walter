#include "stdafx.h"
#include "PCDJDData.h"
#include "Entity/DimensionUtil.h"
#include "Entity/DynamicBlockUtil.h"
#include "Com.h"
#include "Interaction/UpdateUtil.h"
#include <cmath>
static HINSTANCE s_gTyToolInst = 0;

AcGePoint3d SPCDJDData::GetDaoJianPoint(const AcGePoint3d& pnt, bool isTop, int stepIndex)
{
	if (stepIndex > m_stepDatas.size()-1)
	{
		return AcGePoint3d(0,0,0);
	}

	double width = GetWidthByDiameter(m_stepDatas[0].m_diameter);
	double lenx2 = width / tan(CMathUtil::AngleToRadian(m_stepDatas[0].m_angle));
	double lenx1 = 0.5 / sin(CMathUtil::AngleToRadian(m_stepDatas[0].m_angle));
	double firstOffsetX = lenx2 - lenx1;

	AcGePoint3d firstTopPoint = pnt;
	firstTopPoint.x -= firstOffsetX;
	firstTopPoint.y += m_stepDatas[0].m_diameter / 2;

	if (stepIndex == 0)
	{
		if (!isTop)
			firstTopPoint.y -= m_stepDatas[0].m_diameter;
		return firstTopPoint;
	}

	else
	{
		firstTopPoint.x -= m_stepDatas[stepIndex].m_stepLength;
		firstTopPoint.y += m_stepDatas[stepIndex].m_diameter / 2 - m_stepDatas[0].m_diameter / 2;

		if (!isTop)
			firstTopPoint.y -= m_stepDatas[stepIndex].m_diameter;
		return firstTopPoint;
	}
}

//����ֱ����ע
void SPCDJDData::InsertDDiamension(const AcGePoint3d& pnt,int step)
{
	AcGePoint3d ptTop = GetDaoJianPoint(pnt, true, step);
	AcGePoint3d ptBottom = GetDaoJianPoint(pnt, false, step);
	AcGePoint3d ptCenter(pnt);

	//
	//ptCenter.x += step * 10 + 40;
	
	CDimensionUtil::AddDimAligned(ptTop, ptBottom, CMathUtil::GetMidPoint(ptTop,ptBottom), NULL);
}
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

	//Lf2
	double Lf2;

	//��Ƭ�İ뾶�Ϳ�
	double radius = 0;
	double width = 0;

	switch (m_stepNum)
	{
	case 1:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X1", L"����_1_T", L"An", m_stepDatas[0].m_angle);

		radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X1", L"����_1_T", L"R", radius);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X1", L"����_1_T", L"yLen", width);

		InsertDDiamension(pnt, 0);
		break;
	}
	case 2:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);
		
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X2", L"����_1_T", L"An", m_stepDatas[0].m_angle);
		radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X1", L"����_1_T", L"R", radius);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X1", L"����_1_T", L"yLen", width);
		InsertDDiamension(pnt, 0 );

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[ 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X2", L"����_2_T", L"An", m_stepDatas[1].m_angle);
		radius = GetRadiusByDiameter(m_stepDatas[1].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[1].m_diameter);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X2", L"����_1_T", L"R", radius);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X2", L"����_1_T", L"yLen", width);
		InsertDDiamension(pnt, 1);
		break;
	}
	case 3:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_1_T", L"An", m_stepDatas[0].m_angle);
		radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_1_T", L"R", radius);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_1_T", L"yLen", width);
		InsertDDiamension(pnt, 0 );

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_2_T", L"An", m_stepDatas[1].m_angle);
		radius = GetRadiusByDiameter(m_stepDatas[1].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[1].m_diameter);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_2_T", L"R", radius);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_2_T", L"yLen", width);
		InsertDDiamension(pnt, 1 );

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[2].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_3_T", L"An", m_stepDatas[2].m_angle);
		radius = GetRadiusByDiameter(m_stepDatas[2].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[2].m_diameter);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_3_T", L"R", radius);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X3", L"����_3_T", L"yLen", width);
		InsertDDiamension(pnt, 2 );
		break;
	}
	case 4:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_1_T", L"An", m_stepDatas[0].m_angle); 
		radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_1_T", L"R", radius);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_1_T", L"yLen", width);
		InsertDDiamension(pnt, 0 );

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_2_T", L"yLen", m_stepDatas[1].m_angle);
		radius = GetRadiusByDiameter(m_stepDatas[1].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[1].m_diameter);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_2_T", L"R", radius);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_2_T", L"yLen", width);
		InsertDDiamension(pnt, 1);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[2].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_3_T", L"An", m_stepDatas[2].m_angle);
		radius = GetRadiusByDiameter(m_stepDatas[2].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[2].m_diameter);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_3_T", L"R", radius);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_3_T", L"yLen", width);
		InsertDDiamension(pnt, 2 );

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D4", m_stepDatas[3].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L4", m_stepDatas[3].m_stepLength);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_4_T", L"An", m_stepDatas[3].m_angle);
		radius = GetRadiusByDiameter(m_stepDatas[3].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[3].m_diameter);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_4_T", L"R", radius);
		CDynamicBlockUtil::SetDynamicValueInDynamicBlock(L"PCD�µ�ģ��X4", L"����_4_T", L"yLen", width);
		InsertDDiamension(pnt, 3 );
		break;
	}
	default:
		break;
	}

	CViewUtil::ZoomExtent();
	return 0;
}