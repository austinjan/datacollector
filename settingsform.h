#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = 0);
    ~SettingsForm();
    void init();

signals:
    void changeToStartScreen();

private slots:
    void pushButtonBackClicked();
    void pushButtonFtpOkClicked();

private:
    Ui::SettingsForm *ui;

    void connectWidgets();
};

#endif // SETTINGSFORM_H
