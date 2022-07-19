#include "OcrRecognition.h"
#include "OcrManage.h"
#include "ui_OcrRecognition.h"
#include <windows.h>
#include <QDesktopWidget>
#include "freesnapdialog.h"
#include "screenshothelper.h"
OcrRecognition::OcrRecognition(QWidget *parent)
    : PopWidgetBase(parent), ui(new Ui::OcrRecognition)
{
    ui->setupUi(this);
	m_shortcut = new MyGlobalShortCut("", this, "");
	connect(m_shortcut, SIGNAL(activatedHotKey(int)), this, SLOT(hotKeyPressed(int)));
}

OcrRecognition::~OcrRecognition()
{
	m_shortcut->unregisterHotKey();
	m_shortcut->deleteLater();
	m_shortcut = nullptr;
    delete ui;
}


void OcrRecognition::getDpi()
{
	HDC desktopDc = GetDC(NULL);
	float horizontalDPI = GetDeviceCaps(desktopDc, LOGPIXELSX);
	float verticalDPI = GetDeviceCaps(desktopDc, LOGPIXELSY);
	int dpi = (horizontalDPI + verticalDPI) / 2;
	m_dpi = 1 + ((dpi - 96) / 24) * 0.25;
	::ReleaseDC(NULL, desktopDc);
}

void OcrRecognition::showScreenShot()
{
	QPixmap* picture = new QPixmap;
	FreeSnapDialog freeSnapDialog(ScreenShotHelper::grabWindow(1, (HWND)QApplication::desktop()->winId(), m_dpi),
		picture, this, m_dpi);
	freeSnapDialog.exec();
	if (picture)
	{
		QString fileName = QApplication::applicationDirPath() + "/ScreenShot.png";
		picture->save(fileName, "png");
		OcrManage::getInstance()->getTexts(fileName);
	}
}

void OcrRecognition::hotKeyPressed(int i)
{
	showScreenShot();
}
