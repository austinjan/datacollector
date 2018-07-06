#include "barcodeinputform.h"
#include "ui_barcodeinputform.h"

#include <QTextStream>
#include <QDir>
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>
#include <QLineEdit>
#include <QTimer>
#include <QRegExp>
#include <QMessageBox>

#include "information.h"
#include "filedataloader.h"
#include "fileoperation.h"
#include "ftpservice.h"

BarcodeInputForm::BarcodeInputForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BarcodeInputForm)
{
    m_logfileName = QString();
    m_hadCsCode = false;
    m_hadPlateNumber = false;
    ui->setupUi(this);
}

BarcodeInputForm::~BarcodeInputForm()
{
    delete ui;
}

bool BarcodeInputForm::init()
{
    m_logfileName = FileOperation::chekLogFileExist();

    ui->pushButtonModify->hide();
    Information* informatio = Information::instance();
    informatio->addLoginLabel(ui->labelLogin);
    informatio->addDepartmentLabel(ui->labelDepartment);
    informatio->setLonginName(QString());
    informatio->setDepartmentName(QString());



    connect(ui->lineCsCode,SIGNAL(textChanged(QString)),
            this,SLOT(lineCsCodeTextChanged(QString)));
    connect(ui->linePlateNumber,SIGNAL(textChanged(QString)),
            this,SLOT(linePlateNumberTextChanged(QString)));

    connect(ui->pushButtonBack,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonBackClicked()));
    connect(ui->pushButtonNext,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNextClicked()));
    connect(ui->pushButtonSend,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonSendClicked()));

    connect(ui->pushButtonOK,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonOKClicked()));
    connect(ui->pushButtonModify,SIGNAL(clicked(bool)),
            this,SIGNAL(changeToFixScreen()));

    FtpService* ftp = FtpService::instance();
    connect(ftp,SIGNAL(putFinished()),
            this,SLOT(ftpPutFinished()));
    connect(ftp,SIGNAL(updateProgressInformation(int,int)),
            this,SLOT(ftpPutProgress(int,int)));

    connectNumpad();
    //QTimer::singleShot(0,ui->lineBarcode,SLOT(setFocus()));
    return true;
}

void BarcodeInputForm::test()
{
    //QFile file;
    FileOperation::chekLogFileExist();
}

bool BarcodeInputForm::clearProgressbar()
{
    ui->progressBarSend->reset();
    return true;
}

bool BarcodeInputForm::autoSetScanMode()
{
    Information* info = Information::instance();
    QString department = info->department();
    if(department.toLower().startsWith("kiln"))
        settingScanMode(KilnMode);

    return true;
}

void BarcodeInputForm::hidePlateNumberWidgets()
{
    ui->checkBoxKeepPlateNumber->hide();
    ui->linePlateNumber->hide();
    ui->labelPlateNumber->hide();
}

void BarcodeInputForm::showPlateNumberWidgets()
{
    ui->checkBoxKeepPlateNumber->show();
    ui->linePlateNumber->show();
    ui->labelPlateNumber->show();
}

bool BarcodeInputForm::settingScanMode(BarcodeInputForm::ScanMode mode)
{

    m_scanMode = mode;
    if(mode == BarcodeInputForm::KilnMode)
    {
        hidePlateNumberWidgets();
    }
    else
    {
        showPlateNumberWidgets();
    }
    return true;
}

void BarcodeInputForm::updateNavigationButtons()
{
    if(m_hadCsCode && m_hadPlateNumber)
    {
        ui->pushButtonNext->setEnabled(true);
        ui->pushButtonSend->setEnabled(true);
    }
    else
    {
        ui->pushButtonNext->setEnabled(false);
        ui->pushButtonSend->setEnabled(false);
    }
}

bool BarcodeInputForm::validateInput()
{
    QString quantity = ui->LineEditQuantity->text();
    QRegExp re("-?\\d*");
    QString _msg;
    if(!m_hadCsCode)
    {
        _msg = QString("Please scan or input cs code!");
        QMessageBox::warning(this,tr("Data Collector"),
                             _msg);
        return false;
    }

    if(!m_hadPlateNumber && m_scanMode!=KilnMode)
    {
        _msg = QString("Please scan or input plate number!");
        QMessageBox::warning(this,tr("Data Collector"),
                             _msg);
        return false;
    }

    if( quantity.isEmpty())
    {
        _msg = QString("Please input quantity!");
        QMessageBox::warning(this,tr("Data Collector"),
                             _msg);
        return false;
    }

    if(!re.exactMatch(quantity) )
    {
        _msg = QString("Quantity should be number!");
        QMessageBox::warning(this,tr("Data Collector"),
                             _msg);
        return false;
    }
    return true;
}

