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
	int Draw();
	//获取刀尖顶点
	AcGePoint3d GetDaoJianPoint(const AcGePoint3d& pnt, bool isTop, int stepIndex);
	//插入标注
	void InsertDDiamension(const AcGePoint3d& pnt, int step);
}SPCDJDData;

