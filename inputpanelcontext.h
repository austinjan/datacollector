#ifndef INPUTPANELCONTEXT_H
#define INPUTPANELCONTEXT_H

#include <QInputContext>
#include "inputpanel.h"

class InputPanelContext : public QInputContext
{
    Q_OBJECT
public:
    InputPanelContext();
    bool filterEvent(const QEvent* event);

    QString identifierName();
    QString language();

    bool isComposing() const;

    void reset();

   private slots:
       void sendCharacter(QString character);

   private:
       void updatePosition();

   private:
       InputPanel *m_inputPanel;
       QWidget *m_focusWidget;
};

#endif // INPUTPANELCONTEXT_H
