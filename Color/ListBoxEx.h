#pragma once


class CListBoxEx : public CWindowImpl<CListBoxEx, CListBox>
{

public:

	BEGIN_MSG_MAP(CListBoxEx)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

public:
	COLORREF
	COLOR_LISTBOX_TEXT		= RGB(174, 175, 178),
	COLOR_LISTBOX_BK		= RGB(32, 34, 37),
	COLOR_LISTBOX_NORMAL	= RGB(47, 49, 54),
	COLOR_LISTBOX_SELECTED	= RGB(83, 81, 107);

	int TEXT_MARGIN;

public:

	BOOL OnEraseBkgnd(CDCHandle dc)
	{
		return TRUE;
	}

	void OnReflectedDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS)
	{
		CDCHandle dc(lpDIS->hDC);
		CRect rect(lpDIS->rcItem), rt = rect;
		TEXT_MARGIN = 3;

		if (IDC_COMMENU == nIDCtl){
			COLOR_LISTBOX_TEXT     = RGB(229, 229, 229);
			COLOR_LISTBOX_BK	   = RGB(63, 63, 63);
			COLOR_LISTBOX_SELECTED = RGB(96, 96, 96);
		}

		COLORREF clList = COLOR_LISTBOX_BK;

		if (lpDIS->itemState & ODS_SELECTED)
			clList = COLOR_LISTBOX_SELECTED;

		dc.FillSolidRect(rt, clList);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(COLOR_LISTBOX_TEXT);
		CString str;
		GetText(lpDIS->itemID, str);
		rt.left += TEXT_MARGIN;
		dc.DrawText(str, -1, rt, DT_VCENTER | DT_SINGLELINE);
	}

	void OnReflectedMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMIS)
	{
		lpMIS->itemHeight = 20;
		if (IDC_COMMENU == nIDCtl) lpMIS->itemHeight = 23;
	}
};

