#ifndef FTPSERVICE_H
#define FTPSERVICE_H

#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QTime>
#include <QUrlInfo>
#include <QFile>
#include <QQueue>

class QFtp;

class FtpService : public QObject
{
    Q_OBJECT
public:
    explicit FtpService(QObject *parent = NULL);
    ~FtpService();

    static FtpService* instance();
    void test();
    void downloadAll();
    void downloadFiles();
    void downloadNextFile();
    void uploadlogfile();
    void deleteftp();
    void connectFtp();

    // informatiosn
    QString hostname();
    int totalDownloadFiles() { return m_totalDownloadFiles; }
    int alreadyDownloadFiles() { return m_alreadyDownloadFiles; }

signals:
    void updateDownloadTask(int already, int total);
    void updateState(int state);
    void updateProgressInformation(int current, int total);
    void putFinished();


public slots:

//    void	done(bool error)

//    void	rawCommandReply(int replyCode, const QString & detail)
//    void	readyRead()
    void stateChanged(int state);
    void start(int id);
    void finish(int id, bool error);
    void dataTransferProgress(qint64 done, qint64 total);
    void listInfo(const QUrlInfo & i);
    bool isFtpBusy() { return m_ftpBusy; }

private:
    //QString fileFullName(const QString& filename);
    void conncetFtpSignals();
    void clearFileResource();

    QFtp *m_ftp;
    static FtpService *s_instance;
    QFile *m_file;
    QQueue<QString> m_downloadFiles;

    int m_totalDownloadFiles;
    int m_alreadyDownloadFiles;
    bool m_ftpBusy;



};

#endif // FTPSERVICE_H
