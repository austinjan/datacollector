#ifndef BARCODEINPUTFORM_H
#define BARCODEINPUTFORM_H

#include <QWidget>
#include <QFile>
#include "keyhandler.h"

namespace Ui {
class BarcodeInputForm;
}

class BarcodeInputForm : public QWidget, public  BarcodeReceiveInterface
{
    Q_OBJECT

public:
    explicit BarcodeInputForm(QWidget *parent = 0);
    ~BarcodeInputForm();
    enum ScanMode{
        Normal,
        KilnMode,
        PackingMode,
        ContinueMode
    };
    bool init();
    void test();
    bool clearProgressbar();
    bool autoSetScanMode();
    bool settingScanMode(ScanMode mode = Normal);
    virtual void barcodeSend(const QByteArray& context);

signals:
    void changeToStartScreen();
    void changeToFixScreen();

public slots:
    void ftpPutFinished();
    void ftpPutProgress(int current, int total);
private slots:

    void lineCsCodeTextChanged(const QString& code);
    void linePlateNumberTextChanged(const QString& code);
    void pushButtonOKClicked();
    void pushButtonSendClicked();
    void pushButtonNextClicked();
    void pushButtonBackClicked();
    void pushButtonNumpadClicked();


private:
    Ui::BarcodeInputForm *ui;

    bool isCSCode(const QString& barcode);

//    void initializeFile(const QString &fullName);
//    QString chekLogFileExist();
//    QString logFileName();
    void setCsCodeBarcodeState(bool ok);
    void setPlateNumberBarcodeState(bool ok);
    void loadPicture();
    bool updateProducImage(const QString& cscode);

    bool clear();
    void updateNavigationButtons();
    bool validateInput();
    void connectNumpad();
    void hidePlateNumberWidgets();
    void showPlateNumberWidgets();


    QString m_logfileName;
    bool m_hadCsCode;
    bool m_hadPlateNumber;
    ScanMode m_scanMode;



};

#endif // BARCODEINPUTFORM_H
