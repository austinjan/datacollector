#include "fixinputform.h"
#include "ui_fixinputform.h"
#include "filedataloader.h"
#include "fileoperation.h"
#include <QTableWidget>
#include <QTableWidgetItem>

FixInputForm::FixInputForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FixInputForm)
{
    ui->setupUi(this);
}

FixInputForm::~FixInputForm()
{
    delete ui;
}

void FixInputForm::initTable()
{
    QStringList last_5_log = LogFileOprator::instance()->readLast5Log();
    int _row=0;
    int _column = 0;
    foreach (QString line,last_5_log)
    {
        QStringList _columns = line.split(",");
        foreach(QString _text,_columns)
        {
            QTableWidgetItem *item = new QTableWidgetItem(_text.trimmed());
            ui->tableWidget->setItem(_row,_column,item);
            _column++;
        }
        _column =0;
        _row++;
    }

}

void FixInputForm::init()
{
//    QTableWidget* t = ui->tableWidget;
//    t->horizontalHeaderItem(LogFileOprator::DepartmentColumn)
//            ->setText("Dep.");
//    t->horizontalHeaderItem(LogFileOprator::CsCodeColumn)
//            ->setText("Cs Code");
//    t->horizontalHeaderItem(LogFileOprator::PlateNumberColumn)
//            ->setText("Plate #");
//    t->horizontalHeaderItem(LogFileOprator::QuantityColumn)
//            ->setText("Qty");
//    t->horizontalHeaderItem(LogFileOprator::DateTimeColumn)
//            ->setText("Time");
//    t->horizontalHeaderItem(LogFileOprator::MachineIDColumn)
//            ->setText("Scaner");
//    t->horizontalHeaderItem(LogFileOprator::OperatorColumn)
//            ->setText("Op.");

}
