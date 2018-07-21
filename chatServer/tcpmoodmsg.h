#ifndef TCPMOODMSG_H
#define TCPMOODMSG_H
#include <QString>

class TcpMoodMsg
{
public:
    TcpMoodMsg(QString username, QString msg);

    QString username(){
        return m_username;
    }
    QString msg(){
        return m_msg;
    }
private:
    QString m_username;
    QString m_msg;
};

#endif // TCPMOODMSG_H
