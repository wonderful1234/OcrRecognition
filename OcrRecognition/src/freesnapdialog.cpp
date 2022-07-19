#include "freesnapdialog.h"
#include "ui_freesnapdialog.h"
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <windows.h>
#include <math.h>

QColor FreeSnapDialog::rectColor(7, 200, 250);
QColor FreeSnapDialog::grayColor(0, 0, 0, 180);
QColor FreeSnapDialog::rectPointColor(200, 240, 255);

FreeSnapDialog::FreeSnapDialog(QPixmap picture, QPixmap*& result, QWidget *parent, double dpi) :
	QDialog(parent),
	ui(new Ui::FreeSnapDialog),
	resultRef(result),
	m_dpi(dpi),
	zoomRate(static_cast<float>(3.2))
{
	ui->setupUi(this);

	//    QDesktopWidget * desktop = QApplication::desktop();
	QList<QScreen *> screens = QGuiApplication::screens();

	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	setWindowState(Qt::WindowActive);
	//    SetWindowPos((HWND)this->winId() , HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	//多显示器支持
	move(GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN));
	resize(GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN));

	ui->widgetInfoContainer->adjustSize();
	ui->widgetInfoContainer->setVisible(false);
	ui->widgetInfoContainer->setAttribute(Qt::WA_TransparentForMouseEvents, true);      //信息Widget不接收鼠标事件

	//可在不点击鼠标的情况下捕获移动事件
	setMouseTracking(true);

	this->noOperate = true;
	this->picture = picture;
	this->image = picture.toImage();
	this->isDrawing = false;
	this->pictureMap = picture;
	this->pictureMap.fill(GRAYCOLOR);

	setAutoFillBackground(true);
	QPalette palette;
	palette.setBrush(QPalette::Window, QBrush(this->picture));
	setPalette(palette);
}

FreeSnapDialog::~FreeSnapDialog()
{
	if (resultRef)
	{
		delete resultRef;
		resultRef = nullptr;
	}
		
	delete ui;
}

void FreeSnapDialog::paintEvent(QPaintEvent *)
{
	int width = endPoint.x() - lastPoint.x();
	int height = endPoint.y() - lastPoint.y();
	pictureMap = picture;
	QPainter painter(this);
	painter.setPen(QPen(RECTCOLOR, RECTPENWIDTH));

	QPoint topLeftPoint(endPoint.x() > lastPoint.x() ? lastPoint.x() : endPoint.x(),
		endPoint.y() > lastPoint.y() ? lastPoint.y() : endPoint.y());
	QPoint bottomRightPoint(endPoint.x() < lastPoint.x() ? lastPoint.x() : endPoint.x(),
		endPoint.y() < lastPoint.y() ? lastPoint.y() : endPoint.y());

	resultRect = QRect(topLeftPoint*m_dpi, bottomRightPoint*m_dpi);

	if (isDrawing) {
		pictureMap2 = pictureMap;
		QPainter p(&pictureMap2);
		p.setPen(QPen(RECTCOLOR, RECTPENWIDTH));
		//上侧
		p.fillRect(0, 0, this->width(), topLeftPoint.y() - RECTPENWIDTH / 2.0, GRAYCOLOR);
		//左侧
		p.fillRect(0, topLeftPoint.y() - RECTPENWIDTH / 2.0, topLeftPoint.x() - RECTPENWIDTH / 2.0, abs(height) + RECTPENWIDTH, GRAYCOLOR);
		//右侧
		p.fillRect(bottomRightPoint.x() + RECTPENWIDTH / 2.0, topLeftPoint.y() - RECTPENWIDTH / 2.0,
			this->width() - bottomRightPoint.x() - RECTPENWIDTH / 2.0, abs(height) + RECTPENWIDTH, GRAYCOLOR);
		//下侧
		p.fillRect(0, bottomRightPoint.y() + RECTPENWIDTH / 2.0, this->width(), this->height() - bottomRightPoint.y(), GRAYCOLOR);

		//大框
		p.drawRect(lastPoint.x(), lastPoint.y(), width, height);

		//大框周围伸缩小点
		p.setPen(QPen(RECTPOINTCOLOR, 0));
		p.setBrush(QBrush(RECTPOINTCOLOR));
		//左上角的点
		p.drawRect(lastPoint.x() - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0,
			lastPoint.y() - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0,
			RECTPOINTPENWIDTH, RECTPOINTPENHEIGHT);
		//右上角的点
		p.drawRect(endPoint.x() - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0,
			lastPoint.y() - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0,
			RECTPOINTPENWIDTH, RECTPOINTPENHEIGHT);
		//左下角的点
		p.drawRect(lastPoint.x() - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0,
			endPoint.y() - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0,
			RECTPOINTPENWIDTH, RECTPOINTPENHEIGHT);
		//右下角的点
		p.drawRect(endPoint.x() - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0,
			endPoint.y() - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0,
			RECTPOINTPENWIDTH, RECTPOINTPENHEIGHT);
		painter.drawPixmap(0, 0, pictureMap2);
	}
	else {
		QPainter p(&pictureMap);
		p.setPen(QPen(RECTCOLOR, RECTPENWIDTH));
		//上侧
		p.fillRect(0, 0, this->width(), topLeftPoint.y() - RECTPENWIDTH / 2.0, GRAYCOLOR);
		//左侧
		p.fillRect(0, topLeftPoint.y() - RECTPENWIDTH / 2.0, topLeftPoint.x() - RECTPENWIDTH / 2.0, abs(height) + RECTPENWIDTH, GRAYCOLOR);
		//右侧
		p.fillRect(bottomRightPoint.x() + RECTPENWIDTH / 2.0, topLeftPoint.y() - RECTPENWIDTH / 2.0,
			this->width() - bottomRightPoint.x() - RECTPENWIDTH / 2.0, abs(height) + RECTPENWIDTH, GRAYCOLOR);
		//下侧
		p.fillRect(0, bottomRightPoint.y() + RECTPENWIDTH / 2.0, this->width(), this->height() - bottomRightPoint.y(), GRAYCOLOR);

		//大框
		p.drawRect(lastPoint.x(), lastPoint.y(), width, height);

		if (!noOperate) {
			//大框周围伸缩小点
			p.setPen(QPen(RECTPOINTCOLOR, 0));
			p.setBrush(QBrush(RECTPOINTCOLOR));
			//左上角的点
			p.drawRect(lastPoint.x() - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0,
				lastPoint.y() - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0,
				RECTPOINTPENWIDTH, RECTPOINTPENHEIGHT);
			//右上角的点
			p.drawRect(endPoint.x() - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0,
				lastPoint.y() - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0,
				RECTPOINTPENWIDTH, RECTPOINTPENHEIGHT);
			//左下角的点
			p.drawRect(lastPoint.x() - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0,
				endPoint.y() - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0,
				RECTPOINTPENWIDTH, RECTPOINTPENHEIGHT);
			//右下角的点
			p.drawRect(endPoint.x() - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0,
				endPoint.y() - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0,
				RECTPOINTPENWIDTH, RECTPOINTPENHEIGHT);
		}

		painter.drawPixmap(0, 0,pictureMap);
	}
}

