#ifndef FIXINPUTFORM_H
#define FIXINPUTFORM_H

#include <QWidget>

namespace Ui {
class FixInputForm;
}

class FixInputForm : public QWidget
{
    Q_OBJECT

signals:
    void changeToStartForm();

public:
    explicit FixInputForm(QWidget *parent = 0);
    ~FixInputForm();

    void initTable();
    void init();

private:
    Ui::FixInputForm *ui;
};

#endif // FIXINPUTFORM_H
