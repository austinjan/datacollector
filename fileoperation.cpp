#include "fileoperation.h"

#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>



FileOperation::FileOperation()
{

}

QString DOWNLOAD_PATH = QString("download");

bool FileOperation::initAllSubFolders()
{
    QDir working_dir = QDir::current();


    if(!working_dir.exists(SYSTEMLOG_PATH))
    {
        bool _success = working_dir.mkdir(SYSTEMLOG_PATH);
        if(!_success)
        {
            qDebug() << "bool FileOperation::initAllSubFolders mkdir fail"<<SYSTEMLOG_PATH ;
            return false;
        }
    }

    if(!working_dir.exists(DATA_PATH))
    {
        bool _success = working_dir.mkdir(DATA_PATH);
        if(!_success)
        {
            qDebug()<<__func__<< "mkdir fail" <<DATA_PATH ;
            return false;
        }
    }

    if(!working_dir.exists(IMAGE_PATH))
    {
        bool _success = working_dir.mkdir(IMAGE_PATH);
        if(!_success)
        {
            qDebug() << "bool FileOperation::initAllSubFolders mkdir fail"<<IMAGE_PATH ;
            return false;
        }
    }

    if(!working_dir.exists("debug"))
    {
        bool _success = working_dir.mkdir("debug");
        if(!_success)
        {
            qDebug() << "bool FileOperation::initAllSubFolders mkdir fail"<<"debug" ;
            return false;
        }
    }

    if(!working_dir.exists(SETTING_PATH))
    {
        bool _success = working_dir.mkdir(SETTING_PATH);
        if(!_success)
        {
            qDebug() << "bool FileOperation::initAllSubFolders mkdir fail"<<SETTING_PATH ;
            return false;
        }
    }

    if(!working_dir.exists(DOWNLOAD_PATH))
    {
        bool _success = working_dir.mkdir(DOWNLOAD_PATH);
        if(!_success)
        {
            qDebug() << "bool FileOperation::initAllSubFolders mkdir fail"<<"debug" ;
            return false;
        }
    }

    return true;
}

bool FileOperation::clearSubPath(const QString &path)
{
    Q_UNUSED(path);
    return true;
}


QString FileOperation::makingFullName(const QString &subPath, const QString &filename)
{
    QDir working_dir = QDir::current();
    working_dir.cd(subPath);

    QString full_name =
            working_dir.absoluteFilePath(filename);
    return full_name;

}

QString LOG_PATH = QString("log");
QString FileOperation::logPath()
{
    QDir log_file_dir = QDir::current();


    if(!log_file_dir.exists(LOG_PATH))
    {
        bool _success = log_file_dir.mkdir(LOG_PATH);
        if(!_success)
        {
            qDebug() << "bool BarcodeInputForm::openLogFile mkdir fail" ;
            return QString();
        }

    }

    log_file_dir.cd(LOG_PATH);
    return log_file_dir.absolutePath();
}

QString FileOperation::downloadPath()
{
    QDir download_dir = QDir::current();

    download_dir.cd(DOWNLOAD_PATH);
    return download_dir.absolutePath();
}

QDir FileOperation::downloadDir()
{
    QDir download_dir = QDir::current();

    download_dir.cd(DOWNLOAD_PATH);
    return download_dir;
}

QString FileOperation::chekLogFileExist()
{
    QDir log_file_dir = QDir::current();


    if(!log_file_dir.exists(LOG_PATH))
    {
        bool _success = log_file_dir.mkdir(LOG_PATH);
        if(!_success)
        {
            qDebug() << "bool BarcodeInputForm::openLogFile mkdir fail" ;
            return QString();
        }

    }

    log_file_dir.cd(LOG_PATH);

    QFileInfo log_file_info(log_file_dir,
                            FileOperation::logFileName());

    if (!log_file_info.exists())
    {
//        initializeFile(log_file_info.absoluteFilePath());
        QFile file(log_file_info.absoluteFilePath());
        bool isOpened = file.open(QIODevice::ReadWrite);
        if (!isOpened)
        {
            qDebug() << "void BarcodeInputForm::initializeLogFile() can not create file"
                     << log_file_info.absoluteFilePath();
        }
        file.close();
    }

    QString log_full_name =
            log_file_dir.absoluteFilePath(FileOperation::logFileName());



    QFile file(log_full_name);
    bool isOpened = file.open(QIODevice::ReadWrite);
    if (!isOpened)
    {
        qDebug()<< "BarcodeInputForm::openLogFile can not create file" << log_full_name;
        return QString();
    }
    file.close();

    return log_full_name;
}

QString FileOperation::settingsFileName()
{
    QDir data_dir = QDir::current();


    if(!data_dir.exists(SETTING_PATH))
    {
        bool _success = data_dir.mkdir(SETTING_PATH);
        if(!_success)
        {
            qDebug() << "bool BarcodeInputForm::openLogFile mkdir fail" ;
            return QString();
        }

    }

    data_dir.cd(SETTING_PATH);





    QString settings_full_name =
            data_dir.absoluteFilePath("settings");


    return settings_full_name;
}

QString FileOperation::logFileName()
{
    QDateTime today = QDateTime::currentDateTime();
    QString log_file_name = today.toString("yyyyMMdd");
    return QString("DepStat")+"-"+log_file_name+".csv";
}




