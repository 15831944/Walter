#pragma once
#include <vector>

#define DEV_TEST_ENV//开发测试环境
enum TUKuangType{A1 = 0, A2, A3};

CString TY_GetAppPath();

//刀具库存储目录
CString TY_GetDwgFolder();
//得到图框存储路径
CString TY_GetFrameFolder();

CString TY_GetDllFilePathName();



bool TY_LoadTyTool();
bool TY_FreeTyTool();
int TY_SetProgress(int all, int cur);
void TY_Progress_Close();
void TY_Progress_Init();


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
}SPCDJDData;

