#include "stdafx.h"
#include "PCDJDData.h"
#include "Entity/DimensionUtil.h"
#include "Entity/DynamicBlockUtil.h"
#include "Com.h"
#include "Interaction/UpdateUtil.h"
#include "Others/ToolingUtil.h"

static HINSTANCE s_gTyToolInst = 0;

AcGePoint3d SPCDJDData::GetDaoJianPoint(const AcGePoint3d& pnt, bool isTop, int stepIndex)
{
	if (stepIndex > m_stepDatas.size()-1)
	{
		return AcGePoint3d(0,0,0);
	}

	double width = GetHeightByDiameter(m_stepDatas[0].m_diameter);
	double firstOffsetX = 0;
	AcGePoint3d firstTopPoint = pnt;
	if (m_stepDatas[0].m_angle != 90) {
		double lenx2 = width / tan(CMathUtil::AngleToRadian(m_stepDatas[0].m_angle));
		double lenx1 = 0.5 / sin(CMathUtil::AngleToRadian(m_stepDatas[0].m_angle));
		firstOffsetX = lenx2 - lenx1;
		firstTopPoint.x -= firstOffsetX;
		
	}
	else
	{
		firstOffsetX = 0.5;
		firstTopPoint.x += firstOffsetX;
	}

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

//出入直径标注
void SPCDJDData::InsertDDiamension(const AcGePoint3d& pnt,int stepIndex)
{
	CLayerSwitch layer(DIMLAYERNAME);
	AcGePoint3d ptTop = GetDaoJianPoint(pnt, true, stepIndex);
	AcGePoint3d ptBottom = GetDaoJianPoint(pnt, false, stepIndex);
	AcGePoint3d ptCenter(pnt);
	CString temp;
	temp.Format(L"%%%%C%.f", m_stepDatas[stepIndex].m_diameter);
	//
	ptCenter.x += 15 + stepIndex * 15;
	AcDbObjectId dimStyleId = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
	CDimensionUtil::AddDimAligned(ptTop, ptBottom, ptCenter, temp,dimStyleId);
}
//插入L标注
void SPCDJDData::InsertLDiamension(const AcGePoint3d & pnt, int stepIndex)
{
	CLayerSwitch layer(DIMLAYERNAME);
	if (stepIndex == 0)
	{
		return;
	}
	//L标注起点和终点
	AcGePoint3d LDstart = GetDaoJianPoint(pnt, true, 0);
	AcGePoint3d LDend = GetDaoJianPoint(pnt, true, stepIndex);

	AcGePoint3d center = CMathUtil::GetMidPoint(LDstart, LDend);
	center.y = LDend.y + 5*stepIndex;
	
	AcDbObjectId dimStyleId = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
	CDimensionUtil::AddDimRotated(LDstart, LDend, center,0,NULL, dimStyleId);
	
}
//插入Lf1标注
void SPCDJDData::InsertLf1Dimension(const AcGePoint3d & pnt, int stepIndex)
{
	CLayerSwitch layer(DIMLAYERNAME);
	double len = GetDisByDBName(m_daoBing);
	//插入Lf1标注
	AcGePoint3d LfDstart = GetDaoJianPoint(pnt, true, 0);
	AcGePoint3d LfDend = GetDaoJianPoint(pnt,true,stepIndex);
	//Lf1标注长度为最后L+30
	LfDend.x = LfDend.x - 30;

	AcGePoint3d center = CMathUtil::GetMidPoint(LfDstart, LfDend);
	center.y = LfDend.y + stepIndex * 5+ 10;
	AcDbObjectId dimStyleId = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
	CDimensionUtil::AddDimRotated(LfDstart, LfDend, center, 0,NULL, dimStyleId);
	//插入总长标注
	AcGePoint3d lastpoint(LfDstart);
	
	lastpoint.x = lastpoint.x - m_stepDatas[stepIndex].m_stepLength;
	center = CMathUtil::GetMidPoint(LfDstart, lastpoint);
	center.y = LfDend.y + stepIndex * 5 + 20;
	CDimensionUtil::AddDimRotated(LfDstart, lastpoint, center, 0, NULL, dimStyleId);
	////插入Lf2标注
	//double Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
	//AcGePoint3d lf2end(pnt);
	//lf2end.y = LfDstart.y -  Lf2;
	//center.x = pnt.x + 10;
	//center.y = lf2end.y;
	//CDimensionUtil::AddDimAligned(AcGePoint3d(pnt.x,LfDstart.y,LfDstart.z), lf2end, center, NULL, dimStyleId);
}
//插入offset标注
//void SPCDJDData::InsertOffsetDimension(const AcGePoint3d & pnt)
//{
//	//竖直偏移
//	AcGePoint3d ptBottom = GetDaoJianPoint(pnt, false, 0);
//	ptBottom.x -= 3.0;
//	AcGePoint3d ptEdge(ptBottom);
//	//偏移距离为0.5
//	ptEdge.y += 0.5;
//
//	AcGePoint3d center(0,0,0);
//	center.x = ptBottom.x - 15;
//	center.y = ptEdge.y - 15;
//	CDimensionUtil::AddDimAligned(ptBottom, ptEdge, center, NULL);
//	//平行偏移
//	ptBottom.x += 3.0;
//	//找到中点位置
//	double width = GetHeightByDiameter(m_stepDatas[0].m_diameter);
//	ptBottom.x += (width / 2.0) / tan(CMathUtil::AngleToRadian(m_stepDatas[0].m_angle));
//	ptBottom.y += width / 2.0;
//	double y = 0.5 * cos(CMathUtil::AngleToRadian(m_stepDatas[0].m_angle));
//	double x = 0.5 * sin(CMathUtil::AngleToRadian(m_stepDatas[0].m_angle));
//	//刀尖的另外一个顶点
//	AcGePoint3d ptPoint(0, 0, 0);
//	ptPoint.x = ptBottom.x  - x;
//	ptPoint.y = ptBottom.y + y;
//	//中心向下偏移
//	center.x = ptBottom.x + 15;
//	center.y = ptBottom.y - 15;
//	AcDbObjectId dimStyleId = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
//	CDimensionUtil::AddDimAligned(ptBottom, ptPoint,center , NULL,dimStyleId);
//}
//插入主偏角度标注
void SPCDJDData::InsertAngleDimension(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);
	for (int stepIndex = 0;stepIndex < m_stepNum;stepIndex++)
	{
		//角的顶点
		AcGePoint3d ptTop = GetDaoJianPoint(pnt, true, stepIndex);
		double y = GetHeightByDiameter(m_stepDatas[stepIndex].m_diameter);
		double x = m_stepDatas[stepIndex].m_angle != 90 ? y / tan(CMathUtil::AngleToRadian(m_stepDatas[stepIndex].m_angle)) : 0;
		//构成角的射线中两点
		AcGePoint3d ptEnd1(ptTop.x + 5, ptTop.y, 0);
		AcGePoint3d ptEnd2(ptTop.x + x, ptTop.y - y, 0);
		
		//标注点
		AcGePoint3d center(0, 0, 0);
		center.x = ptEnd1.x + 10;
		double len = center.x - ptTop.x;
		center.y = ptTop.y -  len*tan(CMathUtil::AngleToRadian(m_stepDatas[stepIndex].m_angle / 2.0));
		AcDbObjectId dimStyleId = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
		CDimensionUtil::AddDim2LineAngular(ptTop, ptEnd1, ptTop, ptEnd2, center, NULL,dimStyleId);
	}
}
//插入60度标注
//void SPCDJDData::InsertSixtyDimension(const AcGePoint3d & pnt)
//{
//	const double angle = 60.0;
//	AcGePoint3d ptBottom = GetDaoJianPoint(pnt, false, 0);
//	//60度角顶点
//	AcGePoint3d ptVertx(0,0,0);
//	ptVertx.y = pnt.y;
//	ptVertx.x = ptBottom.x - (pnt.y - ptBottom.y) / tan(CMathUtil::AngleToRadian(angle));
//
//	AcGePoint3d ptArcCenter(0, 0, 0);
//	ptArcCenter.x = pnt.x + 20 ;
//	ptArcCenter.y = ptVertx.y -  (ptArcCenter.x - ptVertx.x) * tan(CMathUtil::AngleToRadian(angle / 2.0));
//	AcDbObjectId dimStyleId = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
//	CDimensionUtil::AddDim2LineAngular(ptVertx,ptBottom,ptVertx,pnt,ptArcCenter,NULL,dimStyleId);
//
//}

