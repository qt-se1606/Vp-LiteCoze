#include "tcpclientsocket.h"
#include "cmd.h"
#include <QDataStream>
#include "tcpserver.h"
#include "tcpuser.h"
#include "tcpgroup.h"
#include "tcpmoodmsg.h"
#include "clientuser.h"
#include <QDebug>

TcpClientSocket::TcpClientSocket(QObject *parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

void TcpClientSocket::dataRegisterSend()
{
    serverRegisterAnswerStruct registerAnswer;
//    registerAnswer.result = m_registerAnswer;
    QByteArray result = m_registerAnswer.toLocal8Bit();
    char *resultchar = result.data();
    strcpy(registerAnswer.result, resultchar);
    //构造数据包
    qint64 totalBytes = 2*sizeof(qint64) + sizeof(serverRegisterAnswerStruct);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    //向缓冲区写入文件头
    sendOut<<totalBytes<<qint64(Register_answer);
    mutex.lock();
    memcpy(outBlock.data() + 2*sizeof(qint64),&registerAnswer,sizeof(serverRegisterAnswerStruct));
    mutex.unlock();
    write(outBlock);
    outBlock.resize(0);
}

void TcpClientSocket::dataLoginSend()
{
    serverLoginAnswerStruct loginAnswer;
    loginAnswer.sureLogin = m_sureLogin;
    //构造数据包
    qint64 totalBytes = 2*sizeof(qint64) + sizeof(serverLoginAnswerStruct);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    //向缓冲区写入文件头
    sendOut<<totalBytes<<qint64(Login_answer);
    mutex.lock();
    memcpy(outBlock.data() + 2*sizeof(qint64),&loginAnswer,sizeof(serverLoginAnswerStruct));
    mutex.unlock();
    write(outBlock);
    outBlock.resize(0);
}

void TcpClientSocket::dataAddGroupSend(QString groupid, QString groupname)
{
    serverAddGroupStruct addGroupData;
//    addGroupData.id = groupid;
//    addGroupData.name = groupname;
    QByteArray id = groupid.toLocal8Bit();
    char *idchar = id.data();
    strcpy(addGroupData.id, idchar);

    QByteArray name = groupname.toLocal8Bit();
    char *namechar = name.data();
    strcpy(addGroupData.name, namechar);
    //构造数据包
    qint64 totalBytes = 2*sizeof(qint64) + sizeof(serverAddGroupStruct);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    //向缓冲区写入文件头
    sendOut<<totalBytes<<qint64(Add_group);
    mutex.lock();
    memcpy(outBlock.data() + 2*sizeof(qint64),&addGroupData,sizeof(serverAddGroupStruct));
    mutex.unlock();
    write(outBlock);
    outBlock.resize(0);
}

void TcpClientSocket::dataAddMoodSend(QString username, QString msg)
{
    serverAddMoodStruct addMoodData;
    QByteArray name = username.toLocal8Bit();
    char *namechar = name.data();
    strcpy(addMoodData.user_name, namechar);

    QByteArray moodMsg = msg.toLocal8Bit();
    char *msgchar = moodMsg.data();
    strcpy(addMoodData.msg, msgchar);
    //构造数据包
    qint64 totalBytes = 2*sizeof(qint64) + sizeof(serverAddMoodStruct);
    QByteArray outBlock;
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    outBlock.resize(totalBytes);
    //向缓冲区写入文件头
    sendOut<<totalBytes<<qint64(Add_mood);
    mutex.lock();
    memcpy(outBlock.data() + 2*sizeof(qint64),&addMoodData,sizeof(serverAddMoodStruct));
    mutex.unlock();
    write(outBlock);
    outBlock.resize(0);
}

void TcpClientSocket::doRegister(QString registername, QString registerpower)
{
    for(auto temp : _tcpserver->tcpusers())
    {

        if(temp->name() == registername)
        {
            m_registerAnswer = "用户名重复";
            dataRegisterSend();
            return;
        }
    }
    m_registerAnswer = "注册成功";

    emit doAddRegister(registername, registerpower);

    dataRegisterSend();
}

void TcpClientSocket::doLogin(QString loginname, QString loginpower)
{
    for(auto temp : _tcpserver->tcpusers())
    {
        if(temp->name() == loginname && temp->powerpoint() == loginpower)
        {
            m_sureLogin = true;
            dataLoginSend();

            _clientUser = new ClientUser(loginname, loginpower);
            //
            return;
        }
    }
    m_sureLogin = false;
    dataLoginSend();
}

void TcpClientSocket::flush()
{
    //刷新聊天室
    for(auto temp : _tcpserver->tcpgroups())
    {
        dataAddGroupSend(temp->id(), temp->name());
    }
}

void TcpClientSocket::flushMoodPage()
{
    for(auto temp : _tcpserver->tcpmoodmsg())
    {
        dataAddMoodSend(temp->username(), temp->msg());
    }
}

void TcpClientSocket::dataReceived()
{
    qDebug() << "data received()";
    if(bytesAvailable() <= 0)
    {
        return;
    }

    //从缓存区中去除数据，但是不确定取出来的字节数
    QByteArray  buffer;
    buffer = readAll();
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
        case Register:
        {
            qDebug() << "received register";

            registerStruct registerData;
            QByteArray realStateData = m_buffer.mid(2*sizeof(qint64),totalBytes-2*sizeof(qint64));
            motionMutex.lock();
            memcpy(&registerData,realStateData.data(),sizeof(registerStruct));
            motionMutex.unlock();

            QString name = QString::fromLocal8Bit(registerData.name);
            QString power = QString::fromLocal8Bit(registerData.powerpoint);
            doRegister(name, power);

            break;
        }
        case Connet_Success:
        {
            flush();
            flushMoodPage();
            break;
        }
        case Login:
        {
            qDebug() << "received login";

            loginStruct loginData;
            QByteArray realStateData = m_buffer.mid(2*sizeof(qint64),totalBytes-2*sizeof(qint64));
            motionMutex.lock();
            memcpy(&loginData,realStateData.data(),sizeof(loginStruct));
            motionMutex.unlock();

            QString name = QString::fromLocal8Bit(loginData.name);
            QString power = QString::fromLocal8Bit(loginData.powerpoint);
            doLogin(name, power);

            break;
        }
        case AddGroup:
        {
            addGroupStruct addGroupData;
            QByteArray realStateData = m_buffer.mid(2*sizeof(qint64),totalBytes-2*sizeof(qint64));
            motionMutex.lock();
            memcpy(&addGroupData,realStateData.data(),sizeof(addGroupStruct));
            motionMutex.unlock();

            QString name = QString::fromLocal8Bit(addGroupData.name);
            emit doAddGroup(name);

            break;
        }
        case AddMood:
        {
            addMoodStruct addMoodData;
            QByteArray realStateData = m_buffer.mid(2*sizeof(qint64),totalBytes-2*sizeof(qint64));
            motionMutex.lock();
            memcpy(&addMoodData,realStateData.data(),sizeof(addMoodStruct));
            motionMutex.unlock();

            QString name = QString::fromLocal8Bit(addMoodData.user_name);
            QString msg = QString::fromLocal8Bit(addMoodData.msg);
            emit doAddMood(name, msg);

            break;
        }
        case OpenMoodPage:
        {
            flushMoodPage();
            break;
        }
        case Msg:
        {
            dataChatMsgStruct dataChatData;
            QByteArray realStateData = m_buffer.mid(2*sizeof(qint64),totalBytes-2*sizeof(qint64));
            motionMutex.lock();
            memcpy(&dataChatData,realStateData.data(),sizeof(dataChatMsgStruct));
            motionMutex.unlock();

            QString name = QString::fromLocal8Bit(dataChatData.user_name);
            QString id = QString::fromLocal8Bit(dataChatData.group_id);
            QString msg = QString::fromLocal8Bit(dataChatData.msg);
            emit doSendChatData(name, id, msg);

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

void TcpClientSocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}
