#include "settingsform.h"
#include "ui_settingsform.h"

#include "filedataloader.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::init()
{
    SettingsDataLoader *settings = SettingsDataLoader::instance();
    QString host = settings->getValue("host");
    QString name = settings->getValue("user");
    QString password = settings->getValue("password");
    QString scanerID = settings->getValue("scanerID");
    ui->lineEditHost->setText(host);
    ui->lineEditUser->setText(name);
    ui->lineEditPassword->setText(password);
    ui->lineEditScanerID->setText(scanerID);
    connectWidgets();
}

void SettingsForm::pushButtonBackClicked()
{
    emit changeToStartScreen();
}

void SettingsForm::pushButtonFtpOkClicked()
{
    SettingsDataLoader *setting_loader = SettingsDataLoader::instance();
    QString user = ui->lineEditUser->text();
    setting_loader->setValue("user",user);
    QString password = ui->lineEditPassword->text();
    setting_loader->setValue("password",password);
    QString host = ui->lineEditHost->text();
    setting_loader->setValue("host",host);
    QString scanerID = ui->lineEditScanerID->text();
    setting_loader->setValue("scanerID",scanerID);
}

void SettingsForm::connectWidgets()
{
    connect(ui->pushButtonBack,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonBackClicked()));

    connect(ui->pushButtonFtpOK,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonFtpOkClicked()));
}
