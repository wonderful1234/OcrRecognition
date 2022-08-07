#include "ImageViewer.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QToolButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QImageReader>
#include <QScreen>
#include <QMovie>
#include <QApplication>
#include <QDateTime>
ImageViewer::ImageViewer(QWidget *parent)
	: QWidget(parent), labelWidth(0), labelHeight(0), labelX(0), labelY(0)
{
	//setAttribute(Qt::WA_TranslucentBackground, true);
	//setWindowFlags(Qt::FramelessWindowHint);
	//setWindowModality(Qt::ApplicationModal);//设置窗体模态，要求该窗体没有父类，否则无效
	//setAttribute(Qt::WA_DeleteOnClose);
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	backgroundWidget = new QWidget(this);
	lblImage = new QLabel(this);
	btnEnlarge = new QToolButton(this);
	btnNarrow = new QToolButton(this);
	btnRotate = new QToolButton(this);
	btnEnlarge->setFocusPolicy(Qt::NoFocus);
	btnNarrow->setFocusPolicy(Qt::NoFocus);
	btnRotate->setFocusPolicy(Qt::NoFocus);

	mainLayout->setContentsMargins(0, 0, 0, 0);
	backgroundWidget->setObjectName("backgroundWidget");
	backgroundWidget->setStyleSheet("#backgroundWidget{background-color: rgba(136, 136, 136, 100);}");
	mainLayout->addWidget(backgroundWidget);

	setCursor(Qt::SizeAllCursor);
	btnEnlarge->setCursor(Qt::PointingHandCursor);
	btnNarrow->setCursor(Qt::PointingHandCursor);
	btnRotate->setCursor(Qt::PointingHandCursor);

	btnEnlarge->setToolTip((u8"放大"));
	btnNarrow->setToolTip((u8"缩小"));
	btnRotate->setToolTip((u8"旋转"));
	btnEnlarge->setText((u8"放大"));
	btnNarrow->setText((u8"缩小"));
	btnRotate->setText((u8"旋转"));

	QString style = "QToolButton{border:0px;color:#b2bcc5;padding-top:25px;background-repeat:no-repeat;\
					background-image:url(:/OcrRecognition/res/%1.png);background-position:top center;}\
					QToolButton:hover{background-image: url(:/OcrRecognition/res/%1_hit.png);color:#ffffff;}";
	btnEnlarge->setStyleSheet(style.arg("amplification"));
	btnNarrow->setStyleSheet(style.arg("narrow"));
	btnRotate->setStyleSheet(style.arg("rotating"));

	btnEnlarge->setIconSize(QSize(27, 29));
	btnNarrow->setIconSize(QSize(27, 29));
	btnRotate->setIconSize(QSize(27, 29));
	btnRotate->setFixedSize(40, 50);
	btnEnlarge->setFixedSize(40, 50);
	btnNarrow->setFixedSize(40, 50);

	toolbox = new QWidget(this);
	QHBoxLayout* toolboxLayout = new QHBoxLayout();
	toolbox->setObjectName("toolbox");
	toolbox->setStyleSheet("#toolbox{background-color: rgba(0, 0, 0, 175);border-radius:5px;}");
	toolbox->setLayout(toolboxLayout);
	toolbox->setFixedSize(180, 55);
	toolboxLayout->setContentsMargins(10, 5, 10, 0);
	toolboxLayout->addWidget(btnEnlarge);
	toolboxLayout->addWidget(btnNarrow);
	toolboxLayout->addWidget(btnRotate);

	toolbox->setGeometry((this->width() - toolbox->width()) / 2, this->height() - toolbox->height() - 10, toolbox->width(), toolbox->height());
	connect(btnEnlarge, &QToolButton::clicked, this, &ImageViewer::enlargePix);
	connect(btnNarrow, &QToolButton::clicked, this, &ImageViewer::narrowPix);
	connect(btnRotate, &QToolButton::clicked, this, &ImageViewer::rotatePix);
}

ImageViewer::ImageViewer(QWidget *p, const QString& file, bool hide) :ImageViewer(p)
{
	showPic(file, hide);
}

ImageViewer::~ImageViewer()
{
}

