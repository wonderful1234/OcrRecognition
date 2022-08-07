#include "OcrRecognition.h"
#include "OcrManage.h"
#include "ui_OcrRecognition.h"
#include <windows.h>
#include <QDesktopWidget>
#include "freesnapdialog.h"
#include "screenshothelper.h"
#include "thread"
#include <QFileDialog>
#include <QSettings>
#include <QClipboard>
OcrRecognition::OcrRecognition(QWidget *parent)
    : PopWidgetBase(parent), ui(new Ui::OcrRecognition)
{
    ui->setupUi(this);
	ui->imagewidget->hide();
	ui->textEdit->hide();
	m_shortcut = new MyGlobalShortCut("", this, "");
	setHotKeyLabel();
	ui->stackedWidget->setCurrentIndex(0);
	movie = new QMovie(":/OcrRecognition/res/load.gif");
	connect(m_shortcut, SIGNAL(activatedHotKey(int)), this, SLOT(hotKeyPressed(int)));

	connect(this, &OcrRecognition::signGetResult, this, [&](const QString& result, const QString& fileName) {
		movie->stop();
		ui->stackedWidget->setCurrentIndex(0);
		ui->btnAdd->show();
		ui->imagewidget->show();
		ui->textEdit->show();
		ui->textEdit->setText(result);
		ui->imagewidget->setPixmap(QPixmap(fileName));
		});

	connect(ui->btnMine, &QPushButton::clicked, this, [&]() {
		showMinimized();
		});

	connect(ui->btnClose, &QPushButton::clicked, this, [&]() {
		if (m_shortcut)
		{
			m_shortcut->unregisterHotKey();
			m_shortcut->deleteLater();
			m_shortcut = nullptr;
		}
		if (movie)
		{
			movie->deleteLater();
			movie = nullptr;
		}
		this->close();
		TerminateProcess(GetCurrentProcess(), 0);
		});

	connect(ui->btnAdd, &QPushButton::clicked, this, [&]() {
		auto picturePath = QFileDialog::getOpenFileName(this, u8"ÇëÑ¡ÔñÍ¼Æ¬", "", "(*.png *.jpg)");
		if (!picturePath.isEmpty())
		{
			dealPicture(picturePath);
		}
		});
}

OcrRecognition::~OcrRecognition()
{
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
	getDpi();
	QPixmap* picture = new QPixmap;
	FreeSnapDialog freeSnapDialog(ScreenShotHelper::grabWindow(1, (HWND)QApplication::desktop()->winId(), m_dpi),
		picture, this, m_dpi);
	freeSnapDialog.exec();
	if (picture)
	{
		this->showNormal();
		QString fileName = QApplication::applicationDirPath() + "/ScreenShot.png";
		picture->save(fileName, "png");
		QApplication::clipboard()->setPixmap(QPixmap(fileName));
		dealPicture(fileName);
		
	}
}

void OcrRecognition::dealPicture(const QString& fileName)
{
	ui->btnAdd->hide();
	ui->label_load->setMovie(movie);
	movie->start();
	ui->stackedWidget->setCurrentIndex(1);
	std::thread task = std::thread([=]() {

		QString result = OcrManage::getInstance().getTexts(fileName);
		emit signGetResult(result, fileName);
		});
	task.detach();
}

void OcrRecognition::setHotKeyLabel()
{
	QSettings qSettings(QCoreApplication::applicationDirPath() + "/HotKey.ini", QSettings::IniFormat);
	QString code = qSettings.value("key").toString();
	if (!code.isEmpty())
	{
		ui->label->setText(QString(u8"½ØÆÁ¿ì½Ý¼ü£ºctrl+shift+%1").arg(code));
	}
	
}

void OcrRecognition::hotKeyPressed(int i)
{
	showScreenShot();
}
