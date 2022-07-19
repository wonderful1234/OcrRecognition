#include "screenshothelper.h"
#include <QtWinExtras/qwinfunctions.h>
#include <windows.h>
ScreenShotHelper::ScreenShotHelper()
{

}

QPixmap ScreenShotHelper::grabWindow(int snapMethod, HWND winId,double dpi, int x, int y, int w, int h)
{
	RECT r = { 0, 0, 0, 0 };
	GetWindowRect(winId, &r);

	//多屏支持，全屏截图时始终不包含阴影
	r.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	r.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	r.right = GetSystemMetrics(SM_CXVIRTUALSCREEN) + r.left;
	r.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN) + r.top;

	if (w < 0) w = r.right - r.left;
	if (h < 0) h = r.bottom - r.top;

	long xBorder, yBorder, captionBorder;

	getWindowBorderSize(winId, &xBorder, &yBorder, &captionBorder);

	HDC display_dc = NULL;
	if (snapMethod == 0) {
		display_dc = GetWindowDC(winId);
	}
	else if (snapMethod == 1) {
		display_dc = GetWindowDC(0);
	}

	HDC bitmap_dc = CreateCompatibleDC(display_dc);
	HBITMAP bitmap = NULL;
	bitmap = CreateCompatibleBitmap(display_dc, w, h);
	
	HGDIOBJ null_bitmap = SelectObject(bitmap_dc, bitmap);
	if (snapMethod == 0) {
		BitBlt(bitmap_dc, 0, 0, w, h, display_dc, x, y, SRCCOPY | CAPTUREBLT);
	}
	else if (snapMethod == 1) {
		BitBlt(bitmap_dc, 0, 0, w, h, display_dc, r.left + x, r.top + y, SRCCOPY | CAPTUREBLT);
	}

	// clean up all but bitmap
	ReleaseDC(winId, display_dc);
	SelectObject(bitmap_dc, null_bitmap);
	DeleteDC(bitmap_dc);

	QPixmap pixmap = QtWin::fromHBITMAP(bitmap);
	pixmap.setDevicePixelRatio(dpi);
	DeleteObject(bitmap);

	return pixmap;
}

void ScreenShotHelper::getWindowBorderSize(HWND hwnd, long * xBorder, long * yBorder, long * captionBorder)
{
	long windowStyle = GetWindowLong(hwnd, GWL_STYLE);
	long windowExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

	(*captionBorder) = 0;
	(*xBorder) = 0;
	(*yBorder) = 0;

	if (windowExStyle & WS_EX_TOOLWINDOW) {
		if (windowStyle & WS_CAPTION) {                //有标题栏和边框
			(*captionBorder) = GetSystemMetrics(SM_CYSMCAPTION);
			if (windowStyle & WS_THICKFRAME) {      //尺寸可变
				(*xBorder) = GetSystemMetrics(SM_CXFRAME);
				(*yBorder) = GetSystemMetrics(SM_CYFRAME);
			}
			else {
				(*xBorder) = GetSystemMetrics(SM_CXBORDER);
				(*yBorder) = GetSystemMetrics(SM_CYBORDER);
			}
			return;
		}
		else {
			(*captionBorder) = 0;
			(*xBorder) = 0;
			(*yBorder) = 0;
			return;
		}
	}
	else {
		if (windowStyle & WS_CAPTION) {                //有标题栏和边框
			(*captionBorder) = GetSystemMetrics(SM_CYCAPTION);
			if (windowStyle & WS_THICKFRAME) {      //尺寸可变
				(*xBorder) = GetSystemMetrics(SM_CXFRAME);
				(*yBorder) = GetSystemMetrics(SM_CYFRAME);
			}
			else {
				(*xBorder) = GetSystemMetrics(SM_CXBORDER);
				(*yBorder) = GetSystemMetrics(SM_CYBORDER);
			}
			return;
		}
		else {
			(*captionBorder) = 0;
			(*xBorder) = 0;
			(*yBorder) = 0;
			return;
		}
	}
}
