#pragma once
#include "Com.h"

class PCDXD {
public :
	PCDXD() {}
	virtual ~PCDXD() { m_totalLength = 150; m_bladeLength = 30; m_diameter = 40; offset = 0.75;  }
	int Draw();
	void SetTotalLength(double const& len) { m_totalLength = len; }
	void SetHandleName(CString const& name) { m_handleName = name; }
	void SetBladeLength(double const& len) {
		m_bladeLength = len; m_grooveLength = m_bladeLength + 10;
	}
	void SetDiameter(double const& diameter) { m_diameter = diameter; }

private:
	double m_totalLength;
	double m_bladeLength;
	double m_diameter;
	CString m_handleName;
	double offset;
	double m_grooveLength;
};