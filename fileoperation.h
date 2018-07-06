#ifndef FILEOPERATION_H
#define FILEOPERATION_H
#include <QString>


#define SYSTEMLOG_PATH "log"
#define DATA_PATH "data"
#define IMAGE_PATH "image"
#define SETTING_PATH "settings"

class QDir;
class FileOperation
{
public:
    FileOperation();
    // information
    static QString makingFullName(const QString& subPath, const QString& filename);
    static QString chekLogFileExist();
    static QString settingsFileName();
    static QString logFileName();
    static QString logPath();
    static QString downloadPath();
    static QDir downloadDir();

    // operation
    static bool initAllSubFolders();
    static bool clearSubPath(const QString& path);
};

#endif // FILEOPERATION_H
