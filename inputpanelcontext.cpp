#include "inputpanelcontext.h"

#include <QApplication>
#include <QDebug>
#include <QLineEdit>
#include <QPointer>

InputPanelContext::InputPanelContext()
{
    m_inputPanel = new InputPanel;
    m_focusWidget = NULL;
    connect(m_inputPanel,
            SIGNAL(characterGenerated(QString)),SLOT(sendCharacter(QString)));
}

bool InputPanelContext::filterEvent(const QEvent *event)
{
    if (event->type() == QEvent::RequestSoftwareInputPanel)
    {
        if(!focusWidget())
        {
            qDebug() << "nputPanelContext::filterEvent" << "no focus widget";
            return false;

        }
        m_focusWidget = focusWidget();
        updatePosition();
        m_inputPanel->show();
        return true;
    } else if (event->type() == QEvent::CloseSoftwareInputPanel) {
        m_inputPanel->hide();
        return true;
    }
    return false;
}

QString InputPanelContext::identifierName()
{
    return "InputPanelContext";
}

QString InputPanelContext::language()
{
    return "en_US";
}

bool InputPanelContext::isComposing() const
{
    return false;
}

void InputPanelContext::reset()
{

}

void InputPanelContext::sendCharacter(QString text_context)
{
    QPointer<QWidget> w = m_focusWidget;

    qDebug() << "InputPanelContext::sendCharacter 1";
    if (!w)
        return;

    //QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
    //QApplication::sendEvent(w, &keyPress);

//    if (!w)
//        return;

//    QKeyEvent keyRelease(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString());
//    QApplication::sendEvent(w, &keyRelease);
    QLineEdit *edit  = qobject_cast<QLineEdit*>(w);
    qDebug() << "InputPanelContext::sendCharacter 2" << (int)edit;
    if(edit)
        edit->setText(text_context);
}

void InputPanelContext::updatePosition()
{
    QWidget *widget = focusWidget();
    if (!widget)
        return;
    QLineEdit* edit = qobject_cast<QLineEdit*>(widget);
    if(edit)
        m_inputPanel->setStartupText(edit->text());
    QRect widgetRect = widget->rect();
    QPoint widgetPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
//    panelPos = widget->mapToGlobal(panelPos);
    QPoint panelPos;
    panelPos.setX(0);
    panelPos.setY(200);
//    if(widgetPos.y() < 240)
//        panelPos.setY(240);
//    else
//        panelPos.setY(0);

    m_inputPanel->move(widgetPos);
}
