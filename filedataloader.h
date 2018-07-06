#ifndef LOGINDATALOADER_H
#define LOGINDATALOADER_H

/*************************************************************
 * Provide all file access functions.
 *
 * Load product picture, load login account etc...
 * Write log.
 ************************************************************/
#include <QObject>

#define PIC_DETAIL_WIDTH    300
#define PIC_DETAIL_HEIGHT   300
class PictureDataloader
{
public:
    static PictureDataloader* instance();
    QString picExist(const QString& filename);
private:
  PictureDataloader(){}
  static PictureDataloader* s_instance;
};

class DepartmentDataLoader
{
public:

  static DepartmentDataLoader* instance();
  QString departmentFileName();
  QStringList getAllDepartmentName();

private:
  DepartmentDataLoader();
  static DepartmentDataLoader* s_instance;
};

class LoginDataLoader
{
public:
    LoginDataLoader();
    static LoginDataLoader* instance();
    QString loginFileName();
    void test();
    bool LoginMatch(const QString& account, const QString& password);

    QStringList getAllEmployeesName();
private:
    static LoginDataLoader* s_instance;
};

///
/// \brief The LogFileOprator class
/// Provide function of Log. write log, read log or modify (delete log)
class LogFileOprator
{
public:
    enum LogState
    {
        Duplicate,
        Success,
        WriteFailed
    };

    enum LogColumn
    {
        DepartmentColumn,
        CsCodeColumn,
        PlateNumberColumn,
        QuantityColumn,
        DateTimeColumn,
        MachineIDColumn,
        OperatorColumn
    };

    static LogFileOprator* instance();

    LogState log(const QString& logfilename,const QString& qty,
             const QString &cscode, const QString &plateNumber);

    QStringList readLast5Log();

    QString getDataFromLine(const QString& line, LogColumn column);
    bool checkDuplicate(const QString &loginFileName, const QString& cscode, const QString &plate);

private:
    LogFileOprator();
    static LogFileOprator* s_instance;
};

class SettingsDataLoader
{
public:
    SettingsDataLoader();
    static SettingsDataLoader* instance();
    QString getValue(const QString& key);
    void setValue(const QString& key, const QString& value);

private:
    static SettingsDataLoader* s_instance;
};

#endif // LOGINDATALOADER_H
