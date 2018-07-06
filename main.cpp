#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QWSServer>
#include <ftpservice.h>
#ifdef Q_OS_LINUX
#include "fcntl.h"
#include "unistd.h"
#include "errno.h"
#endif

#include "inputpanelcontext.h"

void myMessageOutput(QtMsgType type, const char *msg)
{
    QString text;
    QString time = QTime::currentTime().toString();
    QDir logDir = QDir::current();
    logDir.cd("debug");
    QString path = logDir.currentPath();


    switch (type)
    {
    case QtDebugMsg:
        text = QString("Debug[%1]: %2")
                .arg(time)
                .arg(msg);
        break;
    case QtWarningMsg:
        text = QString("Warning[%1]: %2")
                .arg(time)
                .arg(msg);
        break;
    case QtCriticalMsg:
        text = QString("Critical[%1]: %2")
                .arg(time)
                .arg(msg);
        break;
    case QtFatalMsg:
        text = QString("Fatal[%1]: %2")
                .arg(time)
                .arg(msg);
        abort();
    }

    QString fileName = QString("debug") + QDate::currentDate().toString("yyyyMMdd") + QString(".log");
    QFileInfo log_fileinfo(logDir,fileName);
    QFile file(log_fileinfo.absoluteFilePath());
    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream ts(&file);
    ts<<text<<endl;
    file.close();
}


int main(int argc, char *argv[])
{
#if defined(Q_OS_LINUX)
    qputenv("QWS_KEYBOARD","LinuxInput:/dev/input/event2");
    //
#endif

    //qInstallMsgHandler(myMessageOutput);
    QApplication a(argc, argv);

    QApplication::setOrganizationName("ATop");
    QApplication::setOrganizationDomain("atop.com.tw");
    QApplication::setApplicationName("Data Collector");

#if defined(Q_OS_LINUX)
    QDir::setCurrent("/home/dest");
#endif

    InputPanelContext *ic = new InputPanelContext;
    a.setInputContext(ic);
    MainWindow w;
    w.initMainWindow();
    w.initEnvironment();
    w.show();

    FtpService* ftp = FtpService::instance();
    ftp->test();

#if defined(Q_OS_LINUX)
    QWSServer::instance()->openKeyboard();
#endif

    return a.exec();
}