void SPCDJDData::InsertOtherDimension(const AcGePoint3d & pnt)
{
	CLayerSwitch layer(DIMLAYERNAME);

	AcDbObjectId dimStyleId = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
	//刀宽标注 默认为7.0
	double xLen = 7.0;
	//最后一个刀尖顶点
	AcGePoint3d ptlastTopPoint = GetDaoJianPoint(pnt, true, m_stepNum - 1);
	CDimensionUtil::AddDimRotated(ptlastTopPoint, AcGePoint3d(ptlastTopPoint.x - 7.0, ptlastTopPoint.y, ptlastTopPoint.z),
		CMathUtil::GetMidPoint(ptlastTopPoint, AcGePoint3d(ptlastTopPoint.x - 7.0, ptlastTopPoint.y, ptlastTopPoint.z)), 0, NULL,dimStyleId);

	//最后一个刀尖的斜边长
	double Height = GetHeightByDiameter(m_stepDatas[m_stepNum - 1].m_diameter);
	AcGePoint3d ptEnd(0, 0, 0);
	ptEnd.x = ptlastTopPoint.x + Height / tan(CMathUtil::AngleToRadian(m_stepDatas[m_stepNum-1].m_angle));
	ptEnd.y = ptlastTopPoint.y - Height;
	AcDbObjectId dimID = CDimensionUtil::AddDimAligned(ptlastTopPoint, ptEnd, CMathUtil::GetMidPoint(ptlastTopPoint, ptEnd), NULL,dimStyleId);
	//最后一个刀尖到上边缘的距离
	CDimensionUtil::AddDimAligned(ptEnd, AcGePoint3d(ptEnd.x, ptlastTopPoint.y,ptEnd.z),
		CMathUtil::GetMidPoint(ptEnd, AcGePoint3d(ptEnd.x, ptlastTopPoint.y, ptEnd.z)), NULL,dimStyleId);


	//插入3度偏角标注
	AcGePoint3d ptfirstTopPoint = GetDaoJianPoint(pnt, true, 0);
	double lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
	AcGePoint3d ptVertex(pnt.x,ptfirstTopPoint.y - lf2 ,pnt.z);
	//计算角的两条射线
	double x = (ptfirstTopPoint.x - ptlastTopPoint.x) / 2.0;
	double angle = 3;
	double y = x * tan(CMathUtil::AngleToRadian(angle));
	CDimensionUtil::AddDim2LineAngular(ptVertex, AcGePoint3d(ptVertex.x -x,ptVertex.y,ptVertex.z),
		ptVertex, AcGePoint3d(ptVertex.x - x, ptVertex.y + y, ptVertex.z),
		AcGePoint3d(ptVertex.x - x,ptVertex.y + y / 2.0,ptVertex.z), NULL, dimStyleId);

}

