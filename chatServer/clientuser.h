#ifndef CLIENTUSER_H
#define CLIENTUSER_H
#include <QString>

class ClientUser
{
public:
    ClientUser(QString username, QString userpwer):m_username(username), m_userpowerpoint(userpwer)
    {

    }

    QString username(){
        return m_username;
    }
    QString userpowerpoint(){
        return m_userpowerpoint;
    }
    QString msg(){
        return m_msg;
    }
private:
    QString m_username;
    QString m_userpowerpoint;
    QString m_msg;
};

#endif // CLIENTUSER_H
