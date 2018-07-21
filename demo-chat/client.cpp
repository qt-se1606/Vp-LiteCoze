#include "client.h"
#include <QDataStream>
#include "cmd.h"
#include <QQuickView>
#include <QQuickItem>
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent)
{
    tcpConnected();

//    QQuickView view(QUrl(QStringLiteral("qml/Main.qml")));
//    QQuickItem * item = view.rootObject();
//    QObject * login = item->findChild<QObject*>("loginpage");
//    connect(this, SIGNAL(getLoginAnswer(bool)), login, SIGNAL(loginAnwser(bool)));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
}

QString Client::userId() const
{
    return m_userId;
}

void Client::setUserId(const QString &userId)
{
    if(m_userId != userId)
    {
        m_userId = userId;
        emit userIdChanged();
    }
}

QString Client::userName() const
{
    return m_userName;
}

void Client::setUserName(const QString &userName)
{
    if(m_userName != userName)
    {
        m_userName = userName;
        emit userNameChanged();
    }
}

QString Client::userPower() const
{
    return m_userPower;
}

void Client::setUserPower(const QString &userPower)
{
    if(m_userPower != userPower)
    {
        m_userPower = userPower;
        emit userPowerChanged();
    }
}

QString Client::groupId() const
{
    return m_groupId;
}

void Client::setGroupId(const QString &groupId)
{
    if(m_groupId != groupId)
    {
        m_groupId = groupId;
        emit groupIdChanged();
    }
}

void Client::dataRegisterSend()
{
    registerStruct registerData;
//    registerData.name = m_registerName;
//    registerData.powerpoint = m_registerPower;
    QByteArray name = m_registerName.toLocal8Bit();
    char *namechar = name.data();
    strcpy(registerData.name, namechar);
    QByteArray power = m_registerPower.toLocal8Bit();
    char *powerchar = power.data();
    strcpy(registerData.powerpoint, powerchar);
    //构造数据包
    qint64 totalBytes = 2*sizeof(qint64) + sizeof(registerStruct);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    //向缓冲区写入文件头
    sendOut<<totalBytes<<qint64(Register);
    mutex.lock();
    memcpy(outBlock.data() + 2*sizeof(qint64),&registerData,sizeof(registerStruct));
    mutex.unlock();
    tcpSocket->write(outBlock);
    outBlock.resize(0);
}

void Client::dataLoginSend()
{
    qDebug() << "loginsend";

    loginStruct loginData;
    QByteArray name = m_userName.toLocal8Bit();
    char*namechar = name.data();
    strcpy(loginData.name,namechar);
    QByteArray power = m_userPower.toLocal8Bit();
    char *powerchar = power.data();
    strcpy(loginData.powerpoint, powerchar);
    //构造数据包
    qint64 totalBytes = 2*sizeof(qint64) + sizeof(loginStruct);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    //向缓冲区写入文件头
    sendOut<<totalBytes<<qint64(Login);
    mutex.lock();
    memcpy(outBlock.data() + 2*sizeof(qint64),&loginData,sizeof(loginStruct));
    mutex.unlock();
    tcpSocket->write(outBlock);
    outBlock.resize(0);
}

void Client::dataChatMsgSend(QString groupid, QString username, QString chatmsg)
{
    dataChatMsgStruct dataChatMsgData;
//    dataChatMsgData.group_id = m_groupId;
//    dataChatMsgData.user_name = m_userName;
//    dataChatMsgData.msg = m_chatMsg;
    QByteArray id = groupid.toLocal8Bit();
    char *idchar = id.data();
    strcpy(dataChatMsgData.group_id, idchar);

    QByteArray name = username.toLocal8Bit();
    char *namechar = name.data();
    strcpy(dataChatMsgData.user_name, namechar);

    QByteArray msg = chatmsg.toLocal8Bit();
    char *msgchar = msg.data();
    strcpy(dataChatMsgData.msg, msgchar);
    //构造数据包
    qint64 totalBytes = 2*sizeof(qint64) + sizeof(dataChatMsgStruct);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    //向缓冲区写入文件头
    sendOut<<totalBytes<<qint64(Msg);
    mutex.lock();
    memcpy(outBlock.data() + 2*sizeof(qint64),&dataChatMsgData,sizeof(dataChatMsgStruct));
    mutex.unlock();
    tcpSocket->write(outBlock);
    outBlock.resize(0);
}

