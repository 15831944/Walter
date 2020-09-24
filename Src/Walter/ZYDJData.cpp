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
	CString blkName = CCommonUtil::GenStrByTime();
	ptInsert.x += m_totalLength ;
	AcDbObjectId DaoShenId = CBlockUtil::InsertDwgAsBlockRef(DaoShenFullPath, blkName, ACDB_MODEL_SPACE, ptInsert, 0, 1);

	//设置总长
	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"Lf", m_grooveLength);
	CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"L", m_totalLength);
	// 如果是扩孔刀需要设置预孔直径
	if (m_IsKKD)
	{
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, L"D0", m_Prediameter);
		InsertPreDiaDim(ptInsert);
	}
	//设置参数数据
	for (int i = 0; i < m_StepData.size(); i++)
	{
		CString temp;
		temp.Format(L"L%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, temp, m_StepData[i].m_stepLength);
		temp.Format(L"A%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, temp, m_StepData[i].m_angle);
		temp.Format(L"D%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(DaoShenId, temp, m_StepData[i].m_diameter);
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
	if (m_IsKKD) {
		if (data.size() >= 2)
			m_grooveLength = data[data.size() - 2].m_stepLength + data[data.size() - 1].m_diameter;
		else
			m_grooveLength = data[data.size() - 1].m_stepLength + data[data.size() - 1].m_diameter;

	}
	else {
		m_grooveLength = data[data.size() - 1].m_stepLength + data[data.size() - 1].m_diameter;
	}

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
	dimTextPosition.x += DIMDISTANCE;

	CString temp;
	temp.Format(L"%%%%C%s", removeLastZero(m_Prediameter));
	CDimensionUtil::AddDimAligned(pt1, pt2, dimTextPosition, temp);
}
/*
insert the dimension of diameter 
@return
@pnt : the insert point
*/
void CZYDJData::InsertDiaDim(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	for (int i = 0; i < m_DjLabberCount; i++)
	{
		AcGePoint3d TopPoint = GetVertexPoint(pnt, i, TRUE);
		AcGePoint3d BottomPoint = GetVertexPoint(pnt, i, FALSE);
		AcGePoint3d centerPoint = CMathUtil::GetMidPoint(TopPoint, BottomPoint);
		centerPoint.x = pnt.x + DIMDISTANCE * i + 20;
		CString temp;
		temp.Format(L"%%%%C%s", removeLastZero(m_StepData[i].m_diameter));
		CDimensionUtil::AddDimAligned(TopPoint, BottomPoint, centerPoint,temp);
	}
}
/*
 @return 
 @pnt : the insert point
*/
void CZYDJData::InsertLenDim(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	//如果是铰刀，还需要再添加一个标注
	int count = m_IsKKD ? m_DjLabberCount - 1 : m_DjLabberCount;
	for (size_t i = 0; i < count; i++)
	{
		AcGePoint3d TopPoint;
		TopPoint.x = pnt.x - m_StepData[i].m_stepLength;
		TopPoint.y = pnt.y + m_StepData[i].m_diameter / 2.0;
		AcGePoint3d ptend(pnt);
		ptend.y = pnt.y + m_StepData[0].m_diameter / 2.0;
		AcGePoint3d centerPoint = CMathUtil::GetMidPoint(TopPoint, ptend);
		centerPoint.y = ptend.y + DIMDISTANCE * i + 10;
		CDimensionUtil::AddDimRotated(TopPoint, ptend, centerPoint,0, NULL);
	}
	
	// 插入总长标注
	AcGePoint3d ptInsert(pnt);
	ptInsert.y = pnt.y + m_StepData[0].m_diameter / 2.0;
	
	AcGePoint3d lastPoint(pnt);
	lastPoint.x = pnt.x - m_totalLength;
	lastPoint.y = pnt.y + m_StepData[m_DjLabberCount - 1].m_diameter / 2.0;
	AcGePoint3d centerPoint = CMathUtil::GetMidPoint(ptInsert, lastPoint);
	centerPoint.y = ptInsert.y + 60;
	CDimensionUtil::AddDimRotated(ptInsert, lastPoint, centerPoint, 0, NULL);

	//插入Lf标注
	lastPoint.y = pnt.y + m_StepData[m_StepData.size() - 1].m_diameter / 2.0;
	lastPoint.x = pnt.x - m_grooveLength;
	centerPoint =  CMathUtil::GetMidPoint(ptInsert, lastPoint);
	centerPoint.y = ptInsert.y + 50;
	CDimensionUtil::AddDimRotated(ptInsert, lastPoint, centerPoint, 0, NULL);

}
/*
 @return 
 @pnt : the insert point
*/
void CZYDJData::InsertAngleDim(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	for (int i = 1; i < m_DjLabberCount; i++)
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
		dimTextPosition.x -= (DIMDISTANCE * i);
		CDimensionUtil::AddDim2LineAngular(TopPoint2, TopPoint1, BottomPoint2, BottomPoint1, dimTextPosition);
	}
}
/*
	the function is used to mend the gap
	@return
	@pnt : the insert point
*/
void CZYDJData::Mending(AcGePoint3d const & pnt)
{
	//轮廓线为图层1
	CLayerSwitch layer(L"1");
	AcGePoint3d lastTopPoint(pnt);
	lastTopPoint.x -= m_totalLength;

	auto diff = [=]() {
		return m_IsKKD ? 0 : 0.5;
	};

	lastTopPoint.y =lastTopPoint.y + m_StepData[m_StepData.size() - 1].m_diameter / 2.0 - diff();
	AcGePoint3d lastBottomPoint(lastTopPoint);
	lastBottomPoint.y = lastBottomPoint.y - m_StepData[m_StepData.size() - 1].m_diameter +2* diff();
	CLineUtil::CreateLine(lastTopPoint, lastBottomPoint);
}
