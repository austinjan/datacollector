#include "inputpanel.h"
#include "ui_inputpanel.h"

#include <qpushbutton.h>

InputPanel::InputPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputPanel)
{
    ui->setupUi(this);
    lastFocusedWidget = NULL;
    setSignalMap();
}

InputPanel::~InputPanel()
{
    delete ui;
}

void InputPanel::saveFocusWidget(QWidget *oldFocus, QWidget *newFocus)
{
    Q_UNUSED(oldFocus);
    if (newFocus != 0 && !this->isAncestorOf(newFocus))
    {
        lastFocusedWidget = newFocus;
    }
}

void InputPanel::buttonClicked(QWidget *w)
{
    QPushButton *btn = qobject_cast<QPushButton*>(w);
    QString keyStr = keyValue(btn);
    if (keyStr == KEY_ENT)
    {
        this->close();
        emit characterGenerated(ui->lineEdit_keyBoard->text());
        ui->lineEdit_keyBoard->setText("");
    }
    else if(keyStr==KEY_ALT)
    {
        changePage();
    }
    else if(keyStr==KEY_DEL)
    {
        QString lineEditText = ui->lineEdit_keyBoard->text();
        if(lineEditText.length()>0)
        {
            lineEditText = lineEditText.left(lineEditText.length()-1);
            ui->lineEdit_keyBoard->setText(lineEditText);
        }
    }
    else if(keyStr==KEY_CAP)
    {
        transformKeyboard();
    }
    else if(keyStr==KEY_CLR)
    {
        ui->lineEdit_keyBoard->setText("");
    }
    else if(keyStr==KEY_ESC)
    {
        ui->lineEdit_keyBoard->setText("");
        this->close();
    }
    else if(keyStr==KEY_SPACE)
    {
        ui->lineEdit_keyBoard->setText(ui->lineEdit_keyBoard->text().append(" "));
    }
    else
    {
        if(keyStr==KEY_Ampersand)
        {
            keyStr = keyStr.replace("&&","&");
        }
//        if(isDefaultText==true)
//        {
//            m_lineEditKeyBoard->setText("");
//            isDefaultText = false;
//        }
//        if(keyStr=="-" && metaObject()->className()==QString("NumInputPanel"))
//        {
//            QString lineEditText = m_lineEditKeyBoard->text();

//            if(lineEditText.length()>0 && lineEditText.at(0)!=QChar('-'))
//            {
//                if(lineEditText.right(1)==QString("e"))
//                {
//                    lineEditText = lineEditText.append("-");
//                    m_lineEditKeyBoard->setText(lineEditText);
//                }
//                else
//                {
//                    lineEditText = lineEditText.prepend("-");
//                    m_lineEditKeyBoard->setText(lineEditText);
//                }
//            }
//            else if(lineEditText.length()==0)
//            {
//                lineEditText = lineEditText.prepend("-");
//                m_lineEditKeyBoard->setText(lineEditText);
//            }
//        }
//        else
        {
            ui->lineEdit_keyBoard->setText(ui->lineEdit_keyBoard->text().append(keyStr));
        }
    }


}

void InputPanel::setStartupText(const QString &text)
{
    ui->lineEdit_keyBoard->setText(text);
}

bool InputPanel::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::WindowActivate:
        if (lastFocusedWidget)
            lastFocusedWidget->activateWindow();
        break;
    default:
        break;
    }

    return QWidget::event(e);
}

void InputPanel::setSignalMap()
{
    QList<QPushButton *> allPButtons = findChildren<QPushButton *>();
    foreach(QPushButton *pushBtn,allPButtons)
    {
        signalMapper.setMapping(pushBtn,pushBtn);
        connect(pushBtn, SIGNAL(clicked()),
                &signalMapper, SLOT(map()));


//        //**** added for real keyboard ****
//        if(keyhandler)
//            keyhandler->addKey(keyValue(pushBtn), pushBtn);
//        // **** ended ****
    }
    connect(&signalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(buttonClicked(QWidget*)));
}

QString InputPanel::keyValue(QPushButton* pushBtn)
{
    QString strKeyVal;
    if(pushBtn->property("keyValue").isValid())
        strKeyVal = pushBtn->property("keyValue").toString();
    else
        strKeyVal = pushBtn->text();
    return strKeyVal;
}

void InputPanel::transformKeyboard()
{
    bool UppercaseToLowercase = false;
    bool LowercaseToUppercase = false;
    QList<QPushButton*> allPButtons;
    QString strKeyVal;

    allPButtons = findChildren<QPushButton *>();

    foreach(QPushButton *pushBtn,allPButtons)
    {
        strKeyVal = keyValue(pushBtn);
        if(strKeyVal.length()==1)
        {
            ushort key = (*strKeyVal.unicode()).unicode();
            if(key >= 65 && key <= 90)
            {
                pushBtn->setText(QChar((key+32)));
                pushBtn->setProperty("keyValue",QVariant(pushBtn->text()));
                UppercaseToLowercase = true;
            }
            else if(key >= 97 && key <= 122)
            {
                pushBtn->setText(QChar((key-32)));
                pushBtn->setProperty("keyValue",QVariant(pushBtn->text()));
                LowercaseToUppercase = true;
            }
        }
    }
}

void InputPanel::changePage()
{
    if(ui->stacked_widget->currentIndex() == 0)
        ui->stacked_widget->setCurrentIndex(1);
    else
        ui->stacked_widget->setCurrentIndex(0);

}
