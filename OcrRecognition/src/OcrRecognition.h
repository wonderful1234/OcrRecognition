#pragma once

#include <QtWidgets/QWidget>
#include "ui_OcrRecognition.h"

class OcrRecognition : public QWidget
{
    Q_OBJECT

public:
    OcrRecognition(QWidget *parent = nullptr);
    ~OcrRecognition();

private:
    Ui::OcrRecognitionClass ui;
};
