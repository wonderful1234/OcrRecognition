#pragma once

#include "PopWidgetBase.h"
#include "MyGlobalShortCut.h"
#include <QMovie>
namespace Ui { class OcrRecognition; }
class OcrRecognition : public PopWidgetBase
{
    Q_OBJECT

public:
    OcrRecognition(QWidget *parent = nullptr);
    ~OcrRecognition();
signals:
    void signGetResult(const QString& result,const QString & fileName);
private:
    Ui::OcrRecognition* ui;
    double m_dpi = 1;
    void getDpi();
    void showScreenShot();
    MyGlobalShortCut* m_shortcut = nullptr;
    QMovie* movie = nullptr;
    void dealPicture(const QString& fileName);
    void setHotKeyLabel();
private slots:
    void hotKeyPressed(int);
};
