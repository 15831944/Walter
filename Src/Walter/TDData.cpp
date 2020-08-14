#include "stdafx.h"
#include "TDData.h"
#include "Entity/DimensionUtil.h"
#include "Entity/DynamicBlockUtil.h"

CTDData::CTDData()
{
}


CTDData::~CTDData()
{
}

void CTDData::Draw()
{
	CDocLock lock;

	AcGePoint3d pnt;
	CGetInputUtil::GetPoint(L"���������㣺",pnt);
	//���뵶��
	CString Handle = TY_GetDaoBingFolder() + m_HandleName + ".dwg";
	CBlockUtil::InsertDwgAsBlockRef(Handle, NULL, ACDB_MODEL_SPACE, pnt, 0, 1);
	//���뵶��
	CString temp;
	temp.Format(L"%d", m_labberCount);
	CString KnifeName = TY_GetDynamicBlockFolder() + "�ɻ���Ƭ�۵�ģ��X" + temp + ".dwg";
	AcGePoint3d ptInsert(pnt);
	ptInsert.x += m_totallength;
	AcDbObjectId KnifeId = CBlockUtil::InsertDwgAsBlockRef(KnifeName, NULL, ACDB_MODEL_SPACE, ptInsert, 0, 1);

	
	double dis = GetDisByDBName(m_HandleName);
	CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, L"L", m_totallength);
	//���ö�̬�����
	for (size_t i = 0; i < m_tdSegdata.size(); ++i)
	{
		CString temp;
		//�Ƕ�
		temp.Format(L"A%d", i + 1);
		if (i == m_tdSegdata.size() - 1) {
			CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, L"A", m_tdSegdata[i].m_angle);
		}
		else
		{
			CDynamicBlockUtil::SetDynamicBlockValue(KnifeId,temp, m_tdSegdata[i].m_angle);
		}
		//ֱ��
		temp.Format(L"D%d", i + 1);
		CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, temp, m_tdSegdata[i].m_diameter);
		//����
		temp.Format(L"L%d", i + 1);
		if (i == m_tdSegdata.size() - 1) {
			CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, temp, m_tdSegdata[i].m_len - dis);
		}
		else {
			CDynamicBlockUtil::SetDynamicBlockValue(KnifeId, temp, m_tdSegdata[i].m_len);
		}

	}

}

void CTDData::AddDiaDimension()
{

}

void CTDData::AddLenDimension()
{

}