void Client::dataAddGroupSend(QString group_name)
{
    addGroupStruct addGroupData;

    QByteArray name = group_name.toLocal8Bit();
    char *namechar = name.data();
    strcpy(addGroupData.name, namechar);

    //构造数据包
    qint64 totalBytes = 2*sizeof(qint64) + sizeof(addGroupStruct);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    //向缓冲区写入文件头
    sendOut<<totalBytes<<qint64(AddGroup);
    mutex.lock();
    memcpy(outBlock.data() + 2*sizeof(qint64),&addGroupData,sizeof(addGroupStruct));
    mutex.unlock();
    tcpSocket->write(outBlock);
    outBlock.resize(0);
}

void Client::dataAddMoodSend(QString msg)
{
    addMoodStruct addMoodData;

    QByteArray name = m_userName.toLocal8Bit();
    char *namechar = name.data();
    strcpy(addMoodData.user_name, namechar);

    QByteArray moodMsg = msg.toLocal8Bit();
    char *msgchar = moodMsg.data();
    strcpy(addMoodData.msg, msgchar);

    //构造数据包
    qint64 totalBytes = 2*sizeof(qint64) + sizeof(addMoodStruct);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    //向缓冲区写入文件头
    sendOut<<totalBytes<<qint64(AddMood);
    mutex.lock();
    memcpy(outBlock.data() + 2*sizeof(qint64),&addMoodData,sizeof(addMoodStruct));
    mutex.unlock();
    tcpSocket->write(outBlock);
    outBlock.resize(0);
}

void Client::dataOpenMoodpage()
{
    qint64 totalBytes = 2*sizeof(qint64);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    sendOut<<totalBytes<<qint64(OpenMoodPage);
    tcpSocket->write(outBlock);
    outBlock.resize(0);
}

void Client::dataConnectSend()
{
    connectSuccessStruct connectSuce;
//    connectSuce.str = "connect success";
    strcpy(connectSuce.str, "connect success");
    //构造数据包
    qint64 totalBytes = 2*sizeof(qint64) + sizeof(connectSuccessStruct);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    //向缓冲区写入文件头
    sendOut<<totalBytes<<qint64(Connet_Success);
    mutex.lock();
    memcpy(outBlock.data() + 2*sizeof(qint64),&connectSuce,sizeof(connectSuccessStruct));
    mutex.unlock();
    tcpSocket->write(outBlock);
    outBlock.resize(0);
}

