#ifndef PRODUCTHISTORYFORM_H
#define PRODUCTHISTORYFORM_H

#include <QWidget>

namespace Ui {
class ProductHistoryForm;
}

class ProductHistoryForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductHistoryForm(QWidget *parent = 0);
    ~ProductHistoryForm();

private:
    Ui::ProductHistoryForm *ui;
};

#endif // PRODUCTHISTORYFORM_H
