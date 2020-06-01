#pragma once
#include "ListBoxEx.h"

#define COLOR_COMBOBOX_TEXT			RGB(174, 175, 178)
#define COLOR_COMBOBOX_BK			RGB(32, 34, 37)
#define COLOR_COMBOBOX_NORMAL		RGB(47, 49, 54)
#define COLOR_COMBOBOX_HOVER		RGB(83, 81, 107)


class CComboBoxFx : public CWindowImpl<CComboBoxFx, CComboBox>
{

public:

	BEGIN_MSG_MAP(CComboBoxFx)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_PAINT(OnPaint)
		MSG_OCM_DRAWITEM(OnReflectedDrawItem)
		MSG_OCM_MEASUREITEM(OnReflectedMeasureItem)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSEHOVER(OnMouseHover)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_KILLFOCUS(OnKillFocus)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	CListBoxEx list;
	COMBOBOXINFO m_cinfo;
	CRect m_rtCombo, m_rtBtn;
	CImage m_iDrop;
	BOOL m_bHover = FALSE;
public:


	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd == NULL);
		ATLASSERT(::IsWindow(hWnd));
		BOOL bRet = CWindowImpl<CComboBoxFx, CComboBox>::SubclassWindow(hWnd);
		m_cinfo.cbSize = sizeof(COMBOBOXINFO);
		GetComboBoxInfo(&m_cinfo);

		list.SubclassWindow(m_cinfo.hwndList);
		list.ModifyStyle(WS_BORDER, LBS_OWNERDRAWVARIABLE);

		m_rtCombo = m_cinfo.rcItem;
		m_rtBtn = m_cinfo.rcButton;
		LoadImageFromResource(&m_iDrop, IDB_DROPLIST, TEXT("png"));  // ▼

		if (bRet == FALSE) return bRet;
		return bRet;
	}

	BOOL OnEraseBkgnd(CDCHandle dc)
	{
		return TRUE;
	}

	void OnKillFocus(CWindow wndFocus)
	{
	}

	void OnPaint(CDCHandle handledc)
	{
		CPaintDC dc(m_hWnd);
		CRect rect;
		GetClientRect(rect);

		if	 (m_bHover) SetFocus();
		else OnKillFocus(GetFocus());

		dc.FillSolidRect(rect, m_bHover ? COLOR_COMBOBOX_HOVER : COLOR_COMBOBOX_NORMAL);
		CString str;
		GetWindowText(str);
		dc.SelectFont(GetFont());
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(COLOR_COMBOBOX_TEXT);
		rect.left += 3;
		dc.DrawText(str, -1, rect, DT_SINGLELINE | DT_VCENTER);
		m_iDrop.TransparentBlt(dc, m_rtBtn.left + 1, m_rtBtn.top + 6, m_iDrop.GetWidth(), m_iDrop.GetHeight(), RGB(255, 255, 255));
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 1;
		tme.hwndTrack = m_hWnd;
		TrackMouseEvent(&tme);
	}

	void OnMouseHover(WPARAM wParam, CPoint ptPos)
	{
		if (!m_bHover)
		{
			m_bHover = TRUE;
			Invalidate();
		}
	}

	void OnMouseLeave()
	{
		if (m_bHover)
		{
			m_bHover = FALSE;
			Invalidate();
		}
	}

	void OnReflectedDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS)
	{
		list.OnReflectedDrawItem(nIDCtl, lpDIS);
	}

	void OnReflectedMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMIS)
	{
		list.OnReflectedMeasureItem(nIDCtl, lpMIS);
	}
};

