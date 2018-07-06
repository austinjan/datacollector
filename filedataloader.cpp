#include "filedataloader.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QSettings>

#include "fileoperation.h"
#include "information.h"
#include "QDateTime"

#ifdef Q_OS_LINUX
#include "fcntl.h"
#include "unistd.h"
#include "errno.h"
#endif

DepartmentDataLoader::DepartmentDataLoader()
{

}

DepartmentDataLoader* DepartmentDataLoader::s_instance = 0;
DepartmentDataLoader *DepartmentDataLoader::instance()
{
    if (!s_instance)
    {
        s_instance = new DepartmentDataLoader();
    }
    return s_instance;
}

QString DEPARTMENT_FILE_FILTER = QString("department-*");

QString DepartmentDataLoader::departmentFileName()
{
    QDir log_file_dir = QDir::current();
    log_file_dir.cd(DATA_PATH);
    QStringList name_filter(DEPARTMENT_FILE_FILTER);
    QStringList exist_files = log_file_dir.entryList(name_filter);
    //qDebug() << exist_files;

    if (exist_files.size() <= 0)
    {
        qDebug( ) << "DepartmentDataLoader::departmentFileName() not found in " <<  log_file_dir.absolutePath();
        return QString();
    }

    qDebug()<<"String DepartmentDataLoader::departmentFileName()" << log_file_dir.absoluteFilePath(exist_files.value(0,QString()));
    return log_file_dir.absoluteFilePath(exist_files.value(0,QString()));
}

QStringList DepartmentDataLoader::getAllDepartmentName()
{
    QStringList names;
    QString file_name = departmentFileName();

    if(file_name.isEmpty())
    {
        qDebug() << __FUNCTION__ << file_name << "not found!";
        return names;
    }

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << __FUNCTION__ <<file.errorString();
        return names;
    }

       //QStringList wordList;
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString name = QString(line);
        name.remove(QRegExp("[\\n\\r]"));
        names << name;
    }

    return names;
}


LoginDataLoader::LoginDataLoader()
{

}

LoginDataLoader* LoginDataLoader::s_instance = 0;
LoginDataLoader *LoginDataLoader::instance()
{
    if (!s_instance)
    {
        s_instance = new LoginDataLoader();
    }
    return s_instance;
}

QString LOGIN_FILE_FILTER = QString("loginlist.csv");

QString LoginDataLoader::loginFileName()
{
    QDir log_file_dir = QDir::current();
    log_file_dir.cd(DATA_PATH);
    QStringList name_filter(LOGIN_FILE_FILTER);
    QStringList exist_files = log_file_dir.entryList(name_filter);
    //qDebug() << exist_files;

    if (exist_files.size() <= 0)
    {
        qDebug( ) << "LoginDataLoader::loginFileName() " <<  log_file_dir.absolutePath();
        return QString();
    }

    qDebug() << "LoginDataLoader::loginFileName()" << log_file_dir.absoluteFilePath(exist_files.value(0,QString()));

    return log_file_dir.absoluteFilePath(exist_files.value(0,QString()));
}

void LoginDataLoader::test()
{
    loginFileName();
    bool ret = LoginMatch("TN1EA1263-3","AUG");
    qDebug() << "LoginDataLoader::test() loinMatch return" << ret;
}

QStringList LoginDataLoader::getAllEmployeesName()
{
    QStringList names;
    QString file_name = loginFileName();

    if(file_name.isEmpty())
    {
        qDebug() << __FUNCTION__ << file_name << "not found!";
        return names;
    }

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << __FUNCTION__ <<file.errorString();
        return names;
    }

       //QStringList wordList;
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString name = line.split(',').first();
        names << name;
    }

    return names;

}

bool LoginDataLoader::LoginMatch(const QString &account, const QString &password)
{
    QString file_name = loginFileName();

    if(file_name.isEmpty())
        return false;

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
        return 1;
    }

       //QStringList wordList;
       while (!file.atEnd())
       {
           QByteArray line = file.readLine();
           QString read_account = line.split(',').first().trimmed();
           QString read_password = line.split(',').value(1,QByteArray()).trimmed();
           if(account == read_account)
           {
               if (read_password == password)
                   return true;
           }
       }

       //qDebug() << wordList;

       return false;
}

PictureDataloader* PictureDataloader::s_instance = 0;
PictureDataloader *PictureDataloader::instance()
{
    if (!s_instance)
    {
        s_instance = new PictureDataloader();
    }
    return s_instance;
}

