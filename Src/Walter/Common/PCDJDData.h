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
}SPCDJDData;

