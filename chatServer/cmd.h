#ifndef CMD_H
#define CMD_H

//服务器------->客户端
#define     Login_answer           0x0F01   //登陆结果
#define     QString_send           0x0F02   //发送字符串
#define     QFile_send             0x0F03   //发送文件
#define     Struct_send            0x0F03   //发送结构体
#define     Register_answer        0x0F04   //注册结果
#define     Msg_send               0x0F05   //发送的信息
#define     Add_group              0x0F06   //添加聊天室
#define     Add_mood               0x0F07   //add mood

//客户端------->服务器
#define     Login                  0x0D00   //客户端登陆
#define     Register               0x0D01   //客户端注册
#define     AddGroup               0x0D02   //客户端添加房间
#define     Msg                    0x0D03   //客户端发送聊天信息
#define     Connet_Success         0x0D04   //连接成功应答包
#define     AddMood                0x0D05   //mood packet
#define     OpenMoodPage           0x0D06   //

#endif // CMD_H
