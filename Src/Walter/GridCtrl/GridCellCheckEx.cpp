#include "stdafx.h"
#include "GridCellCheckEx.h"

IMPLEMENT_DYNCREATE(CGridCellCheckEx, CGridCellCheck)

void CGridCellCheckEx::OnClick(CPoint PointCellRelative)
{
	PointCellRelative += m_Rect.TopLeft();
	CCellID cell = GetGrid()->GetCellFromPt(PointCellRelative);

	if (GetCheckPlacement().PtInRect(PointCellRelative))
	{
		m_bChecked = !m_bChecked;
		GetGrid()->InvalidateRect(m_Rect);
		m_pGrid->GetParent()->PostMessage(WM_GRIDCHECKBOX_CLICKED, cell.row - m_pGrid->GetFixedRowCount(), cell.col);
	}
}

CRect CGridCellCheckEx::GetCheckPlacement()
{
	int nWidth = GetSystemMetrics(SM_CXHSCROLL);
	CRect place = m_Rect;

	// for centering
	int nDiff = (place.Width() - nWidth)/2;
	if (nDiff > 0)
	{
		place.left += nDiff;
		place.right = place.left + nWidth;
	}
	nDiff = (place.Height() - nWidth)/2;
	if (nDiff > 0)
	{
		place.top += nDiff;
		place.bottom = place.top + nWidth;
	}
    
	return place;
}