void ImageViewer::showPic(const QString& file, bool hide)
{
	m_filePath = file;
	if (m_gif_movie)
	{
		m_gif_movie->stop();
		m_gif_movie->deleteLater();
		m_gif_movie = Q_NULLPTR;
	}
	QImageReader reader(file);
	reader.setDecideFormatFromContent(true);
	// 要放在read()之前
	QByteArray m_image_format = reader.format();
	if (m_image_format == QByteArray("gif"))
	{
		isGif = true;
		m_gif_movie = new QMovie(file);
		connect(m_gif_movie, &QMovie::finished, this, [&]() {
			m_gif_movie->start();
		});
		QImage image;
		image.load(file);
		if (image.isNull())
		{
			QImageReader reader(file);
			reader.setDecideFormatFromContent(true);
			image = reader.read();
		}
		labelX = (this->width() - image.width()) / 2;
		labelY = (this->height() - image.height()) / 2;
		labelWidth = image.width();
		labelHeight = image.height();
		gifSize = QSize(labelWidth, labelHeight);
		lblImage->setGeometry(labelX, labelY, labelWidth, labelHeight);
		lblImage->setMovie(m_gif_movie);
		m_gif_movie->start();
	}
	else
	{
		isGif = false;
		QImage image;
		image.load(file);
		if (image.isNull())
		{
			QImageReader reader(file);
			reader.setDecideFormatFromContent(true);
			image = reader.read();
		}
		setPixmap(QPixmap::fromImage(image));
	}
	if (hide)
	{
		
	}
	setGeometry(getMainWindowRect());
	resizeEvent(NULL);
}

QRect ImageViewer::getMainWindowRect()
{
	auto screen = QGuiApplication::primaryScreen();

	if (!screen->logicalDotsPerInch())
	{
		int desktop_width = QApplication::desktop()->width();
		int desktop_high = QApplication::desktop()->height();
		return QRect(desktop_width / 8, desktop_high / 8, desktop_width * 3 / 4, desktop_high * 3 / 4);
	}
	qreal ratio = screen->logicalDotsPerInch() / 96;
	auto widgets = QApplication::topLevelWidgets();
	QWidget *mainwindow = 0;

	for (auto widget : widgets)
	{
		if (widget->inherits("QMainWindow"))
		{
			mainwindow = widget;
			break;
		}
	}
	int margin = 1;
	if (mainwindow)
	{
		if (mainwindow->isMaximized())
			return QRect(mainwindow->x(), mainwindow->y(), mainwindow->width(), mainwindow->height());
		else
			return QRect(mainwindow->x() + margin, mainwindow->y() + margin, mainwindow->width() - margin * 2, mainwindow->height() - margin * 2);
	}
	else
	{
		int desktop_width = QApplication::desktop()->width();
		int desktop_high = QApplication::desktop()->height();
		return QRect(desktop_width / 8, desktop_high / 8, desktop_width * 3 / 4, desktop_high * 3 / 4);
	}
}

void ImageViewer::setPixmap(const QPixmap &pix)
{
	if (m_gif_movie)
	{
		m_gif_movie->stop();
		m_gif_movie->deleteLater();
		m_gif_movie = Q_NULLPTR;
	}
	this->pix = pix;
	if (pix.isNull())
	{
		lblImage->setPixmap(pix);
		return;
	}
	
	double ratio = 1.0 * this->height() / pix.height();
	double ratio2 = 1.0 * this->width() / pix.width();
	if (ratio2 < ratio)
	{
		ratio = ratio2;
	}
	labelHeight = pix.height() * ratio;
	labelWidth = pix.width() * ratio;
	labelX = (this->width() - labelWidth) / 2;
	labelY = (this->height() - labelHeight) / 2;
	defaultEnlargePix();
}


void ImageViewer::enlargePix()
{
	labelWidth = labelWidth * 1.1;
	labelHeight = labelHeight * 1.1;
	labelX = labelX - (0.5*this->width() - labelX)*0.1;
	labelY = labelY - (0.5*this->height() - labelY)*0.1;
	if (isGif)
		calculateGif();
	else
		calculatePix();
}

void ImageViewer::defaultEnlargePix()
{
	calculatePix();
}

