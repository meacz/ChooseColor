
#pragma once
#include "stdafx.h"
#include "resource.h"

class CPicColDlg : public CDialogImpl<CPicColDlg>
{
public:
	enum { IDD = IDD_PICCOL };

	BEGIN_MSG_MAP(CPicColDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
	END_MSG_MAP()

public:
	HCURSOR m_cur;
	HWND m_par;
	COLORREF m_pixcol;

public:

	LRESULT CPicColDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//ModifyStyleEx(0, SWP_HIDEWINDOW);
		m_cur = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_BLUE_ARROW));

		CenterWindow(GetParent());
		return TRUE;
	}

	BOOL CPicColDlg::OnEraseBkgnd(CDCHandle dc)
	{
		return TRUE;
	}

	LRESULT CPicColDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CPaintDC dc(m_hWnd);
		CRect rc, rt1;
		CPoint pt;
		GetClientRect(rc);

		COLORREF col[4][8] = {

			{ RGB(0xFF, 0xC0, 0xCB), RGB(0xC0, 0xC0, 0xC0),
			RGB(0x69, 0x53, 0xB7), RGB(0xAA, 0xFF, 0xAA),
			RGB(0x40, 0xE0, 0xD0), RGB(0xC0, 0xFF, 0xFF),
			RGB(0x48, 0x00, 0x48), RGB(0xC0, 0xC0, 0xC0) },

			{ RGB(0xEF, 0xB7, 0x73), RGB(0x54, 0x95, 0xD3),
			RGB(0x32, 0xCD, 0x32), RGB(0x3C, 0xB3, 0x71),
			RGB(0x7F, 0xFF, 0xD4), RGB(0x7D, 0x9E, 0xC0),
			RGB(0x80, 0x00, 0x80), RGB(0x7F, 0x7F, 0x7F) },

			{ RGB(0xFF, 0xE4, 0xE1), RGB(0xD2, 0xB4, 0x8C),
			RGB(0xFF, 0xFF, 0xE0), RGB(0x98, 0xFB, 0x98),
			RGB(0xAF, 0xEE, 0xEE), RGB(0x68, 0x83, 0x8B),
			RGB(0xE6, 0xE6, 0xFA), RGB(0xFF, 0xFF, 0xFF) },
		
			{RGB(0xA0, 0x6F, 0xA5), RGB(0xBA, 0xA4, 0x9A),
			RGB(0x8B, 0x8B, 0x00), RGB(0x00, 0x93, 0x00),
			RGB(0x38, 0x8E, 0x8E), RGB(0x00, 0x00, 0xFF),
			RGB(0x7B, 0x7B, 0xC0), RGB(0x66, 0x66, 0x66)}};

		int w = 15;

		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				rt1.SetRect(x*w, w*y, (x + 1)*w, (y+1)*w);
				dc.FillSolidRect(rt1, col[y][x]);
			}
		}
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}


	BOOL CPicColDlg::OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
	{
		SetCursor(m_cur);
		return TRUE;
	}

	void CPicColDlg::OnLButtonDown(UINT nFlags, CPoint point)
	{
		OnMouseMove(nFlags, point);
	}

	void CPicColDlg::OnMouseMove(UINT nFlags, CPoint point)
	{
		CDC dc = GetDC();
		if (nFlags & MK_LBUTTON)
		{
			m_pixcol = GetPixel(dc, point.x, point.y);
			::SendMessage(m_par, WM_UPDATECOLOR, UPDATE_CHOOSE_COLOR, m_pixcol);
		}
	}
};
