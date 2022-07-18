#include "OcrRecognition.h"
#include "OcrManage.h"
#include "ui_OcrRecognition.h"
OcrRecognition::OcrRecognition(QWidget *parent)
    : PopWidgetBase(parent), ui(new Ui::OcrRecognition)
{
    ui->setupUi(this);
}

OcrRecognition::~OcrRecognition()
{
    delete ui;
}
