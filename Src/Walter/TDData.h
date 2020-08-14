#pragma once
#include "Com.h"

typedef struct TDSegData {
	double m_angle; //�Ƕ�
	double m_len; //����
	double m_diameter; // ֱ��
}TDSegData;

class CTDData
{
public:
	CTDData();
	virtual ~CTDData();
	void AddSegData(TDSegData const&& segdata) { m_tdSegdata.push_back(segdata); }
	void ClearData() { m_tdSegdata.clear(); m_tdSegdata.shrink_to_fit(); }
	void SetHandleName(CString const&& HandleName) { m_HandleName = HandleName; }
	void SetGrooveLen(double len) { m_grooveLength = len; }
	void SetTotalLen(double len) { m_totallength = len; }
	void SetLabberCount(int count) { m_labberCount = count; }
	void Draw();
	void AddDiaDimension(); //ֱ����ע
	void AddLenDimension(); // ���볤�ȱ�ע
private:
	double m_grooveLength; //��м�۳�
	double m_totallength; //���쳤��
	CString m_HandleName; //��������
	std::vector<TDSegData> m_tdSegdata; //�۵���������
	int m_labberCount; 
};

