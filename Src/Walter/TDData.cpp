#include "stdafx.h"
#include "TDData.h"
#include "Entity/DimensionUtil.h"
#include "Entity/DynamicBlockUtil.h"


void CTDData::Draw()
{
	CDocLock lock;

	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"请输入插入点：",pnt);
	//插入刀柄
	CString Handle = TY_GetDaoBingFolder() + m_HandleName + L".dwg";
	CBlockUtil::InsertDwgAsBlockRef(Handle, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);
	//插入刀身
	CString temp;
	temp.Format(L"%d",m_tdSegdata.size());
	CString KnifeName = TY_GetDynamicBlockFolder() + L"可换刀片镗刀模板X" + temp + L".dwg";
	AcGePoint3d ptInsert(pnt);
	ptInsert.x += m_totallength;
	AcGePoint3d firstPoint = getVertexPoint(ptInsert, 0, true);
	ptInsert.x += (ptInsert.x - firstPoint.x);
	AcDbObjectId KnifeId = CBlockUtil::InsertDwgAsBlockRef(KnifeName, NULL, ACDB_MODEL_SPACE, ptInsert, 0, 1);

	
	double dis = GetDisByDBName(m_HandleName);
	CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, L"L", m_totallength - dis);
	
	//设置动态块参数
	for (size_t i = 0; i < m_tdSegdata.size(); ++i)
	{
		CString temp;
		//角度
		temp.Format(L"A%d", i + 1);
		if (i == m_tdSegdata.size() - 1) {
			CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, L"A", m_tdSegdata[i].m_angle);
		}
		else
		{
			CDynamicBlockUtil::SetDynamicBlockValue(KnifeId,temp, m_tdSegdata[i].m_angle);
		}
		//直径
		temp.Format(L"D%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, temp, m_tdSegdata[i].m_diameter);
		//长度
		temp.Format(L"L%d", i + 1);
		if (i == m_tdSegdata.size() - 1) {
			CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, temp, m_tdSegdata[i].m_len - dis);
		}
		else {
			CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, temp, m_tdSegdata[i].m_len);
		}

	}
	m_grooveLength = m_tdSegdata.size() <= 1 ? 30: m_tdSegdata[m_tdSegdata.size() - 2].m_len +30;
	CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, L"Lf", m_grooveLength);
	//直径标注
	AddDiaDimension(ptInsert);
	//长度标注
	AddLenDimension(ptInsert);
	//角度标注
	AddAngleDimension(ptInsert);
	//补线
	Mending(ptInsert);
}

AcGePoint3d CTDData::getVertexPoint(const AcGePoint3d & pnt, int index,bool IsTop)
{
	
	double offset = 0.0f;
	AcGePoint3d destPnt(pnt);
	const double height = 8.0f;
	offset = m_tdSegdata[0].m_angle == 90 ? 0 : height / tan(CMathUtil::AngleToRadian(m_tdSegdata[0].m_angle));
	AcGePoint3d firstTopPoint(pnt);
	firstTopPoint.x -= offset;
	
	destPnt.x = index == 0 ? firstTopPoint.x : firstTopPoint.x - m_tdSegdata[index-1].m_len;
	destPnt.y = IsTop ? pnt.y + m_tdSegdata[index].m_diameter / 2.0 : pnt.y - m_tdSegdata[index].m_diameter / 2.0;

	return destPnt;
}

void CTDData::AddDiaDimension(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	for (int i=0;i <  m_tdSegdata.size();i++)
	{
		AcGePoint3d ptTop = getVertexPoint(pnt, i, true);
		AcGePoint3d ptBottom = getVertexPoint(pnt, i, false);
		AcGePoint3d center = CMathUtil::GetMidPoint(ptTop, ptBottom);
		center.x =pnt.x +  i * 10 + 10 ;
		CString temp;
		temp.Format(L"%%%%C%s", removeLastZero(m_tdSegdata[i].m_diameter));
		CDimensionUtil::AddDimAligned(ptTop, ptBottom, center, temp, nullptr);
	}
}

