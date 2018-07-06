#ifndef INPUTPANEL_H
#define INPUTPANEL_H

#include <QSignalMapper>
#include <QWidget>

#define INPUTPANEL_CHAR_PORTRAIT             1
#define INPUTPANEL_CHAR_PORTRAIT_WIDE        2
#define INPUTPANEL_CHAR_PORTRAIT_EXT_WIDE    3
#define INPUTPANEL_CHAR                      4
#define INPUTPANEL_CHAR_WIDE                 5
#define INPUTPANEL_CHAR_EXT_WIDE             6
#define INPUTPANEL_CHAR_NUM                  7
#define INPUTPANEL_NUM_HEX                   8
#define INPUTPANEL_NUM_DEC                   9
#define INPUTPANEL_NUM_OCT                   10
#define INPUTPANEL_NUM_BCD                   11



#define KEY_ENT "ENT"
#define KEY_ALT "ALT"
#define KEY_CLR "CLR"
#define KEY_DEL "DEL"
#define KEY_ESC "ESC"
#define KEY_Ampersand "&&"
#define KEY_SPACE "& "
#define KEY_CAP "CAP"
#define KEY_TAB "TAB"
#define KEY_DEC_POINT "."

namespace Ui {
class InputPanel;
}

class QPushButton;

class InputPanel : public QWidget
{
    Q_OBJECT

public:
    explicit InputPanel(QWidget *parent = 0);
    ~InputPanel();

    void setStartupText(const QString& text);

signals:
    void characterGenerated(QString character);

private slots:
    void saveFocusWidget(QWidget *oldFocus, QWidget *newFocus);
    void buttonClicked(QWidget *w);


protected:
    bool event(QEvent *e);
    void setSignalMap();
    QString keyValue(QPushButton* pushBtn);
    void transformKeyboard();
    void changePage();

private:
    Ui::InputPanel *ui;
    QWidget *lastFocusedWidget;
    QSignalMapper signalMapper;
};

#endif // INPUTPANEL_H
