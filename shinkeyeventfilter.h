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
                        sendShinKey(object, event->type(), 0x05E9, QString::fromUcs4(shin));
                        sendShinKey(object, event->type(), 0x05C1, QString::fromUcs4(sinDot));
                        return true;
                    } else if (keyEvent->key() == 0x05E9) {
                        uint shinDot[] = { 0x05C2, 0 };
                        sendShinKey(object, event->type(), 0x05E9, QString::fromUcs4(shin));
                        sendShinKey(object, event->type(), 0x05C2, QString::fromUcs4(shinDot));
                        return true;
                    }
                }
            }
            return QObject::eventFilter(object, event);
        }

    private:
        void sendShinKey(QObject *object, QEvent::Type eventType, int keyCode, const QString &key) {
            QKeyEvent event(eventType, keyCode, Qt::NoModifier, key);
            object->event(&event);
        }
};

#endif // SHINKEYEVENTFILTER_H
