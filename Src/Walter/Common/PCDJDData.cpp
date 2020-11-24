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

//插入直径标注
void SPCDJDData::InsertDDiamension(const AcGePoint3d& pnt,int stepIndex)
{
	CLayerSwitch layer(DIMLAYERNAME);
	AcGePoint3d ptTop = GetDaoJianPoint(pnt, true, stepIndex);
	AcGePoint3d ptBottom = GetDaoJianPoint(pnt, false, stepIndex);
	AcGePoint3d ptCenter(pnt);
	
	CString temp;
	temp.Format(L"%%%%C%s", removeLastZero(m_stepDatas[stepIndex].m_diameter));
	//
	ptCenter.x = pnt.x + 10 + stepIndex * DIMDISTANCE;
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
	center.y = pnt.y + DIMDISTANCE*stepIndex;
	
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
	center.y = pnt.y + stepIndex * DIMDISTANCE + 10;
	AcDbObjectId dimStyleId = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
	CDimensionUtil::AddDimRotated(LfDstart, LfDend, center, 0,NULL, dimStyleId);
	//插入总长标注
	AcGePoint3d lastpoint(LfDstart);
	
	lastpoint.x = lastpoint.x - m_stepDatas[stepIndex].m_stepLength;
	center = CMathUtil::GetMidPoint(LfDstart, lastpoint);
	center.y = LfDend.y + stepIndex * DIMDISTANCE + 20;
	CDimensionUtil::AddDimRotated(LfDstart, lastpoint, center, 0, NULL, dimStyleId);
}

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
	AcGePoint3d firstTopPoint = GetDaoJianPoint(pnt, true, 0);
	AcGePoint3d lastTopPoint(pnt);
	double handleLen = GetDisByDBName(m_daoBing);
	double dia = m_stepDatas[m_stepDatas.size() - 1].m_diameter - 1;

	lastTopPoint.x = firstTopPoint.x - m_stepDatas[m_stepDatas.size() - 1].m_stepLength + handleLen;
	lastTopPoint.y = lastTopPoint.y + dia / 2.0 ;

	AcGePoint3d lastBottomPoint(lastTopPoint);
	lastBottomPoint.y = lastBottomPoint.y - dia ;

	CLineUtil::CreateLine(lastTopPoint, lastBottomPoint);
}

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
	if (Lf2 <= 2.3) {
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"FixAn", 1);
	}
	

	for (size_t i = 0; i < m_stepDatas.size(); ++i)
	{
		CString temp;
		//先设置焊刀模块
		radius = GetRadiusByDiameter(m_stepDatas[i].m_diameter);
		Height = GetHeightByDiameter(m_stepDatas[i].m_diameter);
		temp.Format(L"An%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, m_stepDatas[i].m_angle);
		temp.Format(L"R%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, radius);
		temp.Format(L"yLen%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, Height);

		

		temp.Format(L"D%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, m_stepDatas[i].m_diameter);

		temp.Format(L"L%d", i + 1);
		if (i == m_stepDatas.size() - 1)
			CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, m_stepDatas[i].m_stepLength - dis);
		else
			CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, temp, m_stepDatas[i].m_stepLength);

		//直径标准
		InsertDDiamension(pnt, i);
		//长度标注
		InsertLDiamension(pnt, i);
	}
	
	
	CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);
	
	InsertLf1Dimension(pnt,(int) m_stepDatas.size() - 1);
	//插入角度标注
	InsertAngleDimension(pnt);
	//添加补线
	Mending(pnt);

	return 0;
}

