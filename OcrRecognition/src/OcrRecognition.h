#pragma once

#include "PopWidgetBase.h"
namespace Ui { class OcrRecognition; }
class OcrRecognition : public PopWidgetBase
{
    Q_OBJECT

public:
    OcrRecognition(QWidget *parent = nullptr);
    ~OcrRecognition();

private:
    Ui::OcrRecognition* ui;
};
