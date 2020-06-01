
#pragma once
#include "stdafx.h"
#include "resource.h"
#include "SelColWnd.h"


class CDropDlg : public CDialogImpl<CDropDlg>
{
public:
	enum { IDD = IDD_DROPPER };

	BEGIN_MSG_MAP(CDropDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
	END_MSG_MAP()

public:
	HCURSOR m_cur;
	HWND m_selCol;

public:

	LRESULT CDropDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		ModifyStyleEx(0, SWP_HIDEWINDOW);
		m_cur = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURDROP));

		CenterWindow(GetParent());
		return TRUE;
	}

	BOOL CDropDlg::OnEraseBkgnd(CDCHandle dc)
	{
		return TRUE;
	}

	BOOL CDropDlg::OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
	{
		SetCursor(m_cur);
		return TRUE;
	}

	void CDropDlg::OnLButtonDown(UINT nFlags, CPoint point)
	{
		::SendMessage(m_selCol, WM_ENDTIMER, 0, 0);
		ShowWindow(SW_HIDE);
	}

};
