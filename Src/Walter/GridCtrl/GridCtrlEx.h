#pragma once

#include "GridCellCheckEx.h"
#include "GridCtrl_src/NewCellTypes/GridCellCombo.h"

#define WM_GRIDITEM_MOVE (WM_USER + 1003)

using namespace std;

typedef vector<CString> OneRowText;
typedef vector<OneRowText> MultiRowText;

vector<CString> Split(CString cstr, TCHAR key);

class CGridCtrlEx : public CGridCtrl
{
	DECLARE_DYNCREATE(CGridCtrlEx)

public:
	CGridCtrlEx();

	void LoadDefaltSettings();
	void SetDefaultFormat(UINT style);

	//���ñ�ͷ�ı�����;��Ϊ�ָ���
	void SetHeaderText(CString sText);

	//�����п���;��Ϊ�ָ�������λ�ǰٷֱ�
	//m-n��ʾ��m%��n%֮���Զ�ȡֵ(m,n��ʡ��)��+��ʾռ��ʣ��ͻ���
	void SetHeaderWidth(CString sWidth);

	void SetColumnEditabe(CString sCol);
	void SetContentItemEditable(int nRow, int nCol, bool bEditable);
	void SetContentItemText(int nRow, int nCol, CString text);
	void SetContentItemState(int nRow, int nCol, UINT state);
	void SetContentItemFormat(int nRow, int nCol, UINT format);
	void SetContentItemData(int nRow, int nCol, LPARAM nValue);
	void SetSelectedRows(const vector<int> selectedRows);
	void SetContentItemBkColor(int nRow, int nCol, COLORREF color);

	CString GetContentItemText(int nRow, int nCol);
	vector<int> GetSelectedRows();
	vector<int> GetUnselectedRows();
	int GetContentRowCount();
	UINT GetContentItemState(int nRow, int nCol);
	LPARAM GetContentItemData(int nRow, int nCol);

	BOOL DeleteContentRow(int nRow);
	void Clear();

	void FillTable(MultiRowText data);
	void SetCheckbox(int nRow, int nCol, bool checked);
	void SetCombobox(int nRow, int nCol, const vector<CString>& options, CString sSel = _T(""));

	void OnBeginDrag();
	BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
protected:
	UINT m_dFormat;
};