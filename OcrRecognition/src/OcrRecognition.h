#pragma once

#include "PopWidgetBase.h"
#include "MyGlobalShortCut.h"
namespace Ui { class OcrRecognition; }
class OcrRecognition : public PopWidgetBase
{
    Q_OBJECT

public:
    OcrRecognition(QWidget *parent = nullptr);
    ~OcrRecognition();

private:
    Ui::OcrRecognition* ui;
    double m_dpi = 1;
    void getDpi();
    void showScreenShot();
    MyGlobalShortCut* m_shortcut = nullptr;
private slots:
    void hotKeyPressed(int);
};
