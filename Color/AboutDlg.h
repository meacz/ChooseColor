
#pragma once

#define COLOR_DLG_BK1		RGB(47, 49, 54)
#define COLOR_DLG_BK2		RGB(32, 34, 37)
#define COLOR_DLG_TEXT		RGB(174, 175, 178)

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTDLG };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_SETCURSOR(OnSetCursor)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

public:
	int m_ncHeight = 28;  // 标题栏高度
	CBitmapBtn m_ok;
	HCURSOR m_hcur = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_BLUE_ARROW));

public:

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_ok.SubclassWindow(GetDlgItem(IDOK));

		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return TRUE;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		ScreenToClient(&pt);
		CRect rect;
		GetClientRect(rect);
		rect.bottom = m_ncHeight;
		return rect.PtInRect(pt) ? HTCAPTION : TRUE;
	}

	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		EndDialog(IDD_ABOUTDLG);
	}

	LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HDC hdc = (HDC)wParam;
		HBRUSH hbr = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		::SetTextColor(hdc, COLOR_DLG_TEXT);
		::SetBkMode(hdc, TRANSPARENT);
		return (LRESULT)hbr;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(m_hWnd);
		CRect rtBk, rt;
		GetClientRect(rtBk);
		rt = rtBk;
		// 对话框背景
		dc.FillSolidRect(rtBk, COLOR_DLG_BK1);
		rt.DeflateRect(4, m_ncHeight, 4, 4);
		dc.FillSolidRect(rt, COLOR_DLG_BK2);

		dc.SetBkMode(TRANSPARENT);
		dc.SelectFont(GetFont());
		dc.SetTextColor(COLOR_DLG_TEXT);
		dc.DrawText(TEXT("cmd v1.3"), -1, CRect(5, 8, 160, 29), DT_LEFT | DT_TOP | DT_SINGLELINE);

		CFont f, ft;
		LOGFONT lf;
		HFONT font = GetFont();
		f.Attach(font);
		f.GetLogFont(&lf);
		_tcscpy_s(lf.lfFaceName, _countof(lf.lfFaceName), TEXT("微软雅黑"));
		lf.lfHeight = 21;
		ft.CreateFontIndirect(&lf);

		GetDlgItem(IDC_TEXT).SetFont(ft);
		f.Detach();
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
	{
		SetCursor(m_hcur);
		return TRUE;
	}
};
