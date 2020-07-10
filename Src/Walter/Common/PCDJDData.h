#pragma once
#include <vector>

//pcd�µ���������
typedef struct PCDJDStepData
{
	int index;
	double m_diameter;//ֱ��
	double m_stepLength;//���ݳ���
	double m_angle;//������ƫ��
}PCDJDStepData;

typedef struct  SPCDJDData
{
	CString m_daoBing;//����
	int m_stepNum;//��������
	std::vector<PCDJDStepData>  m_stepDatas;
	int Draw();
	//��ȡ���ⶥ��
	AcGePoint3d GetDaoJianPoint(const AcGePoint3d& pnt, bool isTop, int stepIndex);
	//�����ע
	void InsertDDiamension(const AcGePoint3d& pnt, int step);
}SPCDJDData;

