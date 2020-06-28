#pragma once

#include "GridCtrl_src/GridCtrl.h"
#include "GridCtrl_src/NewCellTypes/GridCellCheck.h"

#define WM_GRIDCHECKBOX_CLICKED (WM_USER + 1002)

class CGridCellCheckEx : public CGridCellCheck
{
	DECLARE_DYNCREATE(CGridCellCheckEx)

public:
	afx_msg void OnClick(CPoint PointCellRelative);

protected:
	CRect GetCheckPlacement();
};