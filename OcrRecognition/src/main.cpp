#include "OcrRecognition.h"
#include <QtWidgets/QApplication>
#include "OcrManage.h"
int main(int argc, char *argv[])
{
	QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    OcrManage::getInstance();
    OcrRecognition w;
    w.show();
    return a.exec();
    OcrManage::getInstance().destoryOcr();
}
