#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>

#include "filedataloader.h"
#include "keyhandler.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();

    void test();
    void init();// {settingBackground();}


private slots:
    void on_lineDepartment_selectionChanged();
    void on_lineDepartment_textChanged(const QString &arg1);
    void numberKeyPress();

private:
    Ui::LoginForm *ui;

    void settingBackground();

    void initEmployeesListWidget();

    LoginDataLoader *m_dataloader;

};

#endif // LOGINFORM_H
