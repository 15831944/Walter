#pragma once
#include "Com.h"

typedef struct TDSegData {
	double m_angle; //角度
	double m_len; //长度
	double m_diameter; // 直径
}TDSegData;

class CTDData
{
public:
	CTDData() {};
	virtual ~CTDData() {};
	void AddSegData(TDSegData const&& segdata) { m_tdSegdata.push_back(segdata); }
	void ClearData() { m_tdSegdata.clear(); m_tdSegdata.shrink_to_fit(); }
	void SetHandleName(CString const&& HandleName) { m_HandleName = HandleName; }
	void SetGrooveLen(double len) { m_grooveLength = len; }
	void SetTotalLen(double len) { m_totallength = len; }
	void SetLabberCount(int count) { m_labberCount = count; }
	void Draw();
	AcGePoint3d getVertexPoint(const AcGePoint3d & pnt, int index, bool IsTop);
	void AddDiaDimension(const AcGePoint3d & pnt); //直径标注
	void AddLenDimension(const AcGePoint3d & pnt); // 插入长度标注
	void AddAngleDimension(const AcGePoint3d & pnt); //角度标注
	void Mending(const AcGePoint3d & pnt); // 补线
private:
	double m_grooveLength; //排屑槽长
	double m_totallength; //悬伸长度
	CString m_HandleName; //刀柄名称
	std::vector<TDSegData> m_tdSegdata; //镗刀阶梯数据
	int m_labberCount; 
};