void SPCDJDData::Mending(AcGePoint3d const & pnt)
{
	CLayerSwitch layer(L"1");
	AcGePoint3d lastTopPoint(pnt);
	lastTopPoint.x -= m_stepDatas[m_stepDatas.size() - 1].m_stepLength;
	lastTopPoint.y = lastTopPoint.y + m_stepDatas[m_stepDatas.size() - 1].m_diameter / 2.0 - 0.5;
	AcGePoint3d lastBottomPoint(lastTopPoint);
	lastBottomPoint.y = lastBottomPoint.y - m_stepDatas[m_stepDatas.size() - 1].m_diameter + 0.5;
	CLineUtil::CreateLine(lastTopPoint, lastBottomPoint);
}

//int SPCDJDData::Draw()
//{
//	//第一步：用户选择一个输入点
//	AcGePoint3d pnt;
//	CGetInputUtil::GetPoint(L"请选择一个插入点:", pnt);
//	CDocLock lock;	//如果读取文件失败，可能是因为之前没有加锁
//
//	//插入对应的刀身
//	CString daoShenFilePath = TY_GetDynamicBlockFolder();
//	CString str;
//	str.Format(L"%d", m_stepNum);
//	daoShenFilePath.Append(L"PCD铰刀模板X" + str + L".dwg");
//	/*CString blkName = CCommonUtil::GenStrByTime();
//	AcDbObjectId daoShenID = CBlockUtil::InsertDwgAsBlockRef(daoShenFilePath, blkName, ACDB_MODEL_SPACE, pnt, 0, 1);*/
//
//	//插入对应的刀柄,并设置参数
//	CString daoBingFilePath = TY_GetDaoBingFolder();
//	daoBingFilePath.Append(m_daoBing + L".dwg");
//	//获得上顶点的位置
//	AcGePoint3d firstTopPoint = GetDaoJianPoint(pnt, true, 0);
//	//距离要修改但是缺少一个参数len没有获取
//	double distance = 0;
//	double dis =   GetDisByDBName(m_daoBing);
//	double MaxLLen = m_stepDatas[m_stepDatas.size() - 1].m_stepLength + pnt.x - firstTopPoint.x ;
//	AcGePoint3d insertPiont(pnt.x+  MaxLLen, pnt.y, 0);
//	CBlockUtil::InsertDwgAsBlockRef(daoBingFilePath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);
//
//	//先插入刀柄，在插入刀身
//	CString blkName = CCommonUtil::GenStrByTime();
//	AcDbObjectId daoShenID = CBlockUtil::InsertDwgAsBlockRef(daoShenFilePath, blkName, ACDB_MODEL_SPACE, insertPiont, 0, 1); 
//
//	pnt = insertPiont;
//	//Lf2
//	double Lf2;
//
//	//刀片的半径和宽
//	double radius = 0;
//	double Height = 0;
//
//	switch (m_stepNum)
//	{
//	case 1:
//	{
//		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);
//
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength - distance);
//		
//		CUpdateUtil::UpdateDisplay();
//		vAcDbObjectId explodeIds;
//		CEntityUtil::ExplodeAndAppend(daoShenID, explodeIds);
//
//		radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
//		Height = GetHeightByDiameter(m_stepDatas[0].m_diameter);
//
//		vAcDbObjectId blkRefIds;
//		CBlockUtil::CycleAllBlockReferences(blkRefIds);
//		for (int i = 0; i < blkRefIds.size(); i++)
//		{
//			CString name;
//			CDynamicBlockUtil::GetDynamicBlockName(blkRefIds[i], name);
//			if (name == L"刀尖_1_T")
//			{
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[0].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", Height);
//			}
//		}
//
//		InsertDDiamension(pnt, 0);
//		//插入LF1
//		InsertLf1Dimension(pnt, 0);
//		//InsertOffsetDimension(pnt);
//		//InsertSixtyDimension(pnt);
//		break;
//	}
//	case 2:
//	{
//		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);
//		
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
//	
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[ 1].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength - distance);
//
//		CUpdateUtil::UpdateDisplay();
//		vAcDbObjectId explodeIds;
//		CEntityUtil::ExplodeAndAppend(daoShenID, explodeIds);
//
//		
//		vAcDbObjectId blkRefIds;
//		CBlockUtil::CycleAllBlockReferences(blkRefIds);
//		for (int i = 0; i < blkRefIds.size(); i++)
//		{
//			CString name;
//			CDynamicBlockUtil::GetDynamicBlockName(blkRefIds[i], name);
//			if (name == L"刀尖_1_T")
//			{
//				radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
//				Height = GetHeightByDiameter(m_stepDatas[0].m_diameter);
//
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[0].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", Height);
//			}
//
//			if (name == L"刀尖_2_T")
//			{
//				radius = GetRadiusByDiameter(m_stepDatas[1].m_diameter);
//				Height = GetHeightByDiameter(m_stepDatas[1].m_diameter);
//
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[1].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", Height);
//			}
//		}
//
//
//		InsertDDiamension(pnt, 0);
//		InsertDDiamension(pnt, 1);
//
//		InsertLDiamension(pnt, 1);
//		//插入LF1
//		InsertLf1Dimension(pnt, 1);
//		//InsertOffsetDimension(pnt);
//		//InsertSixtyDimension(pnt);
//		break;
//	}
//	case 3:
//	{
//		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);
//
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
//
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[1].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);
//		
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[2].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[2].m_stepLength - distance);
//
//		CUpdateUtil::UpdateDisplay();
//		vAcDbObjectId explodeIds;
//		CEntityUtil::ExplodeAndAppend(daoShenID, explodeIds);
//
//
//		vAcDbObjectId blkRefIds;
//		CBlockUtil::CycleAllBlockReferences(blkRefIds);
//		for (int i = 0; i < blkRefIds.size(); i++)
//		{
//			CString name;
//			CDynamicBlockUtil::GetDynamicBlockName(blkRefIds[i], name);
//			if (name == L"刀尖_1_T")
//			{
//				radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
//				Height = GetHeightByDiameter(m_stepDatas[0].m_diameter);
//
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[0].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", Height);
//			}
//
//			if (name == L"刀尖_2_T")
//			{
//				radius = GetRadiusByDiameter(m_stepDatas[1].m_diameter);
//				Height = GetHeightByDiameter(m_stepDatas[1].m_diameter);
//
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[1].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", Height);
//			}
//
//			if (name == L"刀尖_3_T")
//			{
//				radius = GetRadiusByDiameter(m_stepDatas[2].m_diameter);
//				Height = GetHeightByDiameter(m_stepDatas[2].m_diameter);
//
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[2].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", Height);
//			}
//		}
//
//		
//		InsertDDiamension(pnt, 0);
//		InsertDDiamension(pnt, 1);
//		InsertDDiamension(pnt, 2 );
//		InsertLDiamension(pnt, 1);
//		InsertLDiamension(pnt, 2);
//		//插入LF1
//		InsertLf1Dimension(pnt, 2);
//		//InsertOffsetDimension(pnt);
//		//InsertSixtyDimension(pnt);
//		break;
//	}
//	case 4:
//	{
//		Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);
//
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[0].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[0].m_stepLength);
//		
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[1].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[1].m_stepLength);
//
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[2].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[2].m_stepLength);
//		
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D4", m_stepDatas[3].m_diameter);
//		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L4", m_stepDatas[3].m_stepLength - distance);
//		
//	
//		
//		CUpdateUtil::UpdateDisplay();
//		vAcDbObjectId explodeIds;
//		CEntityUtil::ExplodeAndAppend(daoShenID,explodeIds);
//
//		vAcDbObjectId blkRefIds;
//		CBlockUtil::CycleAllBlockReferences(blkRefIds);
//		for (int i = 0; i < blkRefIds.size(); i++)
//		{
//			CString name;
//			CDynamicBlockUtil::GetDynamicBlockName(blkRefIds[i], name);
//			if (name == L"刀尖_1_T")
//			{
//				radius = GetRadiusByDiameter(m_stepDatas[0].m_diameter);
//				Height = GetHeightByDiameter(m_stepDatas[0].m_diameter);
//				
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[0].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", Height);
//			}
//
//			if (name == L"刀尖_2_T")
//			{
//				radius = GetRadiusByDiameter(m_stepDatas[1].m_diameter);
//				Height = GetHeightByDiameter(m_stepDatas[1].m_diameter);
//				//CDynamicBlockUtil::SetDynamicValueInDynamicBlock(blkName, name, L"An", m_stepDatas[1].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[1].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", Height);
//			}
//
//			if (name == L"刀尖_3_T")
//			{
//				radius = GetRadiusByDiameter(m_stepDatas[2].m_diameter);
//				Height = GetHeightByDiameter(m_stepDatas[2].m_diameter);
//				//CDynamicBlockUtil::SetDynamicValueInDynamicBlock(blkName, name, L"An", m_stepDatas[2].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[2].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", Height);
//			}
//
//			if (name == L"刀尖_4_T")
//			{
//				radius = GetRadiusByDiameter(m_stepDatas[3].m_diameter);
//				Height = GetHeightByDiameter(m_stepDatas[3].m_diameter);
//				//CDynamicBlockUtil::SetDynamicValueInDynamicBlock(blkName, name, L"An", m_stepDatas[3].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"An", m_stepDatas[3].m_angle);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"R", radius);
//				CDynamicBlockUtil::SetDynamicBlockValue(blkRefIds[i], L"yLen", Height);
//			}
//		}
//		InsertDDiamension(pnt, 0);
//		InsertDDiamension(pnt, 1);
//		InsertDDiamension(pnt, 2);
//		InsertDDiamension(pnt, 3);
//		InsertLDiamension(pnt, 1);
//		InsertLDiamension(pnt, 2);
//		InsertLDiamension(pnt, 3);
//		InsertLf1Dimension(pnt, 3);
//		//InsertOffsetDimension(pnt);
//		//InsertSixtyDimension(pnt);
//		break;
//	}
//	default:
//		break;
//	}
//	//插入角度标注
//	InsertAngleDimension(pnt);
//	//插入其他标注
//	//InsertOtherDimension(pnt);
//
//	vAcDbObjectId dynamicDimsids;
//	CToolingUtil::CycleAllTypedObjectsInAllLayer(CToolingUtil::ACDB_DYNAMIC_DIMENTION, dynamicDimsids);
//	for (int i = 0; i < dynamicDimsids.size(); i++)
//	{
//		CEntityUtil::DeleteObject(dynamicDimsids[i]);
//	}
//	return 0;
//}

