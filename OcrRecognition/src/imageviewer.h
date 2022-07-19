#pragma once

#include <QWidget>
#include <QPixmap>
#include <QLabel>
class QToolButton;
class  ImageViewer : public QWidget
{
	Q_OBJECT
public:
	ImageViewer(QWidget *parent = Q_NULLPTR);
	ImageViewer(QWidget *parent ,const QString& file,bool hide = true);
	~ImageViewer();
	void setPixmap(const QPixmap &pix );
	static QRect getMainWindowRect();
	void setLeftArrowEnable(bool);
	void setRightArrowEnable(bool);
	void showPic(const QString& s, bool hide = true);
	void defaultEnlargePix();
protected:
	void calculatePix();
	void calculateGif();
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
private slots:
	void enlargePix();
	void narrowPix();
	void rotatePix();
	void savePicture();
signals:
	void sigLastOne();
	void sigNextOne();
private:
	QPixmap pix;
	int labelX, labelY;
	int xMouse, yMouse;
	double labelWidth,labelHeight;
	QToolButton /**btnClose,*/ *btnRotate, *btnEnlarge, *btnNarrow/*, *btnSaveAs*/;
	QToolButton *btnBack, *btnNext/*,*btnInfo*/;
	QWidget* toolbox;
	QWidget* backgroundWidget;
	QLabel* lblImage;
	QMovie*	m_gif_movie = Q_NULLPTR;
	QSize gifSize;
	bool isGif = false;
	QString m_filePath;
};
