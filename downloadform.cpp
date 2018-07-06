#include "downloadform.h"
#include "ui_downloadform.h"

#include <QFtp>

#include "ftpservice.h"

DownloadForm::DownloadForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadForm)
{
    ui->setupUi(this);
}

DownloadForm::~DownloadForm()
{
    delete ui;
}

void DownloadForm::init()
{
    connectSignals();


}

void DownloadForm::clearProgress()
{
    ui->progressBarFIle->reset();
    ui->progressBarTask->reset();
}

void DownloadForm::updateInformation()
{

}

void DownloadForm::updateProgress(int current, int total)
{
    if(current==0 && total==0)
    {
        ui->progressBarFIle->reset();
    }
    ui->progressBarFIle->setMaximum(total);
    ui->progressBarFIle->setValue(current);
}

void DownloadForm::updateTaskProgress(int current, int total)
{
    if(current==0 && total==0)
    {
        ui->progressBarTask->reset();
    }
    ui->progressBarTask->setMaximum(total);
    ui->progressBarTask->setValue(current);
}

void DownloadForm::pushButtonBackClicked()
{
    emit changeToStartScreen();
}

void DownloadForm::ftpUpdateState(int state)
{
    QString state_string;
    FtpService *ftp = FtpService::instance();
//    Unconnected,
//    HostLookup,
//    Connecting,
//    Connected,
//    LoggedIn,
    switch(state)
    {
    case QFtp::Connecting:
        state_string = QString("Connectiong %1 ...")
                .arg(ftp->hostname());
        //connecting
        break;
    case QFtp::Connected:
        state_string = QString("Connected %1 ...")
                .arg(ftp->hostname());
        //connected
        break;
    case QFtp::Login:
        state_string = QString("Login host %1")
                .arg(ftp->hostname());
        break;
//    case QFtp::LoggedIn:
//        state_string = QString("Already login host %1")
//                .arg(ftp->hostname());
//        break;
//    case QFtp::Get:
//        state_string = QString("Downloading...");

//        break;
//    case QFtp::Put:
//        state_string = QString("Uploading...");
//        break;
    case QFtp::Closing:
    default:
        state_string = QString("Waiting command.");
        break;
    }
    ui->labelState->setText(state_string);
}

void DownloadForm::pushButtonFtpDownloadClicked()
{
    FtpService *ftp = FtpService::instance();
    ftp->downloadAll();
}

void DownloadForm::connectSignals()
{
    connect(ui->pushButtonBack,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonBackClicked()));
    connect(ui->pushButtonFtpDownload,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonFtpDownloadClicked()));

    // ftp
    FtpService* ftp = FtpService::instance();
    connect(ftp,SIGNAL(updateState(int)),
            this,SLOT(ftpUpdateState(int)));
    connect(ftp,SIGNAL(updateDownloadTask(int,int)),
            this,SLOT(updateTaskProgress(int,int)));
    connect(ftp,SIGNAL(updateProgressInformation(int,int)),
            this,SLOT(updateProgress(int,int)));
}