QString PictureDataloader::picExist(const QString &filename)
{
    QString pic_file_name = filename + ".jpg";
    QDir log_file_dir = QDir::current();
    log_file_dir.cd(IMAGE_PATH);
    QStringList name_filter(pic_file_name);
    QStringList exist_files = log_file_dir.entryList(name_filter);
    //qDebug() << exist_files;

    if (exist_files.size() <= 0)
    {
        return QString();
    }

    return log_file_dir.absoluteFilePath(exist_files.value(0,QString()));
}


LogFileOprator* LogFileOprator::s_instance = 0;
LogFileOprator *LogFileOprator::instance()
{
    if (!s_instance)
    {
        s_instance = new LogFileOprator();
    }
    return s_instance;
}

// Dep/ cscode+plate / qty / datetime / scannerId / login_name
LogFileOprator::LogState LogFileOprator::log(const QString& logfilename,
                         const QString &qty,
                         const QString& cscode,
                         const QString& plateNumber)
{

    Information* info = Information::instance();
    QString department = info->department();
    QString employee = info->employee();
    QString time =
            QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    QString id = cscode + "-" +plateNumber;
    bool isDuplicate = checkDuplicate(logfilename,cscode, plateNumber);
    if(isDuplicate)
    {

        return Duplicate;
    }

    QString scannerID = SettingsDataLoader::instance()->getValue("scanerID");

    QString input_line = QString("%1,%2,%3,%4,%5,%6,%7 \n")
            .arg(department)
            .arg(cscode)
            .arg(plateNumber)
            .arg(qty)
            .arg(time)
            .arg(scannerID)
            .arg(employee);

    QFile file(logfilename);
    if(file.open(QFile::Append | QFile::Text))
    {
        QTextStream out(&file);
        // for utf-8
        out.setCodec("UTF-8");
        //out.setGenerateByteOrderMark(true);
        out << input_line;
        file.close();
#ifdef Q_OS_LINUX
    sync();
#endif
        return Success;
    }
    else
    {
        return WriteFailed;
    }


}

QStringList LogFileOprator::readLast5Log()
{
    QString logfilename = FileOperation::chekLogFileExist();
    QFile file(logfilename);
    QStringList return_datas;
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        qint64 _pos =file.size();
        int _line_count = 0;
        //qint64 file_size = file.size();
        file.seek(_pos);
        while(_pos)
        {
            char _c;
            file.seek(_pos);
            file.getChar(&_c);
            if(_c=='\n')
            {
                _line_count++;

            }
            _pos--;
            if (_line_count > 5)
                break;
        }

        // case1 total line <= 5
        if(_line_count <=5)
        {
            file.seek(0);
            while(!file.atEnd() )
            {
                QString _readline = (QString)file.readLine();
                if(!_readline.trimmed().isEmpty())
                    return_datas << _readline.trimmed();
            }
        }
        else
        {
            // case2 total line > 5
            file.seek(_pos);
            while(!file.atEnd() )
            {
                QString _readline = (QString)file.readLine();
                if(!_readline.trimmed().isEmpty())
                    return_datas << _readline.trimmed();
            }

        }

    }

    return return_datas;


}

QString LogFileOprator::getDataFromLine(const QString &line, LogFileOprator::LogColumn column)
{
    QStringList split_string = line.split(",");
    return split_string.value((int)column,QString());
}

// return: true mean id duplicate! do not write this record.
bool LogFileOprator::checkDuplicate(const QString &loginFileName, const QString &cscode, const QString& plate)
{
    QString file_name = loginFileName;

    if(file_name.isEmpty())
        return true;

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.errorString();
        return true;
    }

       //QStringList wordList;
       while (!file.atEnd())
       {
           QByteArray line = file.readLine();
           //QString read_account = line.split(',').first();
           QString _cscode = line.split(',').value(1,QByteArray());
           QString _plateNo = line.split(',').value(2,QByteArray());
           if((_cscode == cscode) && (_plateNo == plate))
           {
               if(plate.isEmpty())
                   return false;
               else
                   return true;
           }
       }

       //qDebug() << wordList;

       return false;
}


LogFileOprator::LogFileOprator()
{

}

SettingsDataLoader* SettingsDataLoader::s_instance = 0;

SettingsDataLoader::SettingsDataLoader()
{

}

SettingsDataLoader *SettingsDataLoader::instance()
{
    if (!s_instance)
    {
        s_instance = new SettingsDataLoader();
    }
    return s_instance;
}

QString SettingsDataLoader::getValue(const QString &key)
{
    QString settings_file_name = FileOperation::settingsFileName();
    QSettings settings(settings_file_name,QSettings::IniFormat);
    return settings.value(key).toString();
}

void SettingsDataLoader::setValue(const QString &key, const QString &value)
{
    QString settings_file_name = FileOperation::settingsFileName();
    QSettings settings(settings_file_name,QSettings::IniFormat);
    qDebug() <<settings.fileName();
    settings.setValue(key,value);
}


