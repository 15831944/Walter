#pragma once
#include <vector>
#include "Com.h"

class CCutterTool
{
public:
	CCutterTool(void);
	~CCutterTool(void);
	AcDbObjectId SetTable();
public:
	 CString toolNumber;//���ߺ�
	 vCString proContent;//��������
	 vCString toDescription;//��������
	 vCString toType;//�����ͺ�

	TUKuangType m_tkType;
	AcDbObjectId m_tkId;
	vAcDbObjectId m_toolsIds;
};

int TY_ReadCutterToolsFromExcel(CString xlsFileName, std::vector<CCutterTool> &cutterTools);

