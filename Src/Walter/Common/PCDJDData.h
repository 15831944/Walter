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
	//int Draw();
	int Draw();
	//��ȡ���ⶥ��
	AcGePoint3d GetDaoJianPoint(const AcGePoint3d& pnt, bool isTop, int stepIndex);
	//����ֱ����ע
	void InsertDDiamension(const AcGePoint3d& pnt, int stepIndex);
	//����L��ע
	void InsertLDiamension(const AcGePoint3d& pnt, int stepIndex);
	void InsertLf1Dimension(const AcGePoint3d& pnt,int stepIndex);
	//����offset��ע
	//void InsertOffsetDimension(const AcGePoint3d& pnt);
	//������ƫ�Ƕȱ�ע
	void InsertAngleDimension(const AcGePoint3d& pnt);
	//����60�ȱ�ע
	//void InsertSixtyDimension(const AcGePoint3d& pnt);
	//����������ע
	void InsertOtherDimension(const AcGePoint3d& pnt);
	//����
	void Mending(AcGePoint3d const& pnt);
}SPCDJDData;

