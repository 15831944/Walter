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
	CGetInputUtil::GetPoint(L"请输入插入点：", pnt);

	//设置刀柄刀身文件
	CString labberCountStr;
	labberCountStr.Format(L"%d", m_DjLabberCount);
	CString DaoShenFullPath;
	CString DaoBingFullPath = TY_GetDaoBingZyFolder() + m_DaoBingName + L".dwg";
	if (m_IsKKD)
		DaoShenFullPath = TY_GetDynamicBlockFolder() + L"整硬扩孔刀模板X"+ labberCountStr + L".dwg";
	else
		DaoShenFullPath = TY_GetDynamicBlockFolder() + L"整硬铰刀模板X"+ labberCountStr + L".dwg";
	
	AcDbObjectId DaoBingId = CBlockUtil::InsertDwgAsBlockRef(DaoBingFullPath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);
	//计算刀身插入点
	double dis = GetHandleLengthFromDaoBing(m_DaoBingName);
	AcGePoint3d ptInsert(pnt);
	ptInsert.x += m_totalLength ;
	AcDbObjectId DaoShenId = CBlockUtil::InsertDwgAsBlockRef(DaoShenFullPath, NULL, ACDB_MODEL_SPACE, ptInsert, 0, 1);

	//设置总长
	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L", m_totalLength);
	// 如果是扩孔刀需要设置预孔直径
	if (m_IsKKD)
	{
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D0",m_Prediameter);
		InsertPreDiaDim(ptInsert);
	}

	//设置参数数据
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
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D2", m_StepData[1].m_diameter);

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
	//插入标注
	InsertDiaDim(ptInsert);
	InsertAngleDim(ptInsert);
	InsertLenDim(ptInsert);
	//补线
	Mending(ptInsert);

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
//获取顶点，index表示第几个顶点
AcGePoint3d CZYDJData::GetVertexPoint(const AcGePoint3d & pnt,int index,bool isTop)
{
	AcGePoint3d destPoint;
	double diff = 0.0f;
	if (index >= 1) {
		diff = (m_StepData[index].m_diameter - m_StepData[index - 1].m_diameter) / 2.0;
	}
	else
	{
		diff = (m_StepData[index].m_diameter - m_StepData[0].m_diameter) / 2.0;
	}
	//对称,角度平分
	double angle = m_StepData[index].m_angle / 2.0;
	double distance_x = diff / tan(CMathUtil::AngleToRadian(angle));
	//同在一个xoy平面内
	destPoint.z = pnt.z;
	if (isTop)
		destPoint.y = pnt.y + m_StepData[index].m_diameter / 2.0;
	else
		destPoint.y = pnt.y - m_StepData[index].m_diameter / 2.0;
	
	if (index == 0)
		destPoint.x = pnt.x;
	else
		destPoint.x = pnt.x - m_StepData[index-1].m_stepLength - distance_x;
	return destPoint;
}
//插入预孔直径标注
void CZYDJData::InsertPreDiaDim(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	AcGePoint3d pt1(pnt);
	pt1.y = pnt.y + m_Prediameter / 2.0;
	AcGePoint3d pt2(pnt);
	pt2.y = pnt.y -  m_Prediameter / 2.0;
	AcGePoint3d dimTextPosition = CMathUtil::GetMidPoint(pt1,pt2);
	dimTextPosition.x += 5;

	CString temp;
	temp.Format(L"%%%%C%s", removeLastZero(m_Prediameter));
	CDimensionUtil::AddDimAligned(pt1, pt2, dimTextPosition, temp);
}
//插入直径标注
void CZYDJData::InsertDiaDim(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	for (size_t i = 0; i < m_DjLabberCount; i++)
	{
		AcGePoint3d TopPoint = GetVertexPoint(pnt, i, TRUE);
		AcGePoint3d BottomPoint = GetVertexPoint(pnt, i, FALSE);
		AcGePoint3d centerPoint = CMathUtil::GetMidPoint(TopPoint, BottomPoint);
		centerPoint.x = pnt.x + 8 * i + 12;
		CString temp;
		temp.Format(L"%%%%C%s", removeLastZero(m_StepData[i].m_diameter));
		CDimensionUtil::AddDimAligned(TopPoint, BottomPoint, centerPoint,temp);
	}
}
//插入长度标注
void CZYDJData::InsertLenDim(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	for (size_t i = 0; i < m_DjLabberCount-1; i++)
	{
		AcGePoint3d TopPoint;
		TopPoint.x = pnt.x - m_StepData[i].m_stepLength;
		TopPoint.y = pnt.y + m_StepData[i].m_diameter / 2.0;
		AcGePoint3d ptend(pnt);
		ptend.y = TopPoint.y;
		AcGePoint3d centerPoint = CMathUtil::GetMidPoint(TopPoint, ptend);
		centerPoint.y = centerPoint.y + 5 * i + 5;
		CDimensionUtil::AddDimRotated(TopPoint, ptend, centerPoint,0, NULL);
	}
	
	// 插入总长标注
	//double dis = GetHandleLengthFromDaoBing(m_DaoBingName);
	AcGePoint3d ptInsert(pnt);
	ptInsert.y = pnt.y + m_StepData[0].m_diameter / 2.0;
	
	AcGePoint3d lastPoint(pnt);
	lastPoint.x = pnt.x - m_totalLength;
	lastPoint.y = pnt.y + m_StepData[m_DjLabberCount - 1].m_diameter / 2.0;
	AcGePoint3d centerPoint = CMathUtil::GetMidPoint(ptInsert, lastPoint);
	centerPoint.y = centerPoint.y + 30;
	CDimensionUtil::AddDimRotated(ptInsert, lastPoint, centerPoint, 0, NULL);

	//插入Lf标注
	lastPoint = pnt;
	lastPoint.y = pnt.y + m_StepData[m_StepData.size() - 1].m_diameter / 2.0;
	auto diff = [&](){
		return m_IsKKD ? m_StepData[m_StepData.size() - 1].m_diameter : 0.5 * m_StepData[m_StepData.size() - 1].m_diameter;
	};
	lastPoint.x = pnt.x - m_StepData[m_StepData.size() - 2].m_stepLength - diff();
	centerPoint =  CMathUtil::GetMidPoint(ptInsert, lastPoint);
	centerPoint.y = centerPoint.y + 23;
	CDimensionUtil::AddDimRotated(ptInsert, lastPoint, centerPoint, 0, NULL);

}
//插入角度标注
void CZYDJData::InsertAngleDim(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	for (size_t i = 1; i < m_DjLabberCount; i++)
	{
		
		double diff = (m_StepData[i].m_diameter - m_StepData[i - 1].m_diameter) / 2.0;

		AcGePoint3d TopPoint1 = GetVertexPoint(pnt, i, TRUE);
		AcGePoint3d TopPoint2(TopPoint1);
		/*TopPoint2.y -= diff;
		TopPoint2.x = TopPoint1.x +  diff / tan(CMathUtil::AngleToRadian(m_StepData[i].m_angle / 2.0));*/
		TopPoint2.x = pnt.x - m_StepData[i-1].m_stepLength;
		TopPoint2.y = TopPoint2.y - diff;
		AcGePoint3d BottomPoint1 = GetVertexPoint(pnt, i, FALSE);
		AcGePoint3d BottomPoint2(BottomPoint1);
		BottomPoint2.y += diff;
		BottomPoint2.x = pnt.x - m_StepData[i-1].m_stepLength;
		/*BottomPoint2.x = BottomPoint1.x +  diff / tan(CMathUtil::AngleToRadian(m_StepData[i].m_angle / 2.0));*/
		//计算插入标记位置
		AcGePoint3d dimTextPosition = CMathUtil::GetMidPoint(TopPoint1, BottomPoint1);
		dimTextPosition.x -= (10 * i);
		CDimensionUtil::AddDim2LineAngular(TopPoint2, TopPoint1, BottomPoint2, BottomPoint1, dimTextPosition);
	}
}
//如果刀柄直径小于刀身直径,则需要将刀的缺口补上
void CZYDJData::Mending(AcGePoint3d const & pnt)
{
	//轮廓线为图层1
	CLayerSwitch layer(L"1");
	AcGePoint3d lastTopPoint(pnt);
	lastTopPoint.x -= m_totalLength;

	auto diff = [&]() {
		return m_IsKKD ? 0 : 0.5;
	};

	lastTopPoint.y =lastTopPoint.y + m_StepData[m_StepData.size() - 1].m_diameter / 2.0 - diff();
	AcGePoint3d lastBottomPoint(lastTopPoint);
	lastBottomPoint.y = lastBottomPoint.y - m_StepData[m_StepData.size() - 1].m_diameter +2* diff();
	CLineUtil::CreateLine(lastTopPoint, lastBottomPoint);
}
