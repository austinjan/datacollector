#include "producthistoryform.h"
#include "ui_producthistoryform.h"

ProductHistoryForm::ProductHistoryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductHistoryForm)
{
    ui->setupUi(this);
}

ProductHistoryForm::~ProductHistoryForm()
{
    delete ui;
}