void BarcodeInputForm::connectNumpad()
{
    connect(ui->pushButton0,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButton1,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButton2,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButton3,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButton4,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButton5,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButton6,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButton7,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButton8,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButton9,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButtonNeg,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
    connect(ui->pushButtonCLR,SIGNAL(clicked(bool)),
            this,SLOT(pushButtonNumpadClicked()));
}

void BarcodeInputForm::barcodeSend(const QByteArray &context)
{
    bool isCsCode = isCSCode(context);
    if (isCsCode)
    {
        // cs code
        m_hadCsCode = true;
        updateProducImage(context);
        ui->lineCsCode->setText(context);
    }
    else
    {
        // plate number
        QRegExp re("\\d*");
        if(re.exactMatch(context.simplified()))
        {
            m_hadPlateNumber = true;
            ui->linePlateNumber->setText(context);
        }
        else
        {
            m_hadPlateNumber = false;
            QString _msg=QString("Plate number should be number!");
            QMessageBox::warning(this,tr("Data Collector"),
                                 _msg);
        }
    }

}

void BarcodeInputForm::ftpPutFinished()
{
    ui->labelFtpState->setText("Waiting");
    //ui->pushButtonSend->setEnabled(true);
}

void BarcodeInputForm::ftpPutProgress(int current, int total)
{
    ui->progressBarSend->setMaximum(total);
    ui->progressBarSend->setValue(current);
}

void BarcodeInputForm::lineCsCodeTextChanged(const QString &code)
{
    if(code.isEmpty())
    {
        m_hadCsCode=false;
        return;
    }
    m_hadCsCode = updateProducImage(code);

}

void BarcodeInputForm::linePlateNumberTextChanged(const QString &code)
{
    if(!code.isEmpty())
        m_hadPlateNumber = true;
    else
        m_hadPlateNumber = false;

}

void BarcodeInputForm::pushButtonOKClicked()
{
    bool _success = validateInput();
    if(!_success)
        return;

    LogFileOprator* log = LogFileOprator::instance();
    QString qty = ui->LineEditQuantity->text().simplified();
    QString cscode = ui->lineCsCode->text().simplified();
    QString plate_number = ui->linePlateNumber->text().simplified();
    LogFileOprator::LogState log_state = log->log(m_logfileName,qty,cscode,plate_number);

    if (log_state == LogFileOprator::Duplicate)
    {
        QMessageBox::warning(this,"Write Data","Data already recorded.");

    }

    clear();
}

void BarcodeInputForm::pushButtonSendClicked()
{
//    bool _success = validateInput();
//    if(!_success)
//        return;
    FtpService* ftp = FtpService::instance();
    if(!ftp->isFtpBusy())
    {
        ui->labelFtpState->setText("Uploading...");
        ftp->uploadlogfile();
        //ui->pushButtonSend->setEnabled(false);
    }
    else
    {
        ui->labelFtpState->setText("Ftp Busy");
    }

}

void BarcodeInputForm::pushButtonNextClicked()
{
    bool _success = validateInput();
    if(!_success)
        return;
}

void BarcodeInputForm::pushButtonBackClicked()
{
    emit changeToStartScreen();
}

void BarcodeInputForm::pushButtonNumpadClicked()
{
    QString text = ui->LineEditQuantity->text();
    QString input_text = sender()->property("value").toString();
    if(input_text == "C")
        ui->LineEditQuantity->clear();
    else if (input_text == "-")
    {
        if(text.isEmpty())
            ui->LineEditQuantity->setText("-");
    }
    else
    {
        ui->LineEditQuantity->setText(text+input_text);
    }

}



//void BarcodeInputForm::numberKeyPress()
//{

//    QPushButton* button= qobject_cast<QPushButton*>(sender());
//    if(!button)
//        return;
//    QString button_text = button->text();
//    QString count_lineedit_text = ui->LineEditorCount->text();
//    count_lineedit_text = count_lineedit_text+button_text;
//    ui->LineEditorCount->setText(count_lineedit_text);
//}

///
/// \brief BarcodeInputForm::logFileName create log file name
/// by date.
/// \return log file name
///
//QString BarcodeInputForm::logFileName()
//{
//    QDateTime today = QDateTime::currentDateTime();
//    QString log_file_name = today.toString("yyyyMMdd");
//    return QString("log")+"-"+log_file_name;
//}

void BarcodeInputForm::setCsCodeBarcodeState(bool ok)
{
    if(ok)
    {
        ui->lineCsCode->setStyleSheet("background-color: green;"
                                       "color: white;");
    }
    else
    {
        ui->lineCsCode->setStyleSheet("background-color: yellow;"
                                       "color: black;");
    }
}

void BarcodeInputForm::setPlateNumberBarcodeState(bool ok)
{
    if(ok)
    {
        ui->linePlateNumber->setStyleSheet("background-color: green;"
                                       "color: white;");
    }
    else
    {
        ui->linePlateNumber->setStyleSheet("background-color: yellow;"
                                       "color: black;");
    }
}

void BarcodeInputForm::loadPicture()
{
    QString cscode = ui->lineCsCode->text();

}

bool BarcodeInputForm::updateProducImage(const QString &cscode)
{
    PictureDataloader *pic_loader = PictureDataloader::instance();
    QString clean_filename = cscode.simplified();
    QString pic_full_name = pic_loader->picExist(clean_filename);
    if(pic_full_name.isEmpty())
    {
//        QString _msg=QString("%1 picture not exist!").arg(clean_filename);
//        int ret = QMessageBox::warning(this,tr("Data Collector"),
//                             _msg,QMessageBox::Ok | QMessageBox::Cancel);
//        // load default pic
//        if(ret == QMessageBox::Cancel)
//        {
//            ui->lineCsCode->clear();
//            return false;
//        }
//        else
//        {
//            return true;
//        }
    }
    else
    {
        QPixmap pic;
        bool _success = pic.load(pic_full_name);
//        QImage img;
        if(!_success)
        {
            qDebug() << __func__ <<"Load pic error " << pic_full_name;
        }
        pic.scaled(PIC_DETAIL_WIDTH,PIC_DETAIL_HEIGHT);
        ui->labelPicture->setPixmap(pic);
        ui->labelPicture->update();


    }
    return true;
}

bool BarcodeInputForm::clear()
{
    if(!ui->checkBoxKeepCsCode->isChecked())
        ui->lineCsCode->clear();
    if(!ui->checkBoxKeepQty->isChecked())
        ui->LineEditQuantity->clear();
    if(!ui->checkBoxKeepPlateNumber->isChecked())
        ui->linePlateNumber->clear();

    return true;
}

//void BarcodeInputForm::initializeFile(const QString &fullName)
//{
//    QFile file(fullName);
//    bool isOpened = file.open(QIODevice::ReadWrite);
//    if (!isOpened)
//    {
//        qDebug() << "void BarcodeInputForm::initializeLogFile() can not create file" << fullName;
//    }
//    file.close();
//}

//QString LOG_PATH = QString("log");

//QString BarcodeInputForm::chekLogFileExist()
//{


//    QDir log_file_dir = QDir::current();


//    if(!log_file_dir.exists(LOG_PATH))
//    {
//        bool _success = log_file_dir.mkdir(LOG_PATH);
//        if(!_success)
//        {
//            qDebug() << "bool BarcodeInputForm::openLogFile mkdir fail" ;
//            return QString();
//        }

//    }

//    log_file_dir.cd(LOG_PATH);

//    QFileInfo log_file_info(log_file_dir,logFileName());

//    if (!log_file_info.exists())
//    {
//        initializeFile(log_file_info.absoluteFilePath());
//    }

//    QString log_full_name =
//            log_file_dir.absoluteFilePath(logFileName());



//    QFile file(log_full_name);
//    bool isOpened = file.open(QIODevice::ReadWrite);
//    if (!isOpened)
//    {
//        qDebug()<< "BarcodeInputForm::openLogFile can not create file" << log_full_name;
//        return QString();
//    }
//    file.close();

//    return log_full_name;
//    //qDebug() << "logfile name is " << log_full_name;

////    return true;
//}





//void BarcodeInputForm::clearBarcodeLine()
//{
//    QString cs_code = ui->lineBarcode->text();
//    ui->labelCSCode->setText(cs_code);
//    ui->lineBarcode->setText(QString());
//    setBarcodeReady(true);
//}



bool BarcodeInputForm::isCSCode(const QString &barcode)
{
       QRegExp rx("[a-zA-Z]+");
       int index = rx.indexIn(barcode);
       if(index <0)
           return false;
       else
           return true;
}
