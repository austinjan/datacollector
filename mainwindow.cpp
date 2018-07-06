#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileoperation.h"
#include "keyhandler.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT  480

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_centralWidget = NULL;

    m_startForm = NULL;
    m_loginForm = NULL;
    m_barcodeInputForm = NULL;
    m_productHistoryForm = NULL;
    m_settingsForm = NULL;
    m_keyhandler = NULL;
    m_departmentSelectForm = NULL;
    m_downloadForm = NULL;
    m_fixinputForm = NULL;
    m_secondTimer = new QTimer();
    connect(m_secondTimer,SIGNAL(timeout()),this,SLOT(timerTimeout()));
    m_secondTimer->start(1000);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    killChildWidgets();
    delete ui;
}

void MainWindow::timerTimeout()
{
    updateTimer();
}

void MainWindow::initMainWindow()
{
//    if(m_centralWidget)
//        setCentralWidget(m_centralWidget);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    initChildWidgets();
    setCentralWidget(m_centralWidget);

    QTabBar *tabBar = qFindChild<QTabBar *>(ui->tabWidget);
    tabBar->hide();

}

void MainWindow::initEnvironment()
{
    FileOperation::initAllSubFolders();
}

void MainWindow::changeToLoginScreen()
{
    m_centralWidget->setCurrentWidget(m_loginForm);
    m_keyhandler->setBarcodeReceiver(NULL);
}

void MainWindow::changeToSettingsScreen()
{
    m_centralWidget->setCurrentWidget(m_settingsForm);
    m_keyhandler->setBarcodeReceiver(NULL);
}

void MainWindow::changeToDepartmentSelectScreen()
{
    m_centralWidget->setCurrentWidget(m_departmentSelectForm);
    m_keyhandler->setBarcodeReceiver(NULL);
}

void MainWindow::changeToStartScreen()
{
    m_centralWidget->setCurrentWidget(m_startForm);
    m_keyhandler->setBarcodeReceiver(NULL);
}

void MainWindow::changeToBarcodeInputScreen()
{
    m_barcodeInputForm->clearProgressbar();
    m_barcodeInputForm->autoSetScanMode();
    m_centralWidget->setCurrentWidget(m_barcodeInputForm);
    m_keyhandler->setBarcodeReceiver(m_barcodeInputForm);
}

void MainWindow::changeToDownloadScreen()
{
    m_downloadForm->clearProgress();
    m_centralWidget->setCurrentWidget(m_downloadForm);
    m_keyhandler->setBarcodeReceiver(NULL);

}

void MainWindow::changeToFixScreen()
{
    m_fixinputForm->initTable();
    m_centralWidget->setCurrentWidget(m_fixinputForm);
}

void MainWindow::killChildWidgets()
{
    if(m_secondTimer)
        delete m_secondTimer;

    if(m_centralWidget)
        delete m_centralWidget;

    if(m_settingsForm)
        delete m_settingsForm;

    if(m_loginForm)
        delete m_loginForm;

    if(m_departmentSelectForm)
        delete m_departmentSelectForm;

    if(m_downloadForm)
        delete m_downloadForm;

    if(m_fixinputForm)
        delete m_fixinputForm;

    if(m_secondTimer)
        delete m_secondTimer;

    if(m_keyhandler)
        delete m_keyhandler;
}

void MainWindow::connectChangeScreenSignals()
{

    // Start Form
    connect(m_startForm,SIGNAL(changeToDepartmentScreen()),
            this,SLOT(changeToDepartmentSelectScreen()));
    connect(m_startForm,SIGNAL(changeToBarcodeInputScreen()),
            this,SLOT(changeToBarcodeInputScreen()));
    connect(m_startForm,SIGNAL(changeToSettingScreen()),
            this,SLOT(changeToSettingsScreen()));
    connect(m_startForm,SIGNAL(changeToDownloadScreen()),
            this,SLOT(changeToDownloadScreen()));


    // Department select form
    connect(m_departmentSelectForm,SIGNAL(changeToStartForm()),
            this,SLOT(changeToStartScreen()));
    connect(m_departmentSelectForm,SIGNAL(changeToLoginForm()),
            this,SLOT(changeToLoginScreen()));
    connect(m_departmentSelectForm,SIGNAL(changeToBarcodeInputForm()),
            this,SLOT(changeToBarcodeInputScreen()));

    // barcode input
    connect(m_barcodeInputForm,SIGNAL(changeToStartScreen()),
            this,SLOT(changeToStartScreen()));
    connect (m_barcodeInputForm,SIGNAL(changeToFixScreen()),
             this,SLOT(changeToFixScreen()));

    // settings
    connect(m_settingsForm,SIGNAL(changeToStartScreen()),
            this,SLOT(changeToStartScreen()));

    // download
    connect (m_downloadForm,SIGNAL(changeToStartScreen()),
             this,SLOT(changeToStartScreen()));


    // Fix
    connect (m_fixinputForm,SIGNAL(changeToStartForm()),
             this,SLOT(changeToStartScreen()));

    // keyhandler
    connect (m_keyhandler,SIGNAL(changeToStartScreen()),
             this,SLOT(changeToStartScreen()));

}

void MainWindow::updateTimer()
{
    m_startForm->updateTimer();
}

void MainWindow::initChildWidgets()
{
    m_keyhandler = new KeyHandler(this);


    m_centralWidget = ui->tabWidget;
    m_startForm = new StartForm(this);
    m_startForm->init();
    m_centralWidget->addTab(m_startForm,"Start Form");

    m_barcodeInputForm = new BarcodeInputForm(this);
    m_barcodeInputForm->init();
    m_centralWidget->addTab(m_barcodeInputForm,"Barcode Input Form");

    m_fixinputForm = new FixInputForm(this);
    m_fixinputForm->init();
    m_centralWidget->addTab(m_fixinputForm,"Fix record Form");


    m_downloadForm = new DownloadForm(this);
    m_downloadForm->init();
    m_centralWidget->addTab(m_downloadForm,"Download data Form");

    m_loginForm = new LoginForm(this);
    m_loginForm->init();
    m_loginForm->test();
    m_centralWidget->addTab(m_loginForm,"Login Form");


    m_settingsForm = new SettingsForm(this);
    m_settingsForm->init();
    m_centralWidget->addTab(m_settingsForm,"Settings Form");

    m_departmentSelectForm = new DepartmentSelectForm(this);
    m_departmentSelectForm->init();
    m_centralWidget->addTab(m_departmentSelectForm,"Department Select Form");

    m_centralWidget->setCurrentWidget(m_startForm);

    connectChangeScreenSignals();

}

