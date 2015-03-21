#ifndef SHINKEYEVENTFILTER_H
#define SHINKEYEVENTFILTER_H

#include <QKeyEvent>
#include <QObject>

class ShinKeyEventFilter : public QObject {

    Q_OBJECT

    public:
        explicit ShinKeyEventFilter(QObject *parent) :
            QObject(parent) {
        }

    protected:
        bool eventFilter(QObject *object, QEvent *event) {
            if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
                if (keyEvent->modifiers() & Qt::AltModifier) {
                    uint shin[] = { 0x05E9, 0 };
                    if (keyEvent->key() == 0x05E6) {
                        uint sinDot[] = { 0x05C1, 0 };
                        object->event(&QKeyEvent(event->type(), 0x05E9, Qt::NoModifier, QString::fromUcs4(shin)));
                        object->event(&QKeyEvent(event->type(), 0x05C1, Qt::NoModifier, QString::fromUcs4(sinDot)));
                        return true;
                    } else if (keyEvent->key() == 0x05E9) {
                        uint shinDot[] = { 0x05C2, 0 };
                        object->event(&QKeyEvent(event->type(), 0x05E9, Qt::NoModifier, QString::fromUcs4(shin)));
                        object->event(&QKeyEvent(event->type(), 0x05C2, Qt::NoModifier, QString::fromUcs4(shinDot)));
                        return true;
                    }
                }
            }
            return QObject::eventFilter(object, event);
        }
};

#endif // SHINKEYEVENTFILTER_H