void ImageViewer::narrowPix()
{
	labelWidth = labelWidth * 0.9;
	labelHeight = labelHeight * 0.9;
	labelX = labelX + (0.5*this->width() - labelX)*0.1;
	labelY = labelY + (0.5*this->height() - labelY)*0.1;
	if (isGif)
		calculateGif();
	else
		calculatePix();
}

void ImageViewer::rotatePix()
{
	if (isGif)
		return;

	double oldX = labelX;
	double oldY = labelY;
	labelX = -oldY + this->width() / 2 + this->height() / 2 - labelHeight;
	labelY = oldX - this->width() / 2 + this->height() / 2;

	double tmp = labelWidth;
	labelWidth = labelHeight;
	labelHeight = tmp;

	QTransform rm;
	rm.rotate(90);
	pix = pix.transformed(rm);
	if (isGif)
		calculateGif();
	else
		calculatePix();
}

void ImageViewer::calculatePix()
{
	double ratio = labelHeight / pix.height();
	QRect pixRect(labelX, labelY, pix.width()*ratio, pix.height()*ratio);
	QRect windowRect(0, 0, this->width(), this->height());
	QRect intersectedRect = pixRect.intersected(windowRect);
	QRect intersectedRect2 = QRect((intersectedRect.x() - labelX) / ratio, (intersectedRect.y() - labelY) / ratio,
		intersectedRect.width() / ratio, intersectedRect.height() / ratio);
	QPixmap newPix = pix.copy(intersectedRect2);
	newPix = newPix.scaled(intersectedRect.width(), intersectedRect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	lblImage->resize(intersectedRect.size());
	lblImage->setPixmap(newPix);
	lblImage->setGeometry(intersectedRect);
}

void ImageViewer::calculateGif()
{
	gifSize = QSize(labelWidth, labelHeight);
	lblImage->setGeometry(labelX, labelY, labelWidth, labelHeight);
	m_gif_movie->setScaledSize(gifSize);
}

void ImageViewer::mousePressEvent(QMouseEvent * event)
{
	xMouse = event->x();
	yMouse = event->y();
}

void ImageViewer::mouseMoveEvent(QMouseEvent* event)
{
	int oldX = xMouse;
	int oldY = yMouse;
	xMouse = event->x();
	yMouse = event->y();
	labelX = labelX + xMouse - oldX;
	labelY = labelY + yMouse - oldY;
	if (isGif)
		calculateGif();
	else
		calculatePix();
}

void ImageViewer::wheelEvent(QWheelEvent* event)
{
	if (event->angleDelta().y() > 0)
	{
		enlargePix();
	}
	else if (event->angleDelta().y() < 0)
	{
		narrowPix();
	}
}

void ImageViewer::resizeEvent(QResizeEvent* event)
{
	if (isGif)
	{
		labelX = (this->width() - gifSize.width()) / 2;
		labelY = (this->height() - gifSize.height()) / 2;
		labelWidth = gifSize.width();
		labelHeight = gifSize.height();
	}
	else
	{
		labelWidth = pix.width();
		labelHeight = pix.height();
		if (labelHeight > this->height() || labelWidth > this->width())
		{
			double ratio = 1.0*labelHeight / this->height();
			if (labelWidth / ratio > this->width())
			{
				ratio = 1.0*labelWidth / this->width();
			}
			labelWidth = labelWidth / ratio;
			labelHeight = labelHeight / ratio;
			QPixmap newPix = pix.scaled(labelWidth, labelHeight,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
			lblImage->resize(labelWidth, labelHeight);
			lblImage->setPixmap(newPix);
		}
		labelX = (this->width() - labelWidth) / 2;
		labelY = (this->height() - labelHeight) / 2;
	}

	lblImage->setGeometry(labelX, labelY, labelWidth, labelHeight);
	toolbox->setGeometry((this->width() - toolbox->width()) / 2, this->height() - toolbox->height() - 10, toolbox->width(), toolbox->height());
	defaultEnlargePix();
}

void ImageViewer::keyPressEvent(QKeyEvent* event)
{
	auto widget = QApplication::focusWidget();
	switch (event->key())
	{
	case Qt::Key_Escape:
		this->close();
		break;
	case Qt::Key_Left:
		
		break;
	case Qt::Key_Right:
		
		break;
	default:
		break;
	}
}
