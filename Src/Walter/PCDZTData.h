#pragma once
#include "Com.h"
#include <tuple>
typedef struct ZtSegData {
	double angle = 0.0f;
	double len = 0.0f;
	double diameter = 0.0f;
}ZtSegData;

class CPCDZTData
{
public:
	CPCDZTData();
	virtual ~CPCDZTData();
	void SetHandleName(CString const& handleName) { m_handleName = handleName; }
	void SetTotalLen(double len) { m_totalLen = len; }
	void SetGrooveLen(double len) { m_grooveLenth = len; }
	void AddSegData(ZtSegData&& segdata) {
		m_StepData.push_back(segdata);
		if (m_StepData.size() == 1) { m_grooveLenth = m_StepData[0].len + 30; }
		else { m_grooveLenth = m_StepData[m_StepData.size() - 2].len  + 20; }
	}
	void Draw();
private:
	void CreateDims(AcGePoint3d const& pnt) { AddAngleDimension(pnt); AddLenDimension(pnt); AddDiaDimension(pnt); }
	AcGePoint3d getVertexPoint(AcGePoint3d const& pnt, int index, bool isTop); //获取顶点位置
	void AddDiaDimension(AcGePoint3d const& pnt); //直径标注
	void AddLenDimension(AcGePoint3d const& pnt);	// 长度标注
	void AddAngleDimension(AcGePoint3d const& pnt); // 角度标注
	void Mending(AcGePoint3d const& pnt); //缺口补线

	//根据直径获取半径和长度以及高度
	double getR(double diameter);
	double getXlen(double diameter);
	double getYlen(double diameter);

	
private:
	vector<ZtSegData> m_StepData;
	double m_grooveLenth;
	double m_totalLen;
	CString m_handleName;
};

