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
		firstTopPoint.x -= m_stepDatas[stepIndex-1].m_stepLength;
		firstTopPoint.y += m_stepDatas[stepIndex].m_diameter / 2 - m_stepDatas[0].m_diameter / 2;

		if (!isTop)
			firstTopPoint.y -= m_stepDatas[stepIndex].m_diameter;
		return firstTopPoint;
	}
}

//����ֱ����ע
void SPCDJDData::InsertDDiamension(const AcGePoint3d& pnt,int stepIndex)
{
	AcGePoint3d ptTop = GetDaoJianPoint(pnt, true, stepIndex);
	AcGePoint3d ptBottom = GetDaoJianPoint(pnt, false, stepIndex);
	AcGePoint3d ptCenter(pnt);

	//
	ptCenter.x += 15 + stepIndex * 15;
	
	CDimensionUtil::AddDimAligned(ptTop, ptBottom, ptCenter, NULL);
}
//����L��ע
void SPCDJDData::InsertLDiamension(const AcGePoint3d & pnt, int stepIndex)
{
	if (stepIndex == 0)
	{
		return;
	}
	//L��ע�����յ�
	AcGePoint3d LDstart = GetDaoJianPoint(pnt, true, 0);
	AcGePoint3d LDend = GetDaoJianPoint(pnt, true, stepIndex);
	CString dimension;
	dimension.Format(L"L%d", stepIndex);

	AcGePoint3d center = CMathUtil::GetMidPoint(LDstart, LDend);
	center.y = LDend.y;
	AcDbObjectId dimId = CDimensionUtil::AddDimRotated(LDstart, LDend, center,0);
}
//����Lf1��ע
void SPCDJDData::InsertLf1Dimension(const AcGePoint3d & pnt, int stepIndex)
{
	AcGePoint3d LfDstart = GetDaoJianPoint(pnt, true, 0);
	AcGePoint3d LfDend = GetDaoJianPoint(pnt, true, stepIndex);

	LfDend.x -= 20;
	
	AcGePoint3d center = CMathUtil::GetMidPoint(LfDstart, LfDend);
	center.y = LfDend.y;
	CDimensionUtil::AddDimRotated(LfDstart, LfDend, center, 0);
}
//����offset��ע
void SPCDJDData::InsertOffsetDimension(const AcGePoint3d & pnt)
{
	//��ֱƫ��
	AcGePoint3d ptBottom = GetDaoJianPoint(pnt, false, 0);
	ptBottom.x -= 3.0;
	AcGePoint3d ptEdge(ptBottom);
	//ƫ�ƾ���Ϊ0.5
	ptEdge.y += 0.5;

	AcGePoint3d center = CMathUtil::GetMidPoint(ptBottom, ptEdge);
	center.x -= 5;
	CDimensionUtil::AddDimAligned(ptBottom, ptEdge, center, NULL);
	//ƽ��ƫ��
	double lenx1 = 0.5 / sin(CMathUtil::AngleToRadian(m_stepDatas[0].m_angle));

}
//������ƫ�Ƕȱ�ע
void SPCDJDData::InsertAngleDimension(const AcGePoint3d & pnt, int stepIndex)
{

}
//����60�ȱ�ע
void SPCDJDData::InsertSixtyDimension(const AcGePoint3d & pnt)
{

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
		
		CUpdateUtil::UpdateDisplay();
		CEntityUtil::ExplodeBlockToOwnSpace(daoShenID);

		radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
		width = GetWidthByDiameter(m_stepDatas[0].m_diameter);

		vAcDbObjectId blkRefIds;
		CBlockUtil::CycleAllBlockReferences(blkRefIds);
		for (int i = 0; i < blkRefIds.size(); i++)
		{
			CString name;
			CDynamicBlockUtil::GetDynamicBlockName(blkRefIds[i], name);
			if (name == L"����_1_T")
			{
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[0].m_angle);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", width);
			}
		}

		InsertDDiamension(pnt, 0);
		//����LF1
		InsertLf1Dimension(pnt, 0);
		InsertOffsetDimension(pnt);
		break;
	}
	case 2:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);
		
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
	
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[ 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);

		CUpdateUtil::UpdateDisplay();
		CEntityUtil::ExplodeBlockToOwnSpace(daoShenID);

		
		vAcDbObjectId blkRefIds;
		CBlockUtil::CycleAllBlockReferences(blkRefIds);
		for (int i = 0; i < blkRefIds.size(); i++)
		{
			CString name;
			CDynamicBlockUtil::GetDynamicBlockName(blkRefIds[i], name);
			if (name == L"����_1_T")
			{
				radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
				width = GetWidthByDiameter(m_stepDatas[0].m_diameter);

				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[0].m_angle);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", width);
			}

			if (name == L"����_2_T")
			{
				radius = GetRadiusByDiameter(m_stepDatas[1].m_diameter);
				width = GetWidthByDiameter(m_stepDatas[1].m_diameter);

				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[1].m_angle);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", width);
			}
		}


		InsertDDiamension(pnt, 0);
		InsertDDiamension(pnt, 1);

		InsertLDiamension(pnt, 1);
		//����LF1
		InsertLf1Dimension(pnt, 1);
		break;
	}
	case 3:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);
		
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[2].m_stepLength);

		CUpdateUtil::UpdateDisplay();
		CEntityUtil::ExplodeBlockToOwnSpace(daoShenID);


		vAcDbObjectId blkRefIds;
		CBlockUtil::CycleAllBlockReferences(blkRefIds);
		for (int i = 0; i < blkRefIds.size(); i++)
		{
			CString name;
			CDynamicBlockUtil::GetDynamicBlockName(blkRefIds[i], name);
			if (name == L"����_1_T")
			{
				radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
				width = GetWidthByDiameter(m_stepDatas[0].m_diameter);

				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[0].m_angle);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", width);
			}

			if (name == L"����_2_T")
			{
				radius = GetRadiusByDiameter(m_stepDatas[1].m_diameter);
				width = GetWidthByDiameter(m_stepDatas[1].m_diameter);

				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[1].m_angle);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", width);
			}

			if (name == L"����_3_T")
			{
				radius = GetRadiusByDiameter(m_stepDatas[2].m_diameter);
				width = GetWidthByDiameter(m_stepDatas[2].m_diameter);

				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[2].m_angle);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", width);
			}
		}

		
		InsertDDiamension(pnt, 0);
		InsertDDiamension(pnt, 1);
		InsertDDiamension(pnt, 2 );
		InsertLDiamension(pnt, 1);
		InsertLDiamension(pnt, 2);
		//����LF1
		InsertLf1Dimension(pnt, 2);
		break;
	}
	case 4:
	{
		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
		
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);

		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[2].m_stepLength);
		
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D4", m_stepDatas[3].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L4", m_stepDatas[3].m_stepLength);
		
		
		CUpdateUtil::UpdateDisplay();
		CEntityUtil::ExplodeBlockToOwnSpace(daoShenID);


		vAcDbObjectId blkRefIds;
		CBlockUtil::CycleAllBlockReferences(blkRefIds);
		for (int i = 0; i < blkRefIds.size(); i++)
		{
			CString name;
			CDynamicBlockUtil::GetDynamicBlockName(blkRefIds[i], name);
			if (name == L"����_1_T")
			{
				radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
				width = GetWidthByDiameter(m_stepDatas[0].m_diameter);

				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[0].m_angle);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", width);
			}

			if (name == L"����_2_T")
			{
				radius = GetRadiusByDiameter(m_stepDatas[1].m_diameter);
				width = GetWidthByDiameter(m_stepDatas[1].m_diameter);

				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[1].m_angle);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", width);
			}

			if (name == L"����_3_T")
			{
				radius = GetRadiusByDiameter(m_stepDatas[2].m_diameter);
				width = GetWidthByDiameter(m_stepDatas[2].m_diameter);

				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[2].m_angle);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", width);
			}

			if (name == L"����_4_T")
			{
				radius = GetRadiusByDiameter(m_stepDatas[3].m_diameter);
				width = GetWidthByDiameter(m_stepDatas[3].m_diameter);

				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[3].m_angle);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", width);
			}
		}


		InsertDDiamension(pnt, 0);
		InsertDDiamension(pnt, 1);
		InsertDDiamension(pnt, 2);
		InsertDDiamension(pnt, 3);
		InsertLDiamension(pnt, 1);
		InsertLDiamension(pnt, 2);
		InsertLDiamension(pnt, 3);
		InsertLf1Dimension(pnt, 3);
		break;
	}
	default:
		break;
	}

	CViewUtil::ZoomExtent();
	return 0;
}