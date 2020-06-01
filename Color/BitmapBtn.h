#pragma once
#include "resource.h"
#include "ListBoxEx.h"

#define COLOR_CLOSE_BTN_SELECTED  RGB(235, 59, 58)
#define COLOR_CLOSE_BTN_HOVER     RGB(214, 97, 96)
#define COLOR_CLOSE_BTN_NORMAL    RGB(47, 49, 54)
#define COLOR_ORDER_BTN_NORMAL    RGB(47, 49, 54)
#define COLOR_ORDER_BTN_HOVER     RGB(83, 81, 107)
#define COLOR_ORDER_BTN_SELECTED  RGB(79, 83, 91)
#define COLOR_BTN_TEXT			  RGB(214, 219, 229)


class CBitmapBtn : public CWindowImpl<CBitmapBtn, CButton>
{
public:
	BEGIN_MSG_MAP_EX(CBitmapBtn)
		MSG_OCM_DRAWITEM(OnReflectedDrawItem)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

public:
	CRect m_rect;
	COLORREF m_color;
	BOOL m_bHover = FALSE;
	CImage m_iclose, m_imin;

public:

	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd == NULL);
		ATLASSERT(::IsWindow(hWnd));
		BOOL bRet = CWindowImpl<CBitmapBtn, CButton>::SubclassWindow(hWnd);

		LoadImageFromResource(&m_iclose, IDB_CLOSE, TEXT("PNG"));  // close png
		LoadImageFromResource(&m_imin, IDB_MIN, TEXT("PNG"));      // min png

		if (bRet == FALSE)
			return bRet;

		return bRet;
	}

void OnReflectedDrawItem(UINT nIDCtl, LPDRAWITEMSTRUCT lpDIS)
	{
		CDC dc;
		dc.Attach(lpDIS->hDC);
		CRect rt, rect(lpDIS->rcItem);
		m_rect = rect;
		m_color = COLOR_ORDER_BTN_NORMAL;
		
		if (m_bHover)
		{
			m_color = COLOR_ORDER_BTN_HOVER;
			if (lpDIS->CtlID == IDC_CLOSE)
				m_color = COLOR_CLOSE_BTN_HOVER;

		}

		if (lpDIS->itemState & ODS_SELECTED)
		{
			m_color = COLOR_ORDER_BTN_SELECTED;
			if (lpDIS->CtlID == IDC_CLOSE)
				m_color = COLOR_CLOSE_BTN_SELECTED;
		}

		dc.FillSolidRect(rect, m_color);

		if (lpDIS->CtlID == IDC_CLOSE)
			m_iclose.TransparentBlt(dc, CRect(5, 4, 10 + 5, 10 + 4), RGB(255, 255, 255));

		if (lpDIS->CtlID == IDC_MIN)
			m_imin.TransparentBlt(dc, CRect(2, 1, 16 + 2, 16 + 1), RGB(255, 255, 255));

		CString szText;
		GetWindowText(szText);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(COLOR_BTN_TEXT);
		dc.SelectFont(GetFont());

		dc.DrawText(szText, -1, rect, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
		dc.Detach();
	}

void OnMouseMove(UINT nFlags, CPoint point)
	{
		if (m_rect.PtInRect(point))
		{
			if (!m_bHover)
			{
				this->SetCapture();
				m_bHover = TRUE;
				Invalidate(FALSE);
				//ATLTRACE("true\n");
			}
		}
		else
		{
			ReleaseCapture();
			if (m_bHover)
			{
				m_bHover = FALSE;
				Invalidate(FALSE);
				//ATLTRACE("false\n");
			}
		}
	}
};
