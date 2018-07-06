#ifndef DEPARTMENTSELECTFORM_H
#define DEPARTMENTSELECTFORM_H

#include <QListWidget>
#include <QWidget>

namespace Ui {
class DepartmentSelectForm;
}

class DepartmentSelectForm : public QWidget
{
    Q_OBJECT

public:
    explicit DepartmentSelectForm(QWidget *parent = 0);
    ~DepartmentSelectForm();

    void init() ;


signals:
    void changeToStartForm();
    void changeToLoginForm();
    void changeToBarcodeInputForm();

private slots:
    void on_pushButtonPageUp_clicked();

    void on_pushButtonPageDown_clicked();

    void pushButtonPageUpLoginClicked();
    void pushButtonPageDownLoginClicked();
    void pushButtonLoginClicked();
    void pushButtonBackClicked();
    void pushButtonNextClicked();

    void on_pushButtonOK_clicked();
    void pushButtonLogout();

    //void on_listWidgetDepartment_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_pushButtonBack_clicked();

private:
    Ui::DepartmentSelectForm *ui;

    void initChildWidget();
    void initPushButtons();
    void initListWidget();
    bool validateLogin();

    void processLogIn();
};

#endif // DEPARTMENTSELECTFORM_H
