#include "stdafx.h"
#include "GridCtrlEx.h"

IMPLEMENT_DYNCREATE(CGridCtrlEx, CGridCtrl)

vector<CString> Split(CString cstr, TCHAR key)
{
	vector<CString> ret;
	int pos;
	while ((pos = cstr.Find(key)) != -1)
	{
		ret.push_back(cstr.Left(pos));
		cstr = cstr.Mid(pos + 1);
	}
	if (!cstr.IsEmpty())
		ret.push_back(cstr);
	return ret;
}

CGridCtrlEx::CGridCtrlEx()
{
	LoadDefaltSettings();
}

void CGridCtrlEx::LoadDefaltSettings()
{
	SetListMode(TRUE);
	EnableDragAndDrop(FALSE);
	SetEditable(FALSE);
	m_dFormat = DT_CENTER;
}

void CGridCtrlEx::SetHeaderText(CString sText)
{
	vector<CString> headerTexts = Split(sText, _T(';'));

	if (GetColumnCount() != headerTexts.size())
		SetColumnCount((int)headerTexts.size());
	if (GetRowCount() < 1)
		SetRowCount(1);
	if (GetFixedRowCount() < 1)
		SetFixedRowCount(1);

	for (UINT i = 0; i < headerTexts.size(); i++)
	{
		SetItemText(0, i, headerTexts[i]);
		SetItemFormat(0, i, m_dFormat);
	}
	Refresh();
}

void CGridCtrlEx::SetHeaderWidth(CString sWidth)
{
	vector<CString> headerWidth = Split(sWidth, _T(';'));

	CRect rect;
	GetClientRect(rect);

	int widthNotUsed = rect.Width();
	int pos, plusPos = -1, maxWidth, minWidth;

	for (UINT i = 0; i < headerWidth.size(); i++)
	{
		if (headerWidth[i].Find(_T('+')) != -1)
			plusPos = i;
		if ((pos = headerWidth[i].Find(_T('-'))) != -1)
		{
			minWidth = _ttoi(headerWidth[i].Left(pos)) * rect.Width() / 100;
			maxWidth = _ttoi(headerWidth[i].Mid(pos + 1)) * rect.Width() / 100;
			AutoSizeColumn(i);
			if ((GetColumnWidth(i) > maxWidth) && (maxWidth > 0))
				SetColumnWidth(i, maxWidth);
			if ((GetColumnWidth(i) < minWidth) && (minWidth > 0))
				SetColumnWidth(i, minWidth);
			if (widthNotUsed >= GetColumnWidth(i))
				widthNotUsed -= GetColumnWidth(i);
		}
		else
		{
			SetColumnWidth(i, _ttoi(headerWidth[i]) * rect.Width() / 100);
			if (widthNotUsed >= GetColumnWidth(i))
				widthNotUsed -= GetColumnWidth(i);
		}
	}
	if (plusPos >= 0)
		SetColumnWidth(plusPos, GetColumnWidth(plusPos) + widthNotUsed);

	Refresh();
}

void CGridCtrlEx::SetDefaultFormat(UINT format)
{
	m_dFormat = format;
}

void CGridCtrlEx::SetColumnEditabe(CString sCol)
{
	vector<CString> sCols = Split(sCol, _T(';'));

	vector<int> nCols(sCols.size());
	for (UINT i = 0; i < sCols.size(); i++)
		nCols[i] = _ttoi(sCols[i]);

	for (int i = GetFixedRowCount(); i < GetRowCount(); i++)
		for (int j = 0; j < GetColumnCount(); j++)
			SetItemState(i, j, GetItemState(i, j) | GVIS_READONLY);

	for (int i = GetFixedRowCount(); i < GetRowCount(); i++)
		for (UINT j = 0; j < nCols.size(); j++)
			SetItemState(i, nCols[j], GetItemState(i, nCols[j]) & ~GVIS_READONLY);
	Refresh();
}

void CGridCtrlEx::SetContentItemEditable(int nRow, int nCol, bool bEditable)
{
	nRow += GetFixedRowCount();
	if (bEditable)
		SetItemState(nRow, nCol, GetItemState(nRow, nCol) & ~GVIS_READONLY);
	else
		SetItemState(nRow, nCol, GetItemState(nRow, nCol) | GVIS_READONLY);
	Refresh();
}

void CGridCtrlEx::SetContentItemText(int nRow, int nCol, CString text)
{
	SetContentItemFormat(nRow, nCol, m_dFormat);
	SetItemText(nRow + GetFixedRowCount(), nCol, text);
	Refresh();
}

void CGridCtrlEx::SetContentItemState(int nRow, int nCol, UINT state)
{
	nRow += GetFixedRowCount();
	SetItemState(nRow, nCol, state);
	Refresh();
}

void CGridCtrlEx::SetContentItemFormat(int nRow, int nCol, UINT format)
{
	nRow += GetFixedRowCount();
	SetItemFormat(nRow, nCol, format);
	Refresh();
}

void CGridCtrlEx::SetContentItemData(int nRow, int nCol, LPARAM nValue)
{
	nRow += GetFixedRowCount();
	SetItemData(nRow, nCol, nValue);
}

