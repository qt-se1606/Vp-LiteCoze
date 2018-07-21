#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H
#include <QTcpSocket>
#include <QObject>
#include "tcpserver.h"
#include <QMutex>

class TcpServer;
class ClientUser;

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpClientSocket(QObject *parent=0);
    void linkServer(TcpServer* tcpserver)
    {
        _tcpserver = tcpserver;
    }

    void dataRegisterSend();
    void dataLoginSend();
//    void dataChatMsgSend();
    void dataAddGroupSend(QString groupid, QString groupname);
    void dataAddMoodSend(QString username, QString msg);


    void doRegister(QString registername, QString registerpower);   //注册的函数
    void doLogin(QString loginname, QString loginpower);
    void flush();      //登录后第一次进入主界面刷新
    void flushMoodPage();  //打开心情面板时刷新


    //传回客户端的结构体
    struct serverRegisterAnswerStruct
    {
        char result[1024];
    };
    struct serverLoginAnswerStruct
    {
        bool sureLogin;
    };
    struct serverAddGroupStruct
    {
        char id[1024];
        char name[1024];
    };
    struct serverAddMoodStruct
    {
        char user_name[1024];
        char msg[2048];
    };

    //接收来自客户端的结构体
    struct registerStruct
    {
        char name[1024];
        char powerpoint[1024];
    };
    struct loginStruct
    {
        char name[1024];
        char powerpoint[1024];
    };
    struct addGroupStruct
    {
        char name[1024];
    };
    struct addMoodStruct
    {
        char user_name[1024];
        char msg[2048];
    };
    struct dataChatMsgStruct
    {
        char user_name[1024];
        char group_id[1024];
        char msg[1024];
    };


signals:
    void doAddGroup(QString groupname);
    void doAddMood(QString username, QString msg);
    void doAddRegister(QString registername, QString registerpower);
    void doSendChatData(QString username, QString groupid, QString msg);

    void disconnected(qintptr);
private slots:
    void dataReceived();
    void slotDisconnected();
private:
    TcpServer* _tcpserver;
    QString m_registerAnswer;
    bool m_sureLogin;   //注册是否成功
    ClientUser* _clientUser;

    QMutex mutex;
    QMutex motionMutex;
    QByteArray m_buffer;    //缓存上一次或多次的未处理的数据
    qint64 totalBytes;      //一个数据包MSG部分的完整大小
    qint64 recvdBytes;      //已经收到的字节数
    qint64 serverCmd;       //接受数据包的类型
    QByteArray inBlock;     //接受缓冲
};

#endif // TCPCLIENTSOCKET_H
