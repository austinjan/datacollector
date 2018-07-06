#ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <QObject>
#include <QPointer>
#include <QQueue>
#include <QTimer>
#include <qevent.h>

class BarcodeReceiveInterface
{
public:
    virtual void barcodeSend(const QByteArray& context) = 0;
};

class KKeyEvent : public QKeyEvent
{
public:
    explicit KKeyEvent(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString & text = QString(), bool autorep = false, ushort count = 1 );

    quint64 stamp;
};

class BarcodeFilterMember
{
public:
    bool filter_in_work;

    class DelayedEvent{
    public:
        explicit DelayedEvent() : obj(0), event(0){}
        QPointer<QObject> obj;
        KKeyEvent *event;

        quint64 getStamp()const { return ((event)?(event->stamp):0); }

        bool operator<(const DelayedEvent &rhs) const{
            return (getStamp() < rhs.getStamp());
        }
    };

    struct _SOE{
        DelayedEvent devent;
        bool resolved;
    } start_of_event;

    QQueue<DelayedEvent> slow_key_events;
    QQueue<DelayedEvent> fast_key_events;
    quint64 last_event_stamp;
    QTimer key_timer;
};

class KeyHandler : public QObject
{
    Q_OBJECT


public:
    explicit KeyHandler(QObject *parent = NULL);

    enum BarcodeMode
    {
        NoDefined,
        Login,
        PlateNCsCode
    };

    void settingMode(BarcodeMode mode) { m_mode = mode; }
    void setBarcodeReceiver(BarcodeReceiveInterface* barcodeReceiver)
    { m_barcodeReceiver = barcodeReceiver; }

protected:
    bool eventFilter(QObject *, QEvent *);
    virtual void timerEvent(QTimerEvent * event);

signals:
    void changeToStartScreen();

public slots:
    void shotEvents();
private:
    bool barcodeFilter(QObject *o, QEvent *e);


    BarcodeFilterMember m_barcode;
    void extractBarcodeData();
    void clearFastKeyEvents();
    void turnOnScreen();
    void turnOffScreen();
    void logout();
    void uploadToFtp();
    void resetTimer();

    BarcodeMode m_mode;
    BarcodeReceiveInterface* m_barcodeReceiver;

    bool m_screenSaverActive;
    int m_screenSaverTimer;
    int m_logoutTimer;
    int m_uploadeTimer;
};

#endif // KEYHANDLER_H
