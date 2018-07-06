#include "ftpservice.h"
#include <QDir>
#include <QDebug>
#include <QtNetwork/QFtp>
#include <QDir>
#include <QMessageBox>
#include "filedataloader.h"
#include "fileoperation.h"

FtpService::FtpService(QObject *parent) : QObject(parent)
{
    m_ftp = NULL;
    m_totalDownloadFiles = 0;
    m_alreadyDownloadFiles = 0;
    m_ftpBusy = false;

    //connectFtp();
//    m_ftp = new QFtp(this);
//    conncetFtpSignals();
}

FtpService::~FtpService()
{
//    if(m_ftp)
//        delete m_ftp;
    if (m_ftp)
    {
        m_ftp->abort();
        m_ftp->deleteLater();
        m_ftp = 0;

    }
}

FtpService* FtpService::s_instance = 0;
FtpService *FtpService::instance()
{
    if(s_instance==0)
    {
        s_instance = new FtpService();
    }
    return s_instance;
}

//QString DOWNLOAD_PATH = QString("download");


void FtpService::test()
{

//    fileFullName(filenam);
//    QFile *file = new QFile(log_file_info.absoluteFilePath());

//    m_ftp->connectToHost("speedtest.tele2.net");
//    m_ftp->login("anonymous");
//    if (file ->open(QIODevice::ReadWrite))
//    {
//       // m_ftp->get("5MB.zip",file);
//        m_ftp->list();
//    }
    //    m_ftp->close();
}

void FtpService::downloadAll()
{
    connectFtp();
    m_ftp->list();


}

void FtpService::downloadFiles()
{
//    QDir download_dir  =  FileOperation::downloadDir();
//    foreach(QString filename, m_downloadFiles)
//    {

//        m_file = new QFile(download_dir.absoluteFilePath(filename));
//        if(m_file->open(QIODevice::ReadWrite))
//        {
//            //m_ftp->put(m_file,log_file_name);
//            m_ftp->get(filename,m_file);

//        }
//        else
//        {
//            delete m_file;
//        }
//    }

//    m_ftp->close();

}

void FtpService::downloadNextFile()
{
    if(m_downloadFiles.isEmpty())
    {
        m_ftp->close();
        m_totalDownloadFiles = 0;
        m_alreadyDownloadFiles = 0;
        emit updateDownloadTask(0,0);
        emit updateProgressInformation(0,0);
        return;
    }
    QString filename = m_downloadFiles.dequeue();
    QDir download_dir  =  FileOperation::downloadDir();


    m_file = new QFile(download_dir.absoluteFilePath(filename));
    if(m_file->open(QIODevice::ReadWrite))
    {
        //m_ftp->put(m_file,log_file_name);
        m_ftpBusy = true;
        m_ftp->get(filename,m_file);

    }
    else
    {
        delete m_file;
        m_file = 0;
    }

}



void FtpService::uploadlogfile()
{

    connectFtp();
    QString log_file_name = FileOperation::logFileName();
    QString log_file_fullname = FileOperation::chekLogFileExist();

    m_file = new QFile(log_file_fullname);
    if(m_file->open(QIODevice::ReadWrite))
    {
        m_ftpBusy = true;
        m_ftp->put(m_file,log_file_name);
    }
    else
    {
        delete m_ftp;
    }

    m_ftp->close();
}

void FtpService::deleteftp()
{
    if (m_ftp)
    {
        m_ftp->abort();
        m_ftp->deleteLater();
        m_ftp = 0;
        return;
    }

    //connectFtp();

}

void FtpService::connectFtp()
{

    if(m_ftp==0)
    {
        m_ftp = new QFtp(this);
        conncetFtpSignals();
    }
    SettingsDataLoader* setting = SettingsDataLoader::instance();
    QString user = setting->getValue("user");
    if(user.isEmpty())
        user = QString("anonymous");

    QString password = setting->getValue("password");
    QString host = setting->getValue("host");
    qDebug()<<"Connecting "<<host<<" with "<<user<< " password: "<<password;
//    QString log_file_name = FileOperation::logFileName();
//    QString log_file_fullname = FileOperation::chekLogFileExist();
    m_ftp->connectToHost(host);
    //m_ftp->login(user,password);
    m_ftp->login();
}

QString FtpService::hostname()
{
    return SettingsDataLoader::instance()->getValue("host");
}



