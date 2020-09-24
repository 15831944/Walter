#include "stdafx.h"
#include "PCDZTData.h"
#include "Entity/DimensionUtil.h"
#include "Entity/DynamicBlockUtil.h"


CPCDZTData::CPCDZTData()
{
}


CPCDZTData::~CPCDZTData()
{
}

void CPCDZTData::Draw()
{
	CDocLock lock;
	//插入刀柄
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"请输入插入点的坐标:",pnt);
	double dis = GetDisByDBName(m_handleName);
	CString handlePath = TY_GetDaoBingFolder() + m_handleName + L".dwg";
	CBlockUtil::InsertDwgAsBlockRef(handlePath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);
	//插入刀身
	
	AcGePoint3d ptInsert(pnt);
	ptInsert.x = pnt.x + m_totalLen;
	CString labberCountStr;
	labberCountStr.Format(L"%d", m_StepData.size());
	CString knife = TY_GetDynamicBlockFolder() + L"PCD钻头模板X" + labberCountStr + L".dwg";
	AcDbObjectId knifeId = CBlockUtil::InsertDwgAsBlockRef(knife, NULL, ACDB_MODEL_SPACE, ptInsert, 0, 1);
	//设置动态块属性
	m_totalLen -= dis;
	CDynamicBlockUtil::SetDynamicBlockValue(knifeId, L"L", m_totalLen );
	//排屑槽长
	CDynamicBlockUtil::SetDynamicBlockValue(knifeId, L"lf", m_grooveLenth);

	//设置每个阶梯的约束值
	CString temp;
	for (int i=0;i < m_StepData.size() ;i++)
	{
		if (i == 0)
			temp.Format(L"A");
		else
			temp.Format(L"A%d", i);
		//角度
		CDynamicBlockUtil::SetDynamicBlockValue(knifeId, temp, m_StepData[i].angle);
		//直径
		temp.Format(L"D%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(knifeId, temp, m_StepData[i].diameter);
		//设置pcd片的大小
		double xlen = getXlen(m_StepData[i].diameter);
		double ylen = getYlen(m_StepData[i].diameter);
		double R = getR(m_StepData[i].diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(knifeId, L"R", R);
		CDynamicBlockUtil::SetDynamicBlockValue(knifeId, L"Ylen", ylen);
		CDynamicBlockUtil::SetDynamicBlockValue(knifeId, L"Xlen", xlen);
		//长度
		temp.Format(L"L%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(knifeId, temp, m_StepData[i].len);
	}
	CreateDims(ptInsert);
	//补线
	Mending(ptInsert);
}
//获取顶点坐标
AcGePoint3d CPCDZTData::getVertexPoint(AcGePoint3d const & pnt, int index, bool isTop)
{
	AcGePoint3d dest(0,0,0);
	double h = m_StepData[0].diameter / 2.0;
	double offset_x = m_StepData[0].angle == 180 ? 0 : h / tan(CMathUtil::AngleToRadian(m_StepData[0].angle / 2.0));
	AcGePoint3d firstTopPoint(pnt.x - offset_x, pnt.y + h, pnt.z);
	if (index == 0 && isTop) dest = firstTopPoint;
	else if (index == 0 && !isTop) dest= AcGePoint3d(firstTopPoint.x, firstTopPoint.y - 2.0*h, firstTopPoint.z);
	else {
		double diff_h = (m_StepData[index].diameter - m_StepData[index - 1].diameter) / 2.0;
		double offset_xx = m_StepData[index].angle == 180 ? 0 : diff_h / tan(CMathUtil::AngleToRadian(m_StepData[index].angle / 2.0));
		dest.x = firstTopPoint.x - m_StepData[index - 1].len - offset_xx;
		dest.y =isTop ?  pnt.y + m_StepData[index].diameter / 2.0 : pnt.y - m_StepData[index].diameter / 2.0;
		dest.z = pnt.z;
	}
	return dest;
}
//添加直径标注
void CPCDZTData::AddDiaDimension(AcGePoint3d const& pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	for (int i=0;i <m_StepData.size() ; i++)
	{
		AcGePoint3d TopPoint = getVertexPoint(pnt, i, true);
		AcGePoint3d BottomPoint = getVertexPoint(pnt, i, false);
		CString temp;
		temp.Format(L"%%%%C%s", removeLastZero(m_StepData[i].diameter));
		AcGePoint3d center = CMathUtil::GetMidPoint(TopPoint, BottomPoint);
		center.x = pnt.x + i *DIMDISTANCE + DIMDISTANCE;
		CDimensionUtil::AddDimAligned(TopPoint, BottomPoint, center, temp, nullptr);
	}

}
//添加长度标注
void CPCDZTData::AddLenDimension(AcGePoint3d const& pnt)
{
	double dis = GetDisByDBName(m_handleName);
	CLayerSwitch layer(DIMLAYERNAME);
	AcGePoint3d firstTopPoint = getVertexPoint(pnt, 0, true);
	int i = 0;
	for (i=0;i < m_StepData.size() -1;i ++ )
	{
		AcGePoint3d lastPoint = firstTopPoint;
		lastPoint.x -= m_StepData[i].len;
		lastPoint.y = i == 0 ? lastPoint.y : lastPoint.y + (m_StepData[i].diameter - m_StepData[0].diameter) / 2.0;
		AcGePoint3d center = CMathUtil::GetMidPoint(firstTopPoint, lastPoint);
		center.y = firstTopPoint.y + i * 10 + 10;
		CDimensionUtil::AddDimRotated(firstTopPoint, lastPoint, center, 0);
	}
	//标注一个总长
	AcGePoint3d lastPoint(pnt);
	lastPoint.x = pnt.x - m_totalLen -dis;
	AcGePoint3d center = CMathUtil::GetMidPoint(lastPoint, pnt);
	center.y = firstTopPoint.y + (i +1 ) * DIMDISTANCE + DIMDISTANCE;
	CDimensionUtil::AddDimRotated(pnt, lastPoint, center, 0);
	//标注一个lf
	AcGePoint3d lfendPoint = firstTopPoint;
	lfendPoint.y = lfendPoint.y + (m_StepData[m_StepData.size() - 1].diameter - m_StepData[0].diameter) / 2.0;
	lfendPoint.x -= m_grooveLenth;
	center = CMathUtil::GetMidPoint(firstTopPoint, lfendPoint);
	center.y = firstTopPoint.y + i * DIMDISTANCE + DIMDISTANCE;
	CDimensionUtil::AddDimRotated(firstTopPoint, lfendPoint, center, 0);
}
//添加角度标注
void CPCDZTData::AddAngleDimension(AcGePoint3d const& pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);

	for (int i=1;i < m_StepData.size();i++)
	{
		AcGePoint3d topPoint = getVertexPoint(pnt, 0, true);
		//夹角的第一条射线
		AcGePoint3d firstpoint(topPoint);
		firstpoint.x -= m_StepData[i-1].len;
		firstpoint.y = pnt.y + m_StepData[i - 1].diameter / 2.0;
		AcGePoint3d secondpoint = getVertexPoint(pnt, i, true);
		//夹角的第二条射线
		AcGePoint3d firstPoint1(firstpoint);
		firstPoint1.y -= m_StepData[i - 1].diameter;
		AcGePoint3d secondPoint1 = getVertexPoint(pnt, i, false);
		//标注数字的位置并添加标注
		AcGePoint3d dimtextposition = CMathUtil::GetMidPoint(secondpoint, secondPoint1);
		dimtextposition.x -= DIMDISTANCE;
		CDimensionUtil::AddDim2LineAngular(firstpoint, secondpoint, firstPoint1, secondPoint1, dimtextposition);
	}
	//插入一个顶角标注
	AcGePoint3d firstTopPoint = getVertexPoint(pnt, 0, true);
	AcGePoint3d bottomPoint = getVertexPoint(pnt, 0, false);
	AcGePoint3d center = CMathUtil::GetMidPoint(firstTopPoint, bottomPoint);
	center.x += 20;
	CDimensionUtil::AddDim2LineAngular(firstTopPoint, pnt, bottomPoint, pnt, center,nullptr,nullptr);
}
//补上缺口
void CPCDZTData::Mending(AcGePoint3d const & pnt)
{
	CLayerSwitch layer(L"1");
	double dis = GetDisByDBName(m_handleName);
	double offset_y = (m_StepData[m_StepData.size() - 1].diameter - 1) / 2.0;
	AcGePoint3d topPoint(pnt.x - m_totalLen , pnt.y + offset_y, pnt.z);
	AcGePoint3d BottomPoint(pnt.x-m_totalLen, pnt.y - offset_y, pnt.z);
	CLineUtil::CreateLine(topPoint, BottomPoint);
}

double CPCDZTData::getR(double diameter)
{
	double R = 14.0f;
	if (CMathUtil::IsBiger(diameter, 25.0))	R = 14.0;
	else if (CMathUtil::IsInRange(12.0, 25.0, diameter)) R = 10.0;
	else R = 6.0f;
	return R;
}

double CPCDZTData::getXlen(double diameter)
{
	double Xlen = 5.0f;
	if (CMathUtil::IsBiger(diameter, 25.0))	Xlen = 5.0;
	else if (CMathUtil::IsInRange(12.0, 25.0, diameter)) Xlen = 3.0;
	else Xlen = 2.0f;
	return Xlen;
}

double CPCDZTData::getYlen(double diameter)
{
	double ylen = 2.0f;
	if (CMathUtil::IsBiger(diameter, 25.0))	ylen = 2.0;
	else if (CMathUtil::IsInRange(12.0, 25.0, diameter)) ylen = 2.0;
	else ylen = 1.0f;
	return ylen;
}
