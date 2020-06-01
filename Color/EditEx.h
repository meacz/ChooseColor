#pragma once

class CEditEx : public CWindowImpl<CEditEx, CEdit>
{

public:

	BEGIN_MSG_MAP(CEditEx)
		MSG_WM_NCPAINT(OnNcPaint)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

public:
	int i = 0;
	HCURSOR m_ibeam = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_BLUE_IBEAM));


public:

	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd == NULL);
		ATLASSERT(::IsWindow(hWnd));
		BOOL bRet = CWindowImpl<CEditEx, CEdit>::SubclassWindow(hWnd);

		if (bRet == FALSE)
			return bRet;

		CRect rect;
		GetClientRect(&rect);
		rect.left += 5;
		rect.top += 5;
		SetRect(rect);
		LimitText(10);

		return bRet;
	}

	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		int nID = ::GetDlgCtrlID(::GetFocus());
		int value = ::GetDlgItemInt(GetParent(), nID, 0, 0);
		value += zDelta / 120;
		value = max(value, 0);
		::SendMessage(GetParent(), WM_UPDATECOLOR, UPDATE_WHEEL_COLOR, MAKELONG(nID, value));

		//ATLTRACE(L"%d %d\n", i, value);
		return FALSE;
	}

	void OnNcPaint(CRgnHandle rgn)
	{
		CDC dc = GetDC();
		CRect rect;
		CPen pen = CreatePen(PS_SOLID, 1, RGB(54, 57, 62));
		GetClientRect(rect);
		rect.InflateRect(1, 1, 1, 1);
		dc.SelectStockBrush(NULL_BRUSH);
		dc.SelectPen(pen);
		dc.Rectangle(rect);
		dc.DeleteDC();
	}

	BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
	{
		SetCursor(m_ibeam);
		return TRUE;
	}

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		if (GetKeyState(VK_CONTROL) & 0x80 && (char)wParam == 'A') SetSel(0, -1);

		return DefWindowProc(uMsg, wParam, lParam);
	}

};