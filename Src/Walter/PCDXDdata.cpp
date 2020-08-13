#include "stdafx.h"
#include "PCDXDdata.h"
#include "Entity/DimensionUtil.h"
#include "Entity/DynamicBlockUtil.h"
int PCDXD::Draw()
{
	CDocLock lock;


	offset = 0.75;
	//���뵶��
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"���������㣺", pnt);
	CString Handle = TY_GetDaoBingFolder() + m_handleName + L".dwg";
	CBlockUtil::InsertDwgAsBlockRef(Handle, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);
	//���뵶��
	AcGePoint3d ptInsert(std::move(pnt));
	double handleLength = GetDisByDBName(m_handleName);
	ptInsert.x = ptInsert.x + m_totalLength - offset;
	CString DaoShen = TY_GetDynamicBlockFolder() + L"PCDϳ��ģ��.dwg";
	AcDbObjectId DaoShenId = CBlockUtil::InsertDwgAsBlockRef(DaoShen, NULL, ACDB_MODEL_SPACE, ptInsert, 0, 1);


	//��̬���������
	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L", m_totalLength - handleLength );

	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"Dc", m_diameter);
	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"Lc", m_bladeLength);


	//���ֱ����ע
	AcGePoint3d topPoint(ptInsert.x + offset, ptInsert.y + m_diameter / 2.0, ptInsert.z);
	AcGePoint3d bottomPoint(ptInsert.x + offset, ptInsert.y - m_diameter / 2.0, ptInsert.z);
	AcGePoint3d center = CMathUtil::GetMidPoint(topPoint, bottomPoint);
	center.x += 10;
	CString temp;
	temp.Format(L"%%%%C%s", removeLastZero(m_diameter));
	CDimensionUtil::AddDimAligned(topPoint, bottomPoint, center, temp, NULL);

	//����ܳ���ע
	AcGePoint3d LendPoint(topPoint);
	LendPoint.x -= m_totalLength;
	center =  CMathUtil::GetMidPoint(topPoint, LendPoint);
	center.y += 15;
	CDimensionUtil::AddDimRotated(topPoint, LendPoint, center, NULL, NULL);
	
	
	//���lc��ע
	AcGePoint3d LcendPoint(topPoint);
	LcendPoint.x -= m_bladeLength;
	center = CMathUtil::GetMidPoint(topPoint, LcendPoint);
	center.y += 5;
	CDimensionUtil::AddDimRotated(topPoint, LcendPoint, center, NULL, NULL);
	//���lf��ע 
	AcGePoint3d LfendPoint(topPoint);
	LfendPoint.x -= m_grooveLength;
	center = CMathUtil::GetMidPoint(topPoint, LfendPoint);
	center.y += 10;
	CDimensionUtil::AddDimRotated(topPoint, LfendPoint, center, NULL, NULL);
	return 0;
}