void FreeSnapDialog::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		QPoint topLeftPoint(endPoint.x() > lastPoint.x() ? lastPoint.x() : endPoint.x(),
			endPoint.y() > lastPoint.y() ? lastPoint.y() : endPoint.y());
		QPoint bottomRightPoint(endPoint.x() < lastPoint.x() ? lastPoint.x() : endPoint.x(),
			endPoint.y() < lastPoint.y() ? lastPoint.y() : endPoint.y());
		if (inLeftTopPoint(event)) {
			lastPoint = bottomRightPoint;
			endPoint = topLeftPoint;
			this->noOperate = false;
			isDrawing = true;
		}
		else if (inRightTopPoint(event)) {
			lastPoint.setX(topLeftPoint.x());
			lastPoint.setY(bottomRightPoint.y());
			endPoint.setX(bottomRightPoint.x());
			endPoint.setY(topLeftPoint.y());
			this->noOperate = false;
			isDrawing = true;
		}
		else if (inLeftBottomPoint(event)) {
			lastPoint.setX(bottomRightPoint.x());
			lastPoint.setY(topLeftPoint.y());
			endPoint.setX(topLeftPoint.x());
			endPoint.setY(bottomRightPoint.y());
			this->noOperate = false;
			isDrawing = true;
		}
		else if (inRightBottomPoint(event)) {
			lastPoint = topLeftPoint;
			endPoint = bottomRightPoint;
			this->noOperate = false;
			isDrawing = true;
		}
		else {
			this->noOperate = false;
			QPainter painter(this);
			painter.drawPixmap(0, 0, pictureMap);
			isDrawing = true;
			lastPoint = event->pos();
		}
	}
}

void FreeSnapDialog::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		endPoint = event->pos();
		update();
	}
	if (inLeftTopPoint(event)) {
		setCursor(Qt::SizeFDiagCursor);
	}
	else if (inRightTopPoint(event)) {
		setCursor(Qt::SizeBDiagCursor);
	}
	else if (inLeftBottomPoint(event)) {
		setCursor(Qt::SizeBDiagCursor);
	}
	else if (inRightBottomPoint(event)) {
		setCursor(Qt::SizeFDiagCursor);
	}
	else {
		setCursor(Qt::CrossCursor);
	}

	
}

