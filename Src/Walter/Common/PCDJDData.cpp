#include "stdafx.h"
#include "PCDJDData.h"
#include "Entity/DynamicBlockUtil.h"
#include "Com.h"

static HINSTANCE s_gTyToolInst = 0;

int SPCDJDData::Draw()
{
	//第一步：用户选择一个输入点
	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"请选择一个插入点:", pnt);
	CDocLock lock;	//如果读取文件失败，可能是因为之前没有加锁

	//插入对应的刀身
	CString daoShenFilePath = TY_GetDwgFolder();
	CString str;
	str.Format(L"%d", m_stepNum);
	daoShenFilePath.Append(L"DaoShen\\PCD铰刀模板X" + str + L".dwg");
	AcDbObjectId daoShenID = CBlockUtil::InsertDwgAsBlockRef(daoShenFilePath, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);

	//插入对应的刀柄,并设置参数
	CString daoBingFilePath = TY_GetDwgFolder();
	daoBingFilePath.Append(L"DaoBing\\" + m_daoBing + L".dwg");
	double MaxLLen;
	CDynamicBlockUtil::GetDynamicBlockData(daoShenID, L"L", MaxLLen);
	AcGePoint3d insertPiont(pnt.x - MaxLLen, pnt.y, 0);
	CBlockUtil::InsertDwgAsBlockRef(daoBingFilePath, NULL, ACDB_MODEL_SPACE, insertPiont, 0, 1);
	switch (m_stepNum)
	{
	case 1:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	case 2:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	case 3:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 3].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 3].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[m_stepNum - 2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[m_stepNum - 2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	case 4:
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D1", m_stepDatas[m_stepNum - 4].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L1", m_stepDatas[m_stepNum - 4].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D2", m_stepDatas[m_stepNum - 3].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L2", m_stepDatas[m_stepNum - 3].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D3", m_stepDatas[m_stepNum - 2].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L3", m_stepDatas[m_stepNum - 2].m_stepLength);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"D4", m_stepDatas[m_stepNum - 1].m_diameter);
		CDynamicBlockUtil::SetDynamicBlockValue(daoShenID, L"L", m_stepDatas[m_stepNum - 1].m_stepLength);
		break;
	default:
		break;
	}

	CViewUtil::ZoomExtent();
	return 0;
}