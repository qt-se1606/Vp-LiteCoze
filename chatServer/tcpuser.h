#ifndef TCPUSER_H
#define TCPUSER_H
#include <QString>

class TcpUser
{
public:
    TcpUser(QString name, QString powerpoint);
    QString name(){
        return m_name;
    }
    QString powerpoint(){
        return m_powerpoint;
    }
private:
    QString m_name;
    QString m_powerpoint;
};

#endif // TCPUSER_H
