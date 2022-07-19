#ifndef FREESNAPDIALOG_H
#define FREESNAPDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>

#define GRAYCOLOR grayColor
#define RECTCOLOR rectColor
#define RECTPOINTCOLOR rectPointColor
#define RECTPENWIDTH 2
#define RECTPOINTPENWIDTH 6
#define RECTPOINTPENHEIGHT 6

namespace Ui {
	class FreeSnapDialog;
}

class FreeSnapDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FreeSnapDialog(QPixmap picture, QPixmap*& result, QWidget *parent = nullptr,double dpi=1);
	~FreeSnapDialog();

	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	bool inLeftTopPoint(QMouseEvent *event);
	bool inRightTopPoint(QMouseEvent *event);
	bool inLeftBottomPoint(QMouseEvent *event);
	bool inRightBottomPoint(QMouseEvent *event);

private:
	Ui::FreeSnapDialog *ui;

	QPixmap picture;

	QImage image;        //¥Ê¥¢QImage¿‡–Õµƒpicture

	QPixmap*& resultRef;

	QPixmap pictureMap;

	QPixmap pictureMap2;

	QPoint lastPoint;

	QPoint endPoint;

	bool isDrawing;

	bool noOperate;

	QRect resultRect;

	float zoomRate;

	static QColor rectColor;

	static QColor grayColor;

	static QColor rectPointColor;
	double m_dpi;
};

#endif // FREESNAPDIALOG_H
