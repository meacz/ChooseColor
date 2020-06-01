#pragma once
#include "resource.h"

#define OFS_X	10
#define COLOR_GRCOLOR_BOX		RGB(54, 57, 62)
#define COLOR_GRCOLOR_BOX_TEXT  RGB(174, 175, 178)

class CGroupBoxEx : public CWindowImpl<CGroupBoxEx, CButton>
{

public:
	BEGIN_MSG_MAP_EX(CGroupBoxEx)
		MSG_WM_PAINT(OnPaint)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

public:
	CFont m_font;

public:

	void OnPaint(CDCHandle)
	{
		
		CPaintDC dc(m_hWnd);
		CRect rc;
		CPoint ptStart, ptEnd;
		CSize seText;
		int iUpDist;
		CString sText, sTemp;
		CPen pnFrmDark, pnFrmLight, *ppnOldPen;
		DWORD dwStyle, dwExStyle;

		GetWindowRect(&rc);
		ScreenToClient(&rc);

		//HFONT hFont = (HFONT)GetFont();
		//LOGFONT lf;
		//GetObject(hFont, sizeof(lf), &lf);
		//wcscpy(lf.lfFaceName, L"ºÚÌå");
		//m_font.CreateFontIndirect(&lf);
		dc.SelectFont(GetFont());

		// determine text length
		GetWindowText(sTemp);
		sText.Format(TEXT(" %s "), sTemp);
		dc.GetTextExtent(sText, sText.GetLength(), &seText);

		iUpDist = (seText.cy / 2);

		// calc rect and start/end points
		dwStyle = GetStyle();
		dwExStyle = GetExStyle();

		// handle text alignment (Caution: BS_CENTER == BS_LEFT|BS_RIGHT!!!)
		ptStart.y = ptEnd.y = rc.top + iUpDist;
		if ((dwStyle & BS_CENTER) == BS_RIGHT) // right aligned
		{
			ptEnd.x = rc.right - OFS_X;
			ptStart.x = ptEnd.x - seText.cx;
		}
		else if ((!(dwStyle & BS_CENTER)) || ((dwStyle & BS_CENTER) == BS_LEFT))// left aligned	/ default
		{
			ptStart.x = rc.left + OFS_X;
			ptEnd.x = ptStart.x + seText.cx;
		}
		else if ((dwStyle & BS_CENTER) == BS_CENTER) // text centered
		{
			ptStart.x = (rc.Width() - seText.cx) / 2;
			ptEnd.x = ptStart.x + seText.cx;
		}

		pnFrmDark.CreatePen(PS_SOLID, 1, COLOR_GRCOLOR_BOX);
		//pnFrmLight.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DHILIGHT));

		ppnOldPen = (CPen*)dc.SelectPen(pnFrmDark);

		dc.MoveTo(ptStart);
		dc.LineTo(rc.left, ptStart.y);
		dc.LineTo(rc.left, rc.bottom - 1);
		dc.LineTo(rc.right - 1, rc.bottom - 1);
		dc.LineTo(rc.right - 1, ptEnd.y);
		dc.LineTo(ptEnd);

		// draw text (if any)
		if (!sText.IsEmpty() && !(dwExStyle & (BS_ICON | BS_BITMAP)))
		{
			if (!IsWindowEnabled())
			{
				ptStart.y -= iUpDist;
				dc.DrawState(ptStart, seText, sText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
			}
			else
			{
				dc.SetBkMode(TRANSPARENT);
				dc.SetTextColor(COLOR_GRCOLOR_BOX_TEXT);
				dc.DrawText(sText, -1, CRect(ptStart, ptEnd), DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_NOCLIP);
			}
		}
	}
};

