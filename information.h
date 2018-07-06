#ifndef INFORMATION_H
#define INFORMATION_H

#include <QLabel>
#include <QObject>
#include <QFlag>
#include <QElapsedTimer>

class Information
{
public:
    Information();
    static Information *instance();

    enum LoginStatusEnum {
        NoLogin = 0x00,
        EmployeeLogin = 0x01,
        DepartmentLogin = 0x02,
        AllLogin = 0x03
    };
    Q_DECLARE_FLAGS(LoginStatus,LoginStatusEnum)


    void addLoginLabel(QLabel* label);
    void addDepartmentLabel(QLabel* label);
    void addDescription(QLabel* label);

    void setLonginName(const QString& name);
    void setDepartmentName(const QString& name);

    QString department() { return m_departmentName; }
    QString employee() { return m_loginName; }
    LoginStatus loginStatus();

private:
    static Information* s_instance;



    QString m_loginName;
    QString m_departmentName;

    QList<QLabel*> m_loginLabels;
    QList<QLabel*> m_departmentLabels;
    QList<QLabel*> m_descriptionLabels;
    void settingDescriptionLabels();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Information::LoginStatus)

#endif // INFORMATION_H
