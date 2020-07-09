#pragma once
#include <vector>

#define DEV_TEST_ENV//开发测试环境
enum TUKuangType{A1 = 0, A2, A3};

CString TY_GetAppPath();

//根据刀具直径获取 lf2的值
double GetLf2ByDiameter(double diameter);
//刀具库存储目录
CString TY_GetDwgFolder();
CString TY_GetDynamicBlockFolder();
CString TY_GetDaoBingFolder();
//得到图框存储路径
CString TY_GetFrameFolder();

CString TY_GetDllFilePathName();



bool TY_LoadTyTool();
bool TY_FreeTyTool();
int TY_SetProgress(int all, int cur);
void TY_Progress_Close();
void TY_Progress_Init();



