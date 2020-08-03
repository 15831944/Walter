#pragma once
#include <vector>

//pcd铰刀阶梯数据
typedef struct PCDJDStepData
{
	int index;
	double m_diameter;//直径
	double m_stepLength;//阶梯长度
	double m_angle;//阶梯主偏角
}PCDJDStepData;

typedef struct  SPCDJDData
{
	
	CString m_daoBing;//刀柄
	int m_stepNum;//阶梯数量
	std::vector<PCDJDStepData>  m_stepDatas;
	//int Draw();
	int Draw();
	//获取刀尖顶点
	AcGePoint3d GetDaoJianPoint(const AcGePoint3d& pnt, bool isTop, int stepIndex);
	//插入直径标注
	void InsertDDiamension(const AcGePoint3d& pnt, int stepIndex);
	//插入L标注
	void InsertLDiamension(const AcGePoint3d& pnt, int stepIndex);
	void InsertLf1Dimension(const AcGePoint3d& pnt,int stepIndex);
	//插入offset标注
	//void InsertOffsetDimension(const AcGePoint3d& pnt);
	//插入主偏角度标注
	void InsertAngleDimension(const AcGePoint3d& pnt);
	//插入60度标注
	//void InsertSixtyDimension(const AcGePoint3d& pnt);
	//插入其他标注
	void InsertOtherDimension(const AcGePoint3d& pnt);
	//补线
	void Mending(AcGePoint3d const& pnt);
}SPCDJDData;