void FreeSnapDialog::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		isDrawing = false;
		endPoint = event->pos();
		update();
	}
	if (resultRect.x() > 0 && resultRect.y() > 0)
	{
		picture.copy(resultRect);
		*this->resultRef = picture.copy(resultRect);
	}
	else
	{
		delete this->resultRef;
		this->resultRef = nullptr;
	}
	close();
	return;
}

bool FreeSnapDialog::inLeftTopPoint(QMouseEvent *event)
{
	//    QPoint topLeftPoint(endPoint.x() > lastPoint.x() ? lastPoint.x() : endPoint.x(),
	//                        endPoint.y() > lastPoint.y() ? lastPoint.y() : endPoint.y());
	return event->x() >= (endPoint.x() > lastPoint.x() ? lastPoint.x() : endPoint.x()) - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0 &&
		event->x() <= (endPoint.x() > lastPoint.x() ? lastPoint.x() : endPoint.x()) - RECTPENWIDTH / 2.0 + RECTPOINTPENWIDTH / 2.0 &&
		event->y() >= (endPoint.y() > lastPoint.y() ? lastPoint.y() : endPoint.y()) - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0 &&
		event->y() <= (endPoint.y() > lastPoint.y() ? lastPoint.y() : endPoint.y()) - RECTPENWIDTH / 2.0 + RECTPOINTPENHEIGHT / 2.0;
}

bool FreeSnapDialog::inRightTopPoint(QMouseEvent *event)
{
	//    QPoint topLeftPoint(endPoint.x() > lastPoint.x() ? lastPoint.x() : endPoint.x(),
	//                        endPoint.y() > lastPoint.y() ? lastPoint.y() : endPoint.y());
	//    QPoint bottomRightPoint(endPoint.x() < lastPoint.x() ? lastPoint.x() : endPoint.x(),
	//                            endPoint.y() < lastPoint.y() ? lastPoint.y() : endPoint.y());
	return event->x() >= (endPoint.x() < lastPoint.x() ? lastPoint.x() : endPoint.x()) - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0 &&
		event->x() <= (endPoint.x() < lastPoint.x() ? lastPoint.x() : endPoint.x()) - RECTPENWIDTH / 2.0 + RECTPOINTPENWIDTH / 2.0 &&
		event->y() >= (endPoint.y() > lastPoint.y() ? lastPoint.y() : endPoint.y()) - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0 &&
		event->y() <= (endPoint.y() > lastPoint.y() ? lastPoint.y() : endPoint.y()) - RECTPENWIDTH / 2.0 + RECTPOINTPENHEIGHT / 2.0;
}

bool FreeSnapDialog::inLeftBottomPoint(QMouseEvent *event)
{
	//    QPoint topLeftPoint(endPoint.x() > lastPoint.x() ? lastPoint.x() : endPoint.x(),
	//                        endPoint.y() > lastPoint.y() ? lastPoint.y() : endPoint.y());
	//    QPoint bottomRightPoint(endPoint.x() < lastPoint.x() ? lastPoint.x() : endPoint.x(),
	//                            endPoint.y() < lastPoint.y() ? lastPoint.y() : endPoint.y());
	return event->x() >= (endPoint.x() > lastPoint.x() ? lastPoint.x() : endPoint.x()) - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0 &&
		event->x() <= (endPoint.x() > lastPoint.x() ? lastPoint.x() : endPoint.x()) - RECTPENWIDTH / 2.0 + RECTPOINTPENWIDTH / 2.0 &&
		event->y() >= (endPoint.y() < lastPoint.y() ? lastPoint.y() : endPoint.y()) - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0 &&
		event->y() <= (endPoint.y() < lastPoint.y() ? lastPoint.y() : endPoint.y()) - RECTPENWIDTH / 2.0 + RECTPOINTPENHEIGHT / 2.0;
}

bool FreeSnapDialog::inRightBottomPoint(QMouseEvent *event)
{
	//    QPoint bottomRightPoint(endPoint.x() < lastPoint.x() ? lastPoint.x() : endPoint.x(),
	//                            endPoint.y() < lastPoint.y() ? lastPoint.y() : endPoint.y());
	return event->x() >= (endPoint.x() < lastPoint.x() ? lastPoint.x() : endPoint.x()) - RECTPENWIDTH / 2.0 - RECTPOINTPENWIDTH / 2.0 &&
		event->x() <= (endPoint.x() < lastPoint.x() ? lastPoint.x() : endPoint.x()) - RECTPENWIDTH / 2.0 + RECTPOINTPENWIDTH / 2.0 &&
		event->y() >= (endPoint.y() < lastPoint.y() ? lastPoint.y() : endPoint.y()) - RECTPENWIDTH / 2.0 - RECTPOINTPENHEIGHT / 2.0 &&
		event->y() <= (endPoint.y() < lastPoint.y() ? lastPoint.y() : endPoint.y()) - RECTPENWIDTH / 2.0 + RECTPOINTPENHEIGHT / 2.0;
}

