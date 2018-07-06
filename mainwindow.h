#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "loginform.h"
#include "startform.h"
#include "producthistoryform.h"
#include "barcodeinputform.h"
#include "settingsform.h"
#include "departmentselectform.h"
#include "downloadform.h"
#include "fixinputform.h"

#include <QTabWidget>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class KeyHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initMainWindow();
    void initEnvironment();

public slots:
    void changeToLoginScreen();
    void changeToSettingsScreen();
    void changeToDepartmentSelectScreen();
    void changeToStartScreen();
    void changeToBarcodeInputScreen();
    void changeToDownloadScreen();
    void changeToFixScreen();

private slots:
    void timerTimeout();


private:
    Ui::MainWindow *ui;
//    QWidget *m_centralWidget;
    StartForm *m_startForm;
    LoginForm *m_loginForm;
    BarcodeInputForm *m_barcodeInputForm;
    ProductHistoryForm *m_productHistoryForm;
    SettingsForm *m_settingsForm;
    DepartmentSelectForm *m_departmentSelectForm;
    DownloadForm *m_downloadForm;
    FixInputForm *m_fixinputForm;
    QTimer *m_secondTimer;
    KeyHandler *m_keyhandler;

    QTabWidget *m_centralWidget;
    void initChildWidgets();
    void killChildWidgets();
    void connectChangeScreenSignals();

    void updateTimer();

};


#endif // MAINWINDOW_H
