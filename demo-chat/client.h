//谢逸青
#ifndef CLIENT_H
#define CLIENT_H
#include <QHostAddress>
#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QMutex>

class Client : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString userPower READ userPower WRITE setUserPower NOTIFY userPowerChanged)
    Q_PROPERTY(QString groupId READ groupId WRITE setGroupId NOTIFY groupIdChanged)
    Q_PROPERTY(QString groupName READ groupName WRITE setGroupName NOTIFY groupNameChanged)
    Q_PROPERTY(QString registerName READ registerName WRITE setRegisterName NOTIFY registerNameChanged)
    Q_PROPERTY(QString registerPower READ registerPower WRITE setRegisterPower NOTIFY registerPowerChanged)
public:
    Client(QObject *parent = 0);
    QString userId() const;
    void setUserId(const QString &userId);

    QString userName() const;
    void setUserName(const QString &userName);

    QString userPower() const;
    void setUserPower(const QString &userPower);

    QString groupId() const;
    void setGroupId(const QString &groupId);

    Q_INVOKABLE void dataRegisterSend();
    Q_INVOKABLE void dataLoginSend();
    Q_INVOKABLE void dataChatMsgSend(QString groupid, QString username, QString chatmsg);
    Q_INVOKABLE void dataAddGroupSend(QString group_name);
    Q_INVOKABLE void dataConnectSend();
    Q_INVOKABLE void dataAddMoodSend(QString msg);
    Q_INVOKABLE void dataOpenMoodpage();

    //连接服务端
    void tcpConnected();

    QString groupName() const;
    void setGroupName(const QString &groupName);

    QString registerName() const;
    void setRegisterName(const QString &registerName);

    QString registerPower() const;
    void setRegisterPower(const QString &registerPower);

    //客户端发出去的结构体
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
    struct connectSuccessStruct
    {
        char str[1024];
    };

    //服务端发来的结构体
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
    struct serverMsgStruct
    {
        char user_name[1024];
        char group_id[1024];
        char msg_data[1024];
    };

signals:
    void userIdChanged();
    void userNameChanged();
    void userPowerChanged();
    void groupIdChanged();
    void groupNameChanged();
    void registerNameChanged();
    void registerPowerChanged();


    void getLoginAnswer(bool sureLogin);      //得到登录的反馈信息
    void getRegisterAnswer(QString registerAnswer);   //得到注册的反馈信息
    void signalAddGroup(QString group_id, QString group_name);     //得到添加聊天室的信息
    void signalMsg(QString user_name, QString group_id, QString msg);  //得到聊天信息的信息
    void signalAddMood(QString user_name, QString msg);    //get add mood's message
private slots:
    void dataReceived();     //收到来自服务器的数据
    void sureConnected();
private:
    QString m_userId;
    QString m_userName;
    QString m_userPower;
    QString m_groupId;
    QString m_groupName;
    QString m_registerName;
    QString m_registerPower;

    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;
    QString m_chatMsg;
    QMutex mutex;
    QMutex motionMutex;
    QByteArray m_buffer;    //缓存上一次或多次的未处理的数据
    qint64 totalBytes;      //一个数据包MSG部分的完整大小
    qint64 recvdBytes;      //已经收到的字节数
    qint64 serverCmd;       //接受数据包的类型
    QByteArray inBlock;     //接受缓冲

    void connected();
};

#endif // CLIENT_H
