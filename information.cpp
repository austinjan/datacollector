#include "information.h"

Information* Information::s_instance = 0;

Information::Information()
{

}

Information *Information::instance()
{
    if (!s_instance)
    {
        s_instance = new Information();
    }
    return s_instance;
}

void Information::addLoginLabel(QLabel *label)
{
    m_loginLabels.append(label);
}

void Information::addDepartmentLabel(QLabel *label)
{
    m_departmentLabels.append(label);
}

void Information::addDescription(QLabel *label)
{
    m_descriptionLabels.append(label);
}

void Information::settingDescriptionLabels()
{
    QString description = QString("Department: %1 Login: %2")
            .arg(m_departmentName)
            .arg(m_loginName);
    foreach(QLabel* label, m_descriptionLabels)
    {
        label->setText(description);
    }
}

void Information::setLonginName(const QString &name)
{
    m_loginName = name;
    foreach(QLabel* label, m_loginLabels)
    {
        if(name.isEmpty())
            label->setText("No Login");
        else
            label->setText(name);
    }

    settingDescriptionLabels();
}

void Information::setDepartmentName(const QString &name)
{
    m_departmentName = name;
    foreach(QLabel* label, m_departmentLabels)
    {
        if(name.isEmpty())
            label->setText("No Department");
        else
            label->setText(name);
    }

    settingDescriptionLabels();
}

Information::LoginStatus Information::loginStatus()
{
    LoginStatus ret;
    if(!m_departmentName.isEmpty() && m_departmentName !="No Department")
    {
        ret |= Information::DepartmentLogin;
    }

    if(!m_loginName.isEmpty() && m_loginName !="No Login")
    {
        ret |= Information::EmployeeLogin;
    }
    return ret;
}

