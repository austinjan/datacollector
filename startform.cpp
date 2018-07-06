#include "startform.h"
#include "ui_startform.h"

#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include "information.h"

QString VERSION_TEXT = QString("V1.00.17");

StartForm::StartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartForm)
{
    ui->setupUi(this);


//    QDir dir(":/");
////    qDebug() << dir.entryList();
//    QPixmap bkgnd(":/images/background800_480_black.jpg");
//    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
//    QPalette palette;
//    palette.setBrush(QPalette::Background, QBrush(bkgnd));
//    this->setPalette(palette);
}

StartForm::~StartForm()
{
    delete ui;
}

bool StartForm::init()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    ui->labelTime->setText(currentTime.toString("yyyy/MM/dd-hh:mm:ss"));
    Information* informatio = Information::instance();
    informatio->addLoginLabel(ui->labelLogin);
    informatio->addDepartmentLabel(ui->labelDepartment);
    informatio->setLonginName(QString());
    informatio->setDepartmentName(QString());


    ui->labelVersion->setText(VERSION_TEXT);
    connect(ui->pushButtonSettings,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonSettingClicked()));
    return true;
}

void StartForm::updateTimer()
{
    if(!this->isVisible())
        return;

    QDateTime currentTime = QDateTime::currentDateTime();
    ui->labelTime->setText(currentTime.toString("yyyy/MM/dd-hh:mm:ss"));
}

void StartForm::on_pushButtonLogin_clicked()
{
    emit changeToDepartmentScreen();
}

void StartForm::on_pushButtonRecipe_clicked()
{
    emit changeToDownloadScreen();
}

void StartForm::on_pushButton_clicked()
{
    Information *info = Information::instance();
    if (info->loginStatus() != Information::AllLogin)
    {
        QMessageBox::warning(this,"Data Collector","Please login firest!");
        return;
    }
    emit changeToBarcodeInputScreen();
}

void StartForm::pushButtonSettingClicked()
{
    emit changeToSettingScreen();
}
