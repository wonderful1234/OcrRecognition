#include "MyWinEventFilter.h"
#include "MyGlobalShortCut.h"
#include <QSettings>
MyWinEventFilter::MyWinEventFilter()
{

}
MyWinEventFilter::~MyWinEventFilter()
{

}

MyWinEventFilter::MyWinEventFilter(MyGlobalShortCut *shortcut)
    : m_shortcut(shortcut)
{

}


bool MyWinEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, long*)
{
    if (eventType == "windows_generic_MSG")
    {
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_HOTKEY)
        {
            const quint32 keycode = HIWORD(msg->lParam);
            const quint32 modifiers = LOWORD(msg->lParam);
            bool res = m_shortcut->shortcuts.value(qMakePair(keycode, modifiers));
            if (res)
            {
                QSettings qSettings(QCoreApplication::applicationDirPath() + "/HotKey.ini", QSettings::IniFormat);
                QString code = qSettings.value("key").toString();
                QKeySequence s;
                Qt::Key key;
                Qt::KeyboardModifiers mods;
               
				QString mn = "ctrl+shift";
				mn += "+";
				mn += code;
                s=QKeySequence(mn);
                Qt::KeyboardModifiers allMods = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
                key= (s.isEmpty() ? Qt::Key(0) : Qt::Key((s[0] ^ allMods) & s[0]));
                mods = s.isEmpty() ? Qt::KeyboardModifiers(0) : Qt::KeyboardModifiers(s[0] & allMods);
                if (MyGlobalShortCut::nativeKeycode(key) == HIWORD(msg->lParam) &&
                    MyGlobalShortCut::nativeModifiers(mods) == LOWORD(msg->lParam)) {
                    m_shortcut->activateShortcut(1);
                	
                }
                return true;
            }
        }
    }
    return false;
}