void Client::tcpConnected()
{
    tcpSocket = new QTcpSocket(this);
    serverIP =new QHostAddress();
    serverIP->setAddress("127.0.0.1");  //连接IP
//    tcpSocket->connectToHost();
    tcpSocket->connectToHost(*serverIP, 8010);

    connect(tcpSocket, SIGNAL(connected()), this, SLOT(sureConnected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(sureConnected()));
}

QString Client::groupName() const
{
    return m_groupName;
}

void Client::setGroupName(const QString &groupName)
{
    if(m_groupName != groupName)
    {
        m_groupName = groupName;
        emit groupNameChanged();
    }
}

QString Client::registerName() const
{
    return m_registerName;
}

void Client::setRegisterName(const QString &registerName)
{
    if(m_registerName != registerName)
    {
        m_registerName = registerName;
        emit registerNameChanged();
    }
}

QString Client::registerPower() const
{
    return m_registerPower;
}

void Client::setRegisterPower(const QString &registerPower)
{
    if(m_registerPower != registerPower)
    {
        m_registerPower = registerPower;
        emit registerPowerChanged();
    }
}

void Client::dataReceived()
{
    if(tcpSocket->bytesAvailable() <= 0)
    {
        return;
    }

    //从缓存区中去除数据，但是不确定取出来的字节数
    QByteArray  buffer;
    buffer = tcpSocket->readAll();
    m_buffer.append(buffer);
    unsigned int totalLen = m_buffer.size();
    //这边确实需要利用长度做while循环，因为有可能一下子读取到两条以上的完整记录，就需要进行循环处理了
    //超过一条完整小于第二条完整记录时，如果已经达到包头长度就先把包头保存下来，整个过程循环往复
    while (totalLen) {
        //与QDataStream绑定，方便操作
        QDataStream  packet(m_buffer);
        packet.setVersion(QDataStream::Qt_5_2);
        //不够包头长度的不处理，结束while循环
        if(totalLen < 2*sizeof(qint64))
            break;
        //将包头读入了进来按照定义的协议 先读命令长度，再读命令的类型
        packet>>totalBytes>>serverCmd;
        //缓存中的内容长度没有达到命令的长度，那就先结束，等足够了再来解析
        if(totalLen<totalBytes)
            break;
        //足够长了就开始解析
        switch(serverCmd)
        {
        case Login_answer:
        {
            serverLoginAnswerStruct serverLoginData;
            QByteArray realStateData = m_buffer.mid(2*sizeof(qint64),totalBytes-2*sizeof(qint64));
            motionMutex.lock();
            memcpy(&serverLoginData,realStateData.data(),sizeof(serverLoginAnswerStruct));
            motionMutex.unlock();

            emit getLoginAnswer(serverLoginData.sureLogin);

            break;
        }
        case Register_answer:
        {
            serverRegisterAnswerStruct serverRegisterData;
            QByteArray realStateData = m_buffer.mid(2*sizeof(qint64),totalBytes-2*sizeof(qint64));
            motionMutex.lock();
            memcpy(&serverRegisterData,realStateData.data(),sizeof(serverRegisterAnswerStruct));
            motionMutex.unlock();

            QString result = QString::fromLocal8Bit(serverRegisterData.result);
            emit getRegisterAnswer(result);

            break;
        }
        case Add_group:
        {
            serverAddGroupStruct serverAddGroupData;
            QByteArray realStateData = m_buffer.mid(2*sizeof(qint64),totalBytes-2*sizeof(qint64));
            motionMutex.lock();
            memcpy(&serverAddGroupData,realStateData.data(),sizeof(serverAddGroupStruct));
            motionMutex.unlock();

            QString name = QString::fromLocal8Bit(serverAddGroupData.name);
            QString id = QString::fromLocal8Bit(serverAddGroupData.id);
            emit signalAddGroup(id, name);

            break;
        }
        case Add_mood:
        {
            serverAddMoodStruct serverAddMoodData;
            QByteArray realStateData = m_buffer.mid(2*sizeof(qint64),totalBytes-2*sizeof(qint64));
            motionMutex.lock();
            memcpy(&serverAddMoodData,realStateData.data(),sizeof(serverAddMoodStruct));
            motionMutex.unlock();

            QString name = QString::fromLocal8Bit(serverAddMoodData.user_name);
            QString msg = QString::fromLocal8Bit(serverAddMoodData.msg);
            emit signalAddMood(name, msg);

            break;
        }
        case Msg_send:
        {
            serverMsgStruct serverMsgData;
            QByteArray realStateData = m_buffer.mid(2*sizeof(qint64),totalBytes-2*sizeof(qint64));
            motionMutex.lock();
            memcpy(&serverMsgData,realStateData.data(),sizeof(serverMsgStruct));
            motionMutex.unlock();

            QString user_name = QString::fromLocal8Bit(serverMsgData.user_name);
            QString group_id = QString::fromLocal8Bit(serverMsgData.group_id);
            QString msg = QString::fromLocal8Bit(serverMsgData.msg_data);
            emit signalMsg(user_name, group_id, msg);

            break;
        }
        }
        //缓存多余的数据
        buffer = m_buffer.right(totalLen - totalBytes); //截取下一个数据包的数据，留作下次读取
        totalLen = buffer.size();
        //更新多余的数据
        m_buffer = buffer;
    }
}

void Client::sureConnected()
{
    qDebug() << "have connected";
}
