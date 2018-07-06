#ifndef STARTFORM_H
#define STARTFORM_H

#include <QWidget>

namespace Ui {
class StartForm;
}

class StartForm : public QWidget
{
    Q_OBJECT

public:
    explicit StartForm(QWidget *parent = 0);
    ~StartForm();
    bool init();
    void updateTimer();

signals:
    void changeToLogin();
    void changeToDepartmentScreen();
    void changeToBarcodeInputScreen();
    void changeToSettingScreen();
    void changeToDownloadScreen();

private slots:
    void on_pushButtonLogin_clicked();
    void on_pushButtonRecipe_clicked();
    void on_pushButton_clicked();
    void pushButtonSettingClicked();

private:
    Ui::StartForm *ui;

};

#endif // STARTFORM_H