int SPCDJDData::Draw()
{
	//第一步：用户选择一个输入点
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"请选择一个插入点:", pnt);
	CDocLock lock;	//如果读取文件失败，可能是因为之前没有加锁

					//插入对应的刀身
	CString daoShenFilePath = TY_GetDynamicBlockFolder();
	CString str;
	str.Format(L"%d", m_stepNum);
	daoShenFilePath.Append(L"PCD铰刀模板X" + str + L".dwg");
	/*CString blkName = CCommonUtil::GenStrByTime();
	AcDbObjectId daoShenID = CBlockUtil::InsertDwgAsBlockRef(daoShenFilePath, blkName, ACDB_MODEL_SPACE, pnt, 0, 1);*/

	//插入对应的刀柄,并设置参数
	CString daoBingFilePath = TY_GetDaoBingFolder();
	daoBingFilePath.Append(m_daoBing + L".dwg");
	//获得上顶点的位置
	AcGePoint3d firstTopPoint = GetDaoJianPoint(pnt, true, 0);
	//距离要修改但是缺少一个参数len没有获取
	double distance = 0;
	double dis = GetDisByDBName(m_daoBing);
	double MaxLLen = m_stepDatas[m_stepDatas.size() - 1].m_stepLength + pnt.x - firstTopPoint.x;
	AcGePoint3d insertPiont(pnt.x + MaxLLen, pnt.y, 0);
	CBlockUtil::InsertDwgAsBlockRef(daoBingFilePath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);

	//先插入刀柄，在插入刀身
	CString blkName = CCommonUtil::GenStrByTime();
	AcDbObjectId daoShenID = CBlockUtil::InsertDwgAsBlockRef(daoShenFilePath, blkName, ACDB_MODEL_SPACE, insertPiont, 0, 1);

	pnt = insertPiont;
	//Lf2
	double Lf2;

	//刀片的半径和宽
	double radius = 0;
	double Height = 0;

	
	Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
	CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);

	for (size_t i = 0; i< m_stepDatas.size();++i)
	{
		CString temp;
		temp.Format(L"D%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, m_stepDatas[i].m_diameter);
		temp.Format(L"L%d", i + 1);
		if (i == m_stepDatas.size() - 1)
			CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, m_stepDatas[i].m_stepLength - dis);
		else
			CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, m_stepDatas[i].m_stepLength);
			
	}

	for (size_t i = 0; i < m_stepDatas.size() ; i++)
	{
		CString temp;
		radius = GetRadiusByDiameter(m_stepDatas[i].m_diameter);
		Height = GetHeightByDiameter(m_stepDatas[i].m_diameter);
		temp.Format(L"An%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, m_stepDatas[i].m_angle);
		temp.Format(L"R%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID,temp, radius);
		temp.Format(L"yLen%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, Height);

		//直径标准
		InsertDDiamension(pnt, i);
		//长度标注
		InsertLDiamension(pnt, i);
	}
	InsertLf1Dimension(pnt, m_stepDatas.size() - 1);
	//插入角度标注
	InsertAngleDimension(pnt);
	//插入其他标注
	//InsertOtherDimension(pnt);
	Mending(pnt);
	/*vAcDbObjectId dynamicDimsids;
	CToolingUtil::CycleAllTypedObjectsInAllLayer(CToolingUtil::ACDB_DYNAMIC_DIMENTION, dynamicDimsids);
	for (int i = 0; i < dynamicDimsids.size(); i++)
	{
		CEntityUtil::DeleteObject(dynamicDimsids[i]);
	}*/
	return 0;
}

