#include "departmentselectform.h"
#include "ui_departmentselectform.h"

#include "filedataloader.h"
#include "information.h"

#include <qevent.h>
#include <QApplication>
#include <QScrollBar>
#include <QMessageBox>

DepartmentSelectForm::DepartmentSelectForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DepartmentSelectForm)
{
    ui->setupUi(this);

}

DepartmentSelectForm::~DepartmentSelectForm()
{
    delete ui;
}

void DepartmentSelectForm::init()
{
    Information* informatio = Information::instance();
    informatio->addDescription(ui->labelLoginInformation);
    initChildWidget();
}

void DepartmentSelectForm::initChildWidget()
{
    initListWidget();
    initPushButtons();
}

void DepartmentSelectForm::initPushButtons()
{
    connect(ui->pushButtonBack,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonBackClicked()));
    connect(ui->pushButtonPageUpLogin,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonPageUpLoginClicked()));
    connect(ui->pushButtonPageDownLogin,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonPageDownLoginClicked()));
    connect(ui->pushButtonLogin,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonLoginClicked()));
    connect(ui->pushButtonNext,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNextClicked()));
    connect(ui->pushButtonLogout,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonLogout()));
}

void DepartmentSelectForm::initListWidget()
{
    ui->listWidgetDepartment->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listEmployee->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listEmployee->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidgetDepartment->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    DepartmentDataLoader* loader = DepartmentDataLoader::instance();
    QStringList dep_names = loader->getAllDepartmentName();

    foreach(QString name, dep_names)
    {
        new QListWidgetItem(name, ui->listWidgetDepartment);
    }

    LoginDataLoader* login_loader = LoginDataLoader::instance();
    QStringList emplyee_names = login_loader->getAllEmployeesName();
    foreach(QString name, emplyee_names)
    {
        new QListWidgetItem(name, ui->listEmployee);
    }


}

bool DepartmentSelectForm::validateLogin()
{
    QListWidgetItem *item = ui->listWidgetDepartment->currentItem();
    if(!item)
    {
        QString _msg = QString("Please select department.");
        QMessageBox::warning(this,tr("Data Collector"),
                             _msg);
        return false;
    }
    QString department = item->text().simplified();

    item = NULL;
    item = ui->listEmployee->currentItem();
    if(!item)
    {
        QString _msg = QString("Please select login name.");
        QMessageBox::warning(this,tr("Data Collector"),
                             _msg);
        return false;
    }

    QString account = item->text().simplified();
    QString password = ui->linePassword->text();
    LoginDataLoader *login_loader = LoginDataLoader::instance();
    bool match = login_loader->LoginMatch(account,password);

    if(!match)
    {
        QString _msg = QString("Wrong password!");
        QMessageBox::warning(this,tr("Data Collector"),
                             _msg);
        return false;
    }

    return true;
}

void DepartmentSelectForm::on_pushButtonPageUp_clicked()
{
    //ui->listWidgetDepartment->moveCursor(QAbstractItemView::MovePageUp);
    QScrollBar *scrollbar = ui->listWidgetDepartment->verticalScrollBar();
    scrollbar->setValue(scrollbar->value() - scrollbar->pageStep());

}

void DepartmentSelectForm::on_pushButtonPageDown_clicked()
{
    QScrollBar *scrollbar = ui->listWidgetDepartment->verticalScrollBar();
    scrollbar->setValue(scrollbar->value() + scrollbar->pageStep());

}

void DepartmentSelectForm::pushButtonPageUpLoginClicked()
{
    QScrollBar *scrollbar = ui->listEmployee->verticalScrollBar();
    scrollbar->setValue(scrollbar->value() - scrollbar->pageStep());
}

void DepartmentSelectForm::pushButtonPageDownLoginClicked()
{
    QScrollBar *scrollbar = ui->listEmployee->verticalScrollBar();
    scrollbar->setValue(scrollbar->value() + scrollbar->pageStep());
}

void DepartmentSelectForm::processLogIn()
{
    Information* info = Information::instance();

    QString depname = ui->listWidgetDepartment->currentItem()->text();
    info->setDepartmentName(depname);
    QString name = ui->listEmployee->currentItem()->text();
    info->setLonginName(name);
    ui->linePassword->clear();

}

void DepartmentSelectForm::pushButtonLoginClicked()
{
    bool is_match = validateLogin();
    if(!is_match)
        return;

    processLogIn();
    QString _msg = QString("Login  success");
    QMessageBox::information(this,"Data Collector",_msg);
    //emit changeToBarcodeInputForm();
}

void DepartmentSelectForm::pushButtonBackClicked()
{
    emit changeToStartForm();
}

void DepartmentSelectForm::pushButtonNextClicked()
{
    Information *info = Information::instance();
    Information::LoginStatus login_status = info->loginStatus();

    if(login_status == Information::AllLogin)
    {
        emit changeToBarcodeInputForm();
    }
    else if(login_status == Information::NoLogin)
    {
        QMessageBox::warning(this,"Data Collector",
                             "Please Login and select department!");
    }
    else if(login_status == Information::DepartmentLogin)
    {
        QMessageBox::warning(this,"Data Collector",
                             "Please Login !");
    }
    else if(login_status == Information::EmployeeLogin)
    {
        QMessageBox::warning(this,"Data Collector",
                             "Please select department !");
    }
}

void DepartmentSelectForm::on_pushButtonOK_clicked()
{
    Information* info = Information::instance();
    QString name = ui->listWidgetDepartment->currentItem()->text();
    info->setDepartmentName(name);
    emit changeToLoginForm();
}

void DepartmentSelectForm::pushButtonLogout()
{
    Information::instance()->setLonginName(QString());
    Information::instance()->setDepartmentName(QString());
}

//void DepartmentSelectForm::on_listWidgetDepartment_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
//{
//    Q_UNUSED(current);
//    Q_UNUSED(previous);

//}

void DepartmentSelectForm::on_pushButtonBack_clicked()
{
    emit changeToStartForm();
}
