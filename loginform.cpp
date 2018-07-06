#include "loginform.h"
#include "ui_loginform.h"

#include <QDir>
#include <QDebug>
#include <QTimer>



LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    m_dataloader = new LoginDataLoader();
}

LoginForm::~LoginForm()
{
    delete ui;
    delete m_dataloader;
}

void LoginForm::test()
{
    LoginDataLoader *loader = new LoginDataLoader();
    loader->test();
    delete loader;
}

void LoginForm::init()
{
    //settingBackground();
    //setBarcodeReady(true);

    initEmployeesListWidget();

    connect(ui->pushButton_1,SIGNAL(clicked(bool)),this,SLOT(numberKeyPress()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(numberKeyPress()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(numberKeyPress()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(numberKeyPress()));
    connect(ui->pushButton_5,SIGNAL(clicked(bool)),this,SLOT(numberKeyPress()));
    connect(ui->pushButton_6,SIGNAL(clicked(bool)),this,SLOT(numberKeyPress()));
    connect(ui->pushButton_7,SIGNAL(clicked(bool)),this,SLOT(numberKeyPress()));
    connect(ui->pushButton_8,SIGNAL(clicked(bool)),this,SLOT(numberKeyPress()));
    connect(ui->pushButton_9,SIGNAL(clicked(bool)),this,SLOT(numberKeyPress()));
    connect(ui->pushButton_0,SIGNAL(clicked(bool)),this,SLOT(numberKeyPress()));

    //QTimer::singleShot(0,ui->lineDepartment,SLOT(setFocus()));
}



void LoginForm::settingBackground()
{
    QDir dir(":/");
    //qDebug() << dir.entryList();
    QPixmap bkgnd(":/res/images/black_background_metal-wallpaper-800x480.jpg");
    //bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(bkgnd));
    this->setPalette(palette);
}


void LoginForm::initEmployeesListWidget()
{
//    ui->listWidgetEmployee->setsize
//    QStringList names = m_dataloader->getAllEmployeesName();
//    foreach(QString name, names)
//    {
//        new QListWidgetItem(name,ui->listWidgetEmployee);
//    }

}



void LoginForm::on_lineDepartment_selectionChanged()
{
    //nothing to do now

}

void LoginForm::numberKeyPress()
{

    QPushButton* button= qobject_cast<QPushButton*>(sender());
    if(!button)
        return;
    QString button_text = button->text();
    QString password_lineedit_text = ui->linePassword->text();
    password_lineedit_text = password_lineedit_text+button_text;
    ui->linePassword->setText(password_lineedit_text);
}

void LoginForm::on_lineDepartment_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
//    setBarcodeReady(false);
//    QTimer::singleShot(500,this,SLOT(clearBarcodeLine()));
}