void CGridCtrlEx::SetSelectedRows(const vector<int> selectedRows)
{
	UINT state;
	for (int i = GetFixedRowCount(); i < GetRowCount(); i++)
	{
		for (int j = 0; j < GetColumnCount(); j++)
		{
			state = GetItemState(i, j);
			SetItemState(i, j, state & ~GVIS_SELECTED);
		}
	}

	for (UINT i = 0; i < selectedRows.size(); i++)
	{
		for (int j = 0; j < GetColumnCount(); j++)
		{
			state = GetItemState(selectedRows[i] + GetFixedRowCount(), j);
			SetItemState(selectedRows[i] + GetFixedRowCount(), j, state | GVIS_SELECTED);
		}
	}
	Refresh();
}

void CGridCtrlEx::SetContentItemBkColor(int nRow, int nCol, COLORREF color)
{
	nRow += GetFixedRowCount();
	SetItemBkColour(nRow, nCol, color);
	Refresh();
}

CString CGridCtrlEx::GetContentItemText(int nRow, int nCol)
{
	return GetItemText(nRow + GetFixedRowCount(), nCol);
}

vector<int> CGridCtrlEx::GetSelectedRows()
{
	vector<int> ret;
	for (int i = 0; i < GetContentRowCount(); i++)
	{
		if (GetItemState(i + GetFixedRowCount(), 0) & GVIS_SELECTED)
			ret.push_back(i);
	}

	return ret;
}

vector<int> CGridCtrlEx::GetUnselectedRows()
{
	vector<int> ret;
	for (int i = 0; i < GetContentRowCount(); i++)
	{
		if (!(GetItemState(i + GetFixedRowCount(), 0) & GVIS_SELECTED))
			ret.push_back(i);
	}

	return ret;
}

int CGridCtrlEx::GetContentRowCount()
{
	return GetRowCount() - GetFixedRowCount();
}

UINT CGridCtrlEx::GetContentItemState(int nRow, int nCol)
{
	nRow += GetFixedRowCount();
	return GetItemState(nRow, nCol);
}

LPARAM CGridCtrlEx::GetContentItemData(int nRow, int nCol)
{
	nRow += GetFixedRowCount();
	return GetItemData(nRow, nCol);
}

BOOL CGridCtrlEx::DeleteContentRow(int nRow)
{
	BOOL ret = DeleteRow(nRow + GetFixedRowCount());
	Refresh();
	return ret;
}

void CGridCtrlEx::Clear()
{
	for (int i = 0; i < GetContentRowCount(); i++)
		for (int j  = 0; j < GetColumnCount(); j++)
			SetContentItemText(i, j, _T(""));
}

void CGridCtrlEx::FillTable(MultiRowText data)
{
	SetRowCount(GetFixedRowCount() + (int)data.size());

	for (UINT i = 0; i < data.size(); i++)
		for (UINT j = 0; j < data[i].size(); j++)
		{
			SetItemText(i + GetFixedRowCount(), j, data[i][j]);
			SetItemFormat(i + GetFixedRowCount(), j, m_dFormat);
		}

	Refresh();
}

void CGridCtrlEx::SetCheckbox(int nRow, int nCol, bool checked)
{
	nRow += GetFixedRowCount();
	SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheckEx));
	CGridCellCheckEx* pCheckbox = (CGridCellCheckEx*)GetCell(nRow, nCol);
	if (pCheckbox != NULL)
		pCheckbox->SetCheck(checked);
	Refresh();
}

void CGridCtrlEx::SetCombobox(int nRow, int nCol, const vector<CString>& options, CString sSel)
{
	nRow += GetFixedRowCount();
	SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCombo));
	int nSel = -1;
	
	CGridCellCombo* pCombobox = (CGridCellCombo*)GetCell(nRow, nCol);
	pCombobox->SetFormat(m_dFormat);
	pCombobox->SetStyle(CBS_DROPDOWNLIST);
	CStringArray ar;
	for (UINT i = 0; i < options.size(); i++)
	{
		ar.Add(options[i]);
		if (options[i] == sSel)
			nSel = i;
	}
	pCombobox->SetOptions(ar);
	if (nSel != -1)
		pCombobox->SetCurSel(nSel);
	Refresh();
}

void CGridCtrlEx::OnBeginDrag()
{
	COleDataSource* pSource = NULL;
	if (!m_bAllowDragAndDrop && m_CurCol==-1)
		return;

	if (m_CurCol>=0)	pSource = new COleDataSource();
	if (!pSource && m_bAllowDragAndDrop) pSource = CopyTextFromGrid();
	if (pSource)
	{
		SendMessageToParent(GetSelectedCellRange().GetTopLeft().row,
			GetSelectedCellRange().GetTopLeft().col,
			GVN_BEGINDRAG);

		m_MouseMode = MOUSE_DRAGGING;
		m_bLMouseButtonDown = FALSE;

		DROPEFFECT dropEffect = pSource->DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);

		if (pSource)
			delete pSource;

	}
}

BOOL CGridCtrlEx::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	m_MouseMode = MOUSE_NOTHING;
	if(m_CurCol == -1)
	{
		if (!m_bAllowDragAndDrop || (!IsCellEditable(m_LastDragOverCell) && !m_bDragRowMode))
			return FALSE;
	}
	OnDragLeave();
	CCellID cellID = GetCellFromPt(point);
	CString cellMsg;
	if (cellID.row < GetFixedRowCount() || cellID.col < 0)
		return FALSE;
	GetParent()->PostMessage(WM_GRIDITEM_MOVE, cellID.row - GetFixedRowCount(), cellID.col);
	return TRUE;
}
