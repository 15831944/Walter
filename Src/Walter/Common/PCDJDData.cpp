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

//����ֱ����ע
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
//����L��ע
void SPCDJDData::InsertLDiamension(const AcGePoint3d & pnt, int stepIndex)
{
	CLayerSwitch layer(DIMLAYERNAME);
	if (stepIndex == 0)
	{
		return;
	}
	//L��ע�����յ�
	AcGePoint3d LDstart = GetDaoJianPoint(pnt, true, 0);
	AcGePoint3d LDend = GetDaoJianPoint(pnt, true, stepIndex);

	AcGePoint3d center = CMathUtil::GetMidPoint(LDstart, LDend);
	center.y = pnt.y + DIMDISTANCE*stepIndex;
	
	AcDbObjectId dimStyleId = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
	CDimensionUtil::AddDimRotated(LDstart, LDend, center,0,NULL, dimStyleId);
	
}
//����Lf1��ע
void SPCDJDData::InsertLf1Dimension(const AcGePoint3d & pnt, int stepIndex)
{
	CLayerSwitch layer(DIMLAYERNAME);
	double len = GetDisByDBName(m_daoBing);
	//����Lf1��ע
	AcGePoint3d LfDstart = GetDaoJianPoint(pnt, true, 0);
	AcGePoint3d LfDend = GetDaoJianPoint(pnt,true,stepIndex);
	//Lf1��ע����Ϊ���L+30
	LfDend.x = LfDend.x - 30;

	AcGePoint3d center = CMathUtil::GetMidPoint(LfDstart, LfDend);
	center.y = pnt.y + stepIndex * DIMDISTANCE + 10;
	AcDbObjectId dimStyleId = CDimensionUtil::GetDimstylerID(DIMSTYLENAME);
	CDimensionUtil::AddDimRotated(LfDstart, LfDend, center, 0,NULL, dimStyleId);
	//�����ܳ���ע
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
		//�ǵĶ���
		AcGePoint3d ptTop = GetDaoJianPoint(pnt, true, stepIndex);
		double y = GetHeightByDiameter(m_stepDatas[stepIndex].m_diameter);
		double x = m_stepDatas[stepIndex].m_angle != 90 ? y / tan(CMathUtil::AngleToRadian(m_stepDatas[stepIndex].m_angle)) : 0;
		//���ɽǵ�����������
		AcGePoint3d ptEnd1(ptTop.x + 5, ptTop.y, 0);
		AcGePoint3d ptEnd2(ptTop.x + x, ptTop.y - y, 0);
		
		//��ע��
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
	//�����ע Ĭ��Ϊ7.0
	double xLen = 7.0;
	//���һ�����ⶥ��
	AcGePoint3d ptlastTopPoint = GetDaoJianPoint(pnt, true, m_stepNum - 1);
	CDimensionUtil::AddDimRotated(ptlastTopPoint, AcGePoint3d(ptlastTopPoint.x - 7.0, ptlastTopPoint.y, ptlastTopPoint.z),
		CMathUtil::GetMidPoint(ptlastTopPoint, AcGePoint3d(ptlastTopPoint.x - 7.0, ptlastTopPoint.y, ptlastTopPoint.z)), 0, NULL,dimStyleId);

	//���һ�������б�߳�
	double Height = GetHeightByDiameter(m_stepDatas[m_stepNum - 1].m_diameter);
	AcGePoint3d ptEnd(0, 0, 0);
	ptEnd.x = ptlastTopPoint.x + Height / tan(CMathUtil::AngleToRadian(m_stepDatas[m_stepNum-1].m_angle));
	ptEnd.y = ptlastTopPoint.y - Height;
	AcDbObjectId dimID = CDimensionUtil::AddDimAligned(ptlastTopPoint, ptEnd, CMathUtil::GetMidPoint(ptlastTopPoint, ptEnd), NULL,dimStyleId);
	//���һ�����⵽�ϱ�Ե�ľ���
	CDimensionUtil::AddDimAligned(ptEnd, AcGePoint3d(ptEnd.x, ptlastTopPoint.y,ptEnd.z),
		CMathUtil::GetMidPoint(ptEnd, AcGePoint3d(ptEnd.x, ptlastTopPoint.y, ptEnd.z)), NULL,dimStyleId);


	//����3��ƫ�Ǳ�ע
	AcGePoint3d ptfirstTopPoint = GetDaoJianPoint(pnt, true, 0);
	double lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
	AcGePoint3d ptVertex(pnt.x,ptfirstTopPoint.y - lf2 ,pnt.z);
	//����ǵ���������
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
	//��һ�����û�ѡ��һ�������
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"��ѡ��һ�������:", pnt);
	CDocLock lock;	//�����ȡ�ļ�ʧ�ܣ���������Ϊ֮ǰû�м���

					//�����Ӧ�ĵ���
	CString daoShenFilePath = TY_GetDynamicBlockFolder();
	CString str;
	str.Format(L"%d", m_stepNum);
	daoShenFilePath.Append(L"PCD�µ�ģ��X" + str + L".dwg");
	/*CString blkName = CCommonUtil::GenStrByTime();
	AcDbObjectId daoShenID = CBlockUtil::InsertDwgAsBlockRef(daoShenFilePath, blkName, ACDB_MODEL_SPACE, pnt, 0, 1);*/

	//�����Ӧ�ĵ���,�����ò���
	CString daoBingFilePath = TY_GetDaoBingFolder();
	daoBingFilePath.Append(m_daoBing + L".dwg");
	//����϶����λ��
	AcGePoint3d firstTopPoint = GetDaoJianPoint(pnt, true, 0);
	//����Ҫ�޸ĵ���ȱ��һ������lenû�л�ȡ
	double distance = 0;
	double dis = GetDisByDBName(m_daoBing);
	double MaxLLen = m_stepDatas[m_stepDatas.size() - 1].m_stepLength + pnt.x - firstTopPoint.x;
	AcGePoint3d insertPiont(pnt.x + MaxLLen, pnt.y, 0);
	CBlockUtil::InsertDwgAsBlockRef(daoBingFilePath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);

	//�Ȳ��뵶�����ڲ��뵶��
	CString blkName = CCommonUtil::GenStrByTime();
	AcDbObjectId daoShenID = CBlockUtil::InsertDwgAsBlockRef(daoShenFilePath, blkName, ACDB_MODEL_SPACE, insertPiont, 0, 1);

	pnt = insertPiont;
	//Lf2
	double Lf2;

	//��Ƭ�İ뾶�Ϳ�
	double radius = 0;
	double Height = 0;

	Lf2 = GetLf2ByDiameter(m_stepDatas[0].m_diameter);
	if (Lf2 <= 2.3) {
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"FixAn", 1);
	}
	

	for (size_t i = 0; i < m_stepDatas.size(); ++i)
	{
		CString temp;
		//�����ú���ģ��
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

		//ֱ����׼
		InsertDDiamension(pnt, i);
		//���ȱ�ע
		InsertLDiamension(pnt, i);
	}
	
	
	CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"Lf2", Lf2);
	
	InsertLf1Dimension(pnt,(int) m_stepDatas.size() - 1);
	//����Ƕȱ�ע
	InsertAngleDimension(pnt);
	//��Ӳ���
	Mending(pnt);

	return 0;
}

