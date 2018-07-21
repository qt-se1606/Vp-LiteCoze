#ifndef TCPGROUP_H
#define TCPGROUP_H
#include <QString>

class TcpGroup
{
public:
    TcpGroup(QString id, QString name);
    QString id(){
        return m_id;
    }
    QString name(){
        return m_name;
    }
private:
    QString m_id;
    QString m_name;
};

#endif // TCPGROUP_H
