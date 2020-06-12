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
	 CString toolNumber;//刀具号
	 vCString proContent;//刀具内容
	 vCString toDescription;//刀具描述
	 vCString toType;//刀具型号

	TUKuangType m_tkType;
	AcDbObjectId m_tkId;
	vAcDbObjectId m_toolsIds;
};

int TY_ReadCutterToolsFromExcel(CString xlsFileName, std::vector<CCutterTool> &cutterTools);

