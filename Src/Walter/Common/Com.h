#pragma once
#include <vector>

#define DEV_TEST_ENV//开发测试环境
enum TUKuangType{A1 = 0, A2, A3};

CString TY_GetAppPath();

//刀具库存储目录
CString TY_GetDwgFolder();
//得到图框存储路径
CString TY_GetFrameFolder();
