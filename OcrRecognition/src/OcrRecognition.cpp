#include "OcrRecognition.h"
#include "OcrManage.h"
OcrRecognition::OcrRecognition(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    OcrManage::getInstance()->getTexts("C:\\Users\\wangfude\\Pictures\\0880c08b0110e0868615.png");
}

OcrRecognition::~OcrRecognition()
{}
