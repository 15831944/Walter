#pragma once
#include <vector>

#define DEV_TEST_ENV//�������Ի���
enum TUKuangType{A1 = 0, A2, A3};

CString TY_GetAppPath();

//���ݵ���ֱ����ȡ lf2��ֵ
double GetLf2ByDiameter(double diameter);
//���߿�洢Ŀ¼
CString TY_GetDwgFolder();
CString TY_GetDynamicBlockFolder();
CString TY_GetDaoBingFolder();
//�õ�ͼ��洢·��
CString TY_GetFrameFolder();

CString TY_GetDllFilePathName();



bool TY_LoadTyTool();
bool TY_FreeTyTool();
int TY_SetProgress(int all, int cur);
void TY_Progress_Close();
void TY_Progress_Init();



