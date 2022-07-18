#include "OcrRecognition.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OcrRecognition w;
    w.show();
    return a.exec();
}