void FtpService::stateChanged(int state)
{
//    int state_p = state;
    emit updateState(state);
}



void FtpService::start(int id)
{
    Q_UNUSED(id);
    if(m_ftp->currentCommand() == QFtp::ConnectToHost)
        qDebug() <<"ftp start to connect server";
    if(m_ftp->currentCommand() == QFtp::Put)
        qDebug() << "ftp command put start" ;
    if(m_ftp->currentCommand() == QFtp::Get)
        qDebug() << "ftp command get start" ;

}

void FtpService::finish(int id, bool error)
{
//    qDebug() << "ftp command finish id="<<id <<"error = "<<error;
    Q_UNUSED(id);
    if (m_ftp->currentCommand() == QFtp::ConnectToHost) {
        if (error) {
            QString _msg = QString("Unable to connect to the FTP server "
                                   " Please check that the host "
                                   "name(%1) is correct.").arg(SettingsDataLoader::instance()->getValue("host"));
            QMessageBox::information(NULL, "FTP",
                                     _msg);
            deleteftp();
            return;
        }
        // success connect
        clearFileResource();
        if(m_ftpBusy)
            m_ftpBusy = false;
        return;
    }

    if (m_ftp->currentCommand() == QFtp::Login)
    {
        //m_ftp->list();
        //login success
        if(error)
        {
            QString err = m_ftp->errorString();
            qDebug() << "ftp login fail" << m_ftp->errorString();
        }
        clearFileResource();
        if(m_ftpBusy)
            m_ftpBusy = false;

    }

    if (m_ftp->currentCommand() == QFtp::Get) {
        if (error)
        {
            qDebug() << "ftp get file error" << m_file->fileName() << m_ftp->errorString();
            clearFileResource();
        }
        else
        {
            clearFileResource();
            downloadNextFile();
            m_alreadyDownloadFiles++;

            emit updateDownloadTask(m_alreadyDownloadFiles,m_totalDownloadFiles);
        }
        if(m_ftpBusy)
            m_ftpBusy = false;

    }
    else if(m_ftp->currentCommand() == QFtp::Put)
    {
        if(error)
        {
            qDebug() << "fup put error!" << m_ftp->errorString();
            QMessageBox::information(NULL, "FTP",
                                     "Put file failed!");

        }

        clearFileResource();
        if(m_ftpBusy)
            m_ftpBusy = false;
        emit putFinished();
    }
    else if (m_ftp->currentCommand() == QFtp::List)
    {
        //list done
        if(error)
        {
            qDebug() << "ftp list error :" << m_ftp->errorString();
        }
        else
        {
            m_totalDownloadFiles = m_downloadFiles.count();
            m_alreadyDownloadFiles = 0;
            emit updateDownloadTask(m_alreadyDownloadFiles,m_totalDownloadFiles);
            downloadNextFile();
        }


    }

}

void FtpService::dataTransferProgress(qint64 done, qint64 total)
{
//    qDebug() << "ftp data taransfer done = "<<done << "total="<<total;
    emit updateProgressInformation(done,total);
}

void FtpService::listInfo(const QUrlInfo &i)
{
    if(i.isFile())
    {
        QString name = i.name();
        m_downloadFiles.enqueue(name);
//        m_downloadFiles << name;
    }
}

//QString FtpService::fileFullName(const QString &filename)
//{
//    QDir download_dir = QDir::current();

//    download_dir.cd(DOWNLOAD_PATH);

//    QFileInfo download_file_info(download_dir,
//                            filename);
//    return download_file_info.absoluteFilePath();
//}

void FtpService::conncetFtpSignals()
{
    connect(m_ftp,SIGNAL(commandStarted(int)),
            this,SLOT(start(int)));
    connect(m_ftp,SIGNAL(commandFinished(int,bool)),
            this,SLOT(finish(int,bool)));
    connect(m_ftp,SIGNAL(dataTransferProgress(qint64,qint64)),
            this,SLOT(dataTransferProgress(qint64,qint64)));
    connect(m_ftp,SIGNAL(listInfo(QUrlInfo)),
            this,SLOT(listInfo(QUrlInfo)));
    connect(m_ftp,SIGNAL(stateChanged(int)),
            this,SLOT(stateChanged(int)));
}

void FtpService::clearFileResource()
{
    if(m_file == 0)
        return;
    m_file->close();
    m_file->remove();
    delete m_file;
    m_file = 0;
}



