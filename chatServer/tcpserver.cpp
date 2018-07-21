#include "tcpserver.h"
#include "tcpclientsocket.h"
#include "tcpgroup.h"
#include "tcpuser.h"
#include "tcpmoodmsg.h"
#include "cmd.h"
#include <QDataStream>
#include <iostream>
#include <QDebug>

TcpServer::TcpServer(QObject *parent, quint16 port)
    :QTcpServer(parent)
{
    if(!listen(QHostAddress::Any,port))
    {
        qDebug() << "do not listen";
    }
    connectDatabase();
    qDebug() << "start\n";
}

//出现一个新的连接时触发

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    TcpClientSocket *tcpClientSocket = new TcpClientSocket(this);
    tcpClientSocket->linkServer(this);

    connect(tcpClientSocket, SIGNAL(doAddGroup(QString)), this, SLOT(doAddGroup(QString)));
    connect(tcpClientSocket, SIGNAL(doAddMood(QString,QString)), this, SLOT(doAddMood(QString,QString)));
    connect(tcpClientSocket, SIGNAL(doAddRegister(QString,QString)), this, SLOT(doAddRegister(QString,QString)));
    connect(tcpClientSocket, SIGNAL(doSendChatData(QString,QString,QString)), this, SLOT(updateMsgClients(QString,QString,QString)));
    connect(tcpClientSocket, SIGNAL(disconnected(qintptr)), this, SLOT(slotDisconnection(qintptr)));

    tcpClientSocket->setSocketDescriptor(socketDescriptor);

    qDebug() << "connect" ;
    qDebug() << socketDescriptor;

    tcpClientSocketList.append(tcpClientSocket);
}

void TcpServer::dataAddGroupSend(QString groupid, QString groupname)
{
    serverAddGroupStruct addGroupData;
    QByteArray id = groupid.toLocal8Bit();
    char *idchar = id.data();
    strcpy(addGroupData.id, idchar);

    QByteArray name = groupname.toLocal8Bit();
    char *namechar = name.data();
    strcpy(addGroupData.name, namechar);
    for(int i=0; i<tcpClientSocketList.count(); i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);

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
        item->write(outBlock);
        outBlock.resize(0);
    }
}

void TcpServer::dataAddMoodSend(QString username, QString msg)
{
    serverAddMoodStruct addMoodData;
    QByteArray name = username.toLocal8Bit();
    char *namechar = name.data();
    strcpy(addMoodData.user_name, namechar);

    QByteArray moodMsg = msg.toLocal8Bit();
    char *msgchar = moodMsg.data();
    strcpy(addMoodData.msg, msgchar);
    for(int i=0; i<tcpClientSocketList.count(); i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);

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
        item->write(outBlock);
        outBlock.resize(0);
    }
}

void TcpServer::updateMsgClients(QString username, QString groupid, QString msg)
{
    serverMsgStruct msgData;
//    msgData.group_id = groupid;
//    msgData.msg_data = msg;
//    msgData.user_name = username;

    QByteArray id = groupid.toLocal8Bit();
    char *idchar = id.data();
    strcpy(msgData.group_id, idchar);

    QByteArray msg_data = msg.toLocal8Bit();
    char *msgchar = msg_data.data();
    strcpy(msgData.msg_data, msgchar);

    QByteArray name = username.toLocal8Bit();
    char *namechar = name.data();
    strcpy(msgData.user_name, namechar);
    for(int i=0; i<tcpClientSocketList.count(); i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);

        //构造数据包
        qint64 totalBytes = 2*sizeof(qint64) + sizeof(serverMsgStruct);
        QByteArray outBlock;
        QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
        sendOut.setVersion(QDataStream::Qt_5_2);
        outBlock.resize(totalBytes);
        //向缓冲区写入文件头
        sendOut<<totalBytes<<qint64(Msg_send);
        mutex.lock();
        memcpy(outBlock.data() + 2*sizeof(qint64),&msgData,sizeof(serverMsgStruct));
        mutex.unlock();
        item->write(outBlock);
        outBlock.resize(0);
    }
}

void TcpServer::doAddGroup(QString groupname)
{
    int i = 0;
    for(auto temp : tcpgroups())
    {
        i++;
    }

    ++i;
    db.open();
    QSqlQuery query;
    QString sql = "insert into chatgroup values('" + QString::number(i) + "', '" + groupname + "')";
    query.exec(sql);

    //发送给客户端
    dataAddGroupSend(QString::number(i), groupname);
}

void TcpServer::doAddMood(QString username, QString msg)
{
    db.open();
    QSqlQuery query;
    QString sql = "insert into moodmsg values('" + username + "', '" + msg + "')";
    query.exec(sql);

    //发送给客户端
    dataAddMoodSend(username, msg);
}

void TcpServer::clientConnect()
{
    std::cout << "connect\n" << std::endl;
}

void TcpServer::slotDisconnection(qintptr descriptor)
{
    for(int i=0; i<tcpClientSocketList.count(); i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor() == descriptor)
        {
            tcpClientSocketList.removeAt(i);
            qDebug() << "remove socket";
            qDebug() << descriptor;
            return;
        }
    }
    return;
}

void TcpServer::doAddRegister(QString regisername, QString registerpower)
{
    db.open();
    QSqlQuery query;
    QString sql = "insert into user values('" + regisername + "', '" + registerpower + "')";
    query.exec(sql);
}

std::vector<TcpUser *> TcpServer::tcpusers()
{
    //删除以前的vector
    for(auto &temp : _tcpusers)
    {
        delete temp;
    }
    _tcpusers.clear();

    db.open();

    QSqlQuery query;
    query.exec("select * from user");
    while (query.next()) {
        QString name = query.value(0).toString();
        QString powerpoint = query.value(1).toString();
        _tcpusers.push_back(new TcpUser(name, powerpoint));
    }
    return _tcpusers;
}

std::vector<TcpGroup *> TcpServer::tcpgroups()
{
    //删除以前的vector
    for(auto &temp : _tcpgroups)
    {
        delete temp;
    }
    _tcpgroups.clear();

    db.open();

    QSqlQuery query;
    query.exec("select * from chat"
               "group");
    while (query.next()) {
        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        _tcpgroups.push_back(new TcpGroup(id, name));
    }
    return _tcpgroups;
}

std::vector<TcpMoodMsg *> TcpServer::tcpmoodmsg()
{
    //删除以前的vector
    for(auto &temp : _tcpmoodmsg)
    {
        delete temp;
    }
    _tcpmoodmsg.clear();

    db.open();

    QSqlQuery query;
    query.exec("select * from moodmsg");
    while (query.next()) {
        QString name = query.value(0).toString();
        QString msg = query.value(1).toString();
        _tcpmoodmsg.push_back(new TcpMoodMsg(name, msg));
    }
    return _tcpmoodmsg;
}



void TcpServer::connectDatabase()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("172.96.214.241");
    db.setPort(3306);
    db.setDatabaseName("tcpchat");
    db.setUserName("tcpchat");
    db.setPassword("tcpchat");

    QSqlQuery query;
    if(db.open())
    {
        std::cout << "datebases connected\n" << std::endl;
        query.exec(QObject::tr("create table user (name varchar(50) primary key, powerpoint varchar(20))"));
        query.exec(QObject::tr("create table chatgroup (id varchar(20) primary key, name varchar(50))"));
        query.exec(QObject::tr("create table groupidmax (id varchar(20) primary key)"));
        query.exec(QObject::tr("create table moodmsg (name varchar(50), msg varchar(2048))"));
    }else {
        printf("failure opent database\n");
    }
}


