#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <vector>
#include <QMutex>

class TcpClientSocket;
class TcpUser;
class TcpGroup;
class TcpMoodMsg;

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent=0,quint16 port=8010);


    //传回客户端的结构体
    struct serverAddGroupStruct
    {
        char id[1024];
        char name[1024];
    };
    struct serverMsgStruct
    {
        char user_name[1024];
        char group_id[1024];
        char msg_data[1024];
    };
    struct serverAddMoodStruct
    {
        char user_name[1024];
        char msg[2048];
    };


    //传送msg给客户端
//    void dataMsgSend(QString username, QString msg, QString groupid);
    //传送addgroup给客户端
    void dataAddGroupSend(QString groupid, QString groupname);   //广播给客户端
    void dataAddMoodSend(QString username, QString msg);

    std::vector<TcpUser*> tcpusers();
    std::vector<TcpGroup*> tcpgroups();
    std::vector<TcpMoodMsg*> tcpmoodmsg();

    void incomingConnection(qintptr socketDescriptor);
public slots:
    //接受要广播的聊天信息
    void updateMsgClients(QString username, QString groupid, QString msg);

    //接收断开连接的信息
    void slotDisconnection(qintptr);

    void doAddRegister(QString regisername, QString registerpower);
    void doAddGroup(QString groupname);  //添加房间到数据库并发送给客户端
    void doAddMood(QString username, QString msg);  //添加心情到数据库并发送给客户端

    void clientConnect();


private:
    QSqlDatabase db;
    QList<TcpClientSocket*> tcpClientSocketList;

    std::vector<TcpUser*> _tcpusers;
    std::vector<TcpGroup*> _tcpgroups;
    std::vector<TcpMoodMsg*> _tcpmoodmsg;

    void connectDatabase();


    QMutex mutex;
    QMutex motionMutex;
    QByteArray m_buffer;    //缓存上一次或多次的未处理的数据
    qint64 totalBytes;      //一个数据包MSG部分的完整大小
    qint64 recvdBytes;      //已经收到的字节数
    qint64 serverCmd;       //接受数据包的类型
    QByteArray inBlock;     //接受缓冲
protected:

};

#endif // TCPSERVER_H
