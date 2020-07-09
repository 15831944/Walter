#pragma once
#include <vector>

//pcd½Âµ¶½×ÌÝÊý¾Ý
typedef struct PCDJDStepData
{
	int index;
	double m_diameter;//Ö±¾¶
	double m_stepLength;//½×ÌÝ³¤¶È
	double m_angle;//½×ÌÝÖ÷Æ«½Ç
}PCDJDStepData;

typedef struct  SPCDJDData
{
	CString m_daoBing;//µ¶±ú
	int m_stepNum;//½×ÌÝÊýÁ¿
	std::vector<PCDJDStepData>  m_stepDatas;
	int Draw();
}SPCDJDData;