void CTDData::AddLenDimension(const AcGePoint3d & pnt)
{

	CLayerSwitch layer(DIMLAYERNAME);
	AcGePoint3d firstTopPoint(getVertexPoint(pnt, 0, true));
	int i;
	for (i =1;i < m_tdSegdata.size() ;i++)
	{
		AcGePoint3d topPoint = getVertexPoint(pnt, i, true);
		AcGePoint3d center = CMathUtil::GetMidPoint(firstTopPoint, topPoint);
		center.y = firstTopPoint.y +  i *10 + 10;
		CDimensionUtil::AddDimRotated(firstTopPoint, topPoint, center, 0, nullptr, nullptr);
	}
	//计算排屑槽长
	m_grooveLength = m_tdSegdata.size() <= 1 ? 30 : m_tdSegdata[m_tdSegdata.size() - 2].m_len + 30;
	//添加lf标注
	AcGePoint3d endPoint1(firstTopPoint.x, pnt.y + m_tdSegdata[m_tdSegdata.size() - 1].m_diameter / 2.0, firstTopPoint.z);
	endPoint1.x -= m_grooveLength;
	AcGePoint3d  center = CMathUtil::GetMidPoint(firstTopPoint, endPoint1);
	center.y = firstTopPoint.y + i * 10 +10;
	CDimensionUtil::AddDimRotated(firstTopPoint, endPoint1, center, 0, nullptr, nullptr);
	//添加总长标注
	AcGePoint3d endPoint(firstTopPoint.x,firstTopPoint.y,firstTopPoint.z);
	firstTopPoint.x -= m_totallength;
	center = CMathUtil::GetMidPoint(firstTopPoint, endPoint);
	center.y = firstTopPoint.y + (i+1) *10 + 10;
	CDimensionUtil::AddDimRotated(firstTopPoint, endPoint, center, 0, nullptr, nullptr);
}

void CTDData::AddAngleDimension(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	//插入第一个角的标注
	double height = 8.0f;
	AcGePoint3d firstTopPoint = getVertexPoint(pnt, 0, true);
	AcGePoint3d secondPoint(pnt.x, firstTopPoint.y - height, pnt.z);
	AcGePoint3d thirdPoint(pnt.x, firstTopPoint.y, pnt.z);
	AcGePoint3d center = CMathUtil::GetMidPoint(secondPoint, thirdPoint);
	CDimensionUtil::AddDim2LineAngular(firstTopPoint, secondPoint, firstTopPoint, thirdPoint, center, nullptr, nullptr);
	//插入其他角的标注
	for (int i=1;i < m_tdSegdata.size();i++)
	{

		AcGePoint3d topPoint = getVertexPoint(pnt, i, true);
		double height = (m_tdSegdata[i].m_diameter - m_tdSegdata[i - 1].m_diameter) / 2.0;
		double offset = m_tdSegdata[i].m_angle == 90 ? 0 : height / tan(CMathUtil::AngleToRadian(m_tdSegdata[i].m_angle));
		secondPoint.x = topPoint.x + offset;
		secondPoint.y = topPoint.y - height;
		thirdPoint.x = secondPoint.x;
		thirdPoint.y = topPoint.y;

		center = CMathUtil::GetMidPoint(secondPoint, thirdPoint);
		center.x += 3;
		center.y -= 3;
		CDimensionUtil::AddDim2LineAngular(topPoint, secondPoint, topPoint, thirdPoint, center, nullptr, nullptr);
	}
}

void CTDData::Mending(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(L"1");
	double dis = GetDisByDBName(m_HandleName);
	AcGePoint3d firstTopPoint = getVertexPoint(pnt, 0, true);
	AcGePoint3d TopPoint = getVertexPoint(pnt, (int)m_tdSegdata.size() - 1, true);
	AcGePoint3d BottomPoint = getVertexPoint(pnt, (int)m_tdSegdata.size() - 1, false);
	TopPoint.x = firstTopPoint.x - m_totallength + dis;
	BottomPoint.x = firstTopPoint.x - m_totallength + dis;
	CLineUtil::CreateLine(TopPoint, BottomPoint);
}
