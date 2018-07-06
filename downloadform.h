#ifndef DOWNLOADFORM_H
#define DOWNLOADFORM_H

#include <QWidget>

class FtpService;
namespace Ui {
class DownloadForm;
}

class DownloadForm : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadForm(QWidget *parent = 0);
    ~DownloadForm();
    void init();
    void clearProgress();

signals:
    void changeToStartScreen();

public slots:
    void updateInformation();
    void updateProgress(int current, int total);
    void updateTaskProgress(int current,int total);
    void pushButtonBackClicked();
    void ftpUpdateState(int state);
    void pushButtonFtpDownloadClicked();

private:
    Ui::DownloadForm *ui;

    void connectSignals();
};

#endif // DOWNLOADFORM_H
