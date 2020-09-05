
#pragma once
#include "stdafx.h"
#include "resource.h"

class CSelColDlg : public CDialogImpl<CSelColDlg>
{
public:
	enum { IDD = IDD_SELCOLOR };

	BEGIN_MSG_MAP(CSelColDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_STARTTIMER, OnStartTimer)
		MESSAGE_HANDLER(WM_ENDTIMER, OnEndTimer)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)

	END_MSG_MAP()

public:
	BOOL m_bShow = FALSE;
	HCURSOR m_cur;
	COLORREF m_pixcol;
public:

	LRESULT CSelColDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		ModifyStyleEx(0, SWP_HIDEWINDOW);
		m_cur = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_BLUE_ARROW));

		CenterWindow(GetParent());
		return TRUE;
	}

	BOOL CSelColDlg::OnEraseBkgnd(CDCHandle dc)
	{
		return TRUE;
	}

	LRESULT CSelColDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CPaintDC dc(m_hWnd);
		CRect rc;
		CPoint pt;
		GetClientRect(rc);
		if(m_bShow)
		{
			GetCursorPos(&pt);
			CDC sDC = ::GetDC(NULL);

			// 放大光标所在矩形区域内图像
			int ratio = 7;
			dc.StretchBlt(rc.left, rc.top,
				rc.Width(), rc.Height(),
				sDC,
				pt.x - rc.Width() / ratio / 2,
				pt.y - rc.Height() / ratio / 2,
				rc.Width() / ratio,
				rc.Height() / ratio,
				SRCCOPY);
		}
		else
			dc.FillSolidRect(rc, RGB(255, 255, 255));

		return DefWindowProc();
	}

	void CSelColDlg::OnTimer(UINT_PTR nIDEvent)
	{
		Invalidate(FALSE);
	}

	BOOL CSelColDlg::OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
	{
		SetCursor(m_cur);
		return TRUE;
	}

	void CSelColDlg::OnLButtonDown(UINT nFlags, CPoint point)
	{
		CDC dc = GetDC();
		COLORREF c = GetPixel(dc, point.x, point.y);
		OnMouseMove(nFlags, point);

	}

	void CSelColDlg::OnMouseMove(UINT nFlags, CPoint point)
	{
		CDC dc = GetDC();
		if (nFlags & MK_LBUTTON)
		{
			m_pixcol = GetPixel(dc, point.x, point.y);
			::SendMessage(GetParent(), WM_UPDATECOLOR, UPDATE_CHOOSE_COLOR, m_pixcol);
		}
	}

	// 开启定时器
	LRESULT CSelColDlg::OnStartTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SetTimer(1, 100, NULL);
		return TRUE;
	}

	// 关闭定时器
	LRESULT CSelColDlg::OnEndTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		KillTimer(1);
		return TRUE;
	}
};
