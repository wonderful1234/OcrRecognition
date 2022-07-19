#ifndef SCREENSHOTHELPER_H
#define SCREENSHOTHELPER_H

#include <QPixmap>
class ScreenShotHelper
{
public:
	ScreenShotHelper();

	static QPixmap grabWindow(int snapMethod, HWND winId, double dpi = 1,int x = 0, int y = 0, int w = -1, int h = -1);
	static void getWindowBorderSize(HWND hwnd, long* xBorder, long* yBorder, long* captionBorder);

};

#endif // SCREENSHOTHELPER_H
