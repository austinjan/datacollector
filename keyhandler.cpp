#include "keyhandler.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDateTime>

#include "ftpservice.h"
#include "hardwareinterface.h"

static const int SEQ_TIME_VAL = 60;

#include "hardwareinterface.h"

KKeyEvent::KKeyEvent(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text, bool autorep, ushort count)
    : QKeyEvent(type, key, modifiers, text, autorep, count),
      stamp(0)
{
    //
}

int SCREENSAVER_TIME_LIMIT= 600; // test > 10 min
int LOGOUT_TIMER_LIMIT= 1200; //test > 20 min
int UPLOAD_TIMER_LIMIT = 1200;

KeyHandler::KeyHandler(QObject *parent) : QObject(parent)
{

    m_mode = NoDefined;
    m_barcode.filter_in_work = true;

    m_barcode.start_of_event.devent.event = 0;
    m_barcode.start_of_event.devent.obj = 0;
    m_barcode.start_of_event.resolved = false;
    m_barcode.last_event_stamp = 0;
    m_barcodeReceiver = NULL;

    m_barcode.key_timer.setSingleShot(true);
    m_barcode.key_timer.setInterval(SEQ_TIME_VAL);
    connect(&m_barcode.key_timer, SIGNAL(timeout()), this, SLOT(shotEvents()));
    qApp->installEventFilter(this);

    m_uploadeTimer = 0;
    m_screenSaverTimer = 0;
    m_screenSaverActive = false;
    startTimer(1000);

}

bool KeyHandler::barcodeFilter(QObject *o, QEvent *e)
{
    qDebug() << __func__;
    QKeyEvent *ev = static_cast<QKeyEvent *>(e);
    quint64 current_key_stamp = QDateTime::currentMSecsSinceEpoch();
    m_barcode.last_event_stamp = current_key_stamp;

    //1. duplicate event
    BarcodeFilterMember::DelayedEvent dev;
    dev.event = new KKeyEvent(ev->type(), ev->key(), ev->modifiers(), ev->text(), ev->isAutoRepeat(), ev->count());
    dev.event->stamp = current_key_stamp;
    dev.obj = o;

    //2. check
    if(m_barcode.start_of_event.devent.event)
    {
        if(!m_barcode.start_of_event.resolved)
        {
            qDebug() << "enqueue keyboard event";
            m_barcode.fast_key_events.enqueue(m_barcode.start_of_event.devent);
            m_barcode.start_of_event.resolved = true;
        }

        m_barcode.fast_key_events.enqueue(dev);
        if(Qt::Key_Return == m_barcode.fast_key_events.last().event->key())
            shotEvents();
    } else
    {
        qDebug() << "Setting start keyboard event";
        m_barcode.start_of_event.devent = dev;
        m_barcode.start_of_event.resolved = false;
    }

    //m_barcode.key_timer.start();
    return true;
}

void KeyHandler::shotEvents()
{
    qDebug() << __func__;
    m_barcode.filter_in_work = false;
    if(m_barcode.start_of_event.devent.event && !m_barcode.start_of_event.resolved){
        m_barcode.start_of_event.resolved = true;
    }

    if(!m_barcode.fast_key_events.isEmpty()){
        if(Qt::Key_Return == m_barcode.fast_key_events.last().event->key())
        {
            qDebug() << __func__ << "setp2";
            extractBarcodeData();
        }
        else
        {
            qDebug()<<"last key"<<m_barcode.fast_key_events.last().event->text();
        }
    }

    clearFastKeyEvents();
    m_barcode.start_of_event.devent.event = 0;
//    qDebug()<<__func__<<"clear barcode event";
    m_barcode.start_of_event.devent.obj = 0;
    m_barcode.start_of_event.resolved = false;
    m_barcode.filter_in_work = true;
}

qint64 s_last_time = 0;

bool KeyHandler::eventFilter(QObject *o, QEvent *e)
{
    QEvent::Type type = e->type();

    if(type == QEvent::MouseButtonPress)
    {
        resetTimer();
        qint64 _current_time =QDateTime::currentMSecsSinceEpoch();
        if( (_current_time-s_last_time) > 200)
            HardwareInterface::makeSound();
        s_last_time = _current_time;
        if(m_screenSaverActive)
        {
            turnOnScreen();

            return true;

        }
        else
        {
            return false;
        }
    }

    if(type == QEvent::KeyPress || type == QEvent::KeyRelease) {

        qDebug() << "KeyHandler::eventFilter ----- 1";
        bool b = barcodeFilter(o, e);
        QKeyEvent *ke = static_cast<QKeyEvent*>(e);
        qDebug() <<">>>" <<ke->text();
        return b;

    }

    return false;
}

void KeyHandler::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    m_uploadeTimer++;
    m_screenSaverTimer++;
    m_logoutTimer++;
    if(m_screenSaverTimer >= SCREENSAVER_TIME_LIMIT)
    {
        turnOffScreen();

    }
    if(m_logoutTimer >= LOGOUT_TIMER_LIMIT)
    {
        logout();
    }
    if(m_uploadeTimer >= UPLOAD_TIMER_LIMIT )
    {
        uploadToFtp();
    }
}

void KeyHandler::clearFastKeyEvents()
{
    while(!m_barcode.fast_key_events.isEmpty()){
        BarcodeFilterMember::DelayedEvent delayed_event = m_barcode.fast_key_events.dequeue();
        KKeyEvent *ev = delayed_event.event;
        delete ev;
    }
}

void KeyHandler::turnOnScreen()
{
    HardwareInterface::setBacklight(20);
    m_screenSaverActive =false;
}

void KeyHandler::turnOffScreen()
{
    HardwareInterface::setBacklight(0);
    m_screenSaverActive =true;
}

#include "information.h"
void KeyHandler::logout()
{
    Information *info = Information::instance();
    info->setLonginName(QString());
    info->setDepartmentName(QString());
    emit changeToStartScreen();
}

void KeyHandler::uploadToFtp()
{
    FtpService* ftp= FtpService::instance();
    if(!ftp->isFtpBusy())
    {
        qDebug() << "auto upload file";
        ftp->uploadlogfile();
        m_uploadeTimer = 0;
    }
    else
    {
        //retry 5mins later
        m_uploadeTimer -= 300;
    }
}

void KeyHandler::resetTimer()
{
    m_screenSaverTimer = 0;
    m_logoutTimer = 0;
}

void KeyHandler::extractBarcodeData()
{
    QByteArray ba;

    while(!m_barcode.fast_key_events.isEmpty()){
        BarcodeFilterMember::DelayedEvent delayed_event = m_barcode.fast_key_events.dequeue();
        KKeyEvent *ev = delayed_event.event;

        if(ev->type() == QEvent::KeyPress){
            ba.append( ev->text() );
        }

        delete ev;
    }
    //SerialService::getInstance()->setDirectInput(ba);

    if(m_barcodeReceiver)
    {
        qDebug() << __func__ << ba;
        m_barcodeReceiver->barcodeSend(ba);
    }
}
