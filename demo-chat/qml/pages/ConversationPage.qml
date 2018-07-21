//杜虹朋
import QtQuick 2.4
import VPlayApps 1.0

ListPage {
    title: person

    property string person

    property var newMsgs: []

    property string groupid

    property int numRepeats: 1

    property  bool ifme

    property  string msgname

    //发送聊天信息到服务端
    Connections {
        target: client
        onSignalMsg: {
            console.debug("fds")
            if(group_id === groupid)
            {
                if(user_name === client.userName)
                {
                    ifme = true
                }
                else{
                    ifme = false
                }
                msgname = user_name
                newMsgs = newMsgs.concat({text: msg, me: ifme, msg_user_name: msgname})
                listView.positionViewAtEnd()
            }
        }
    }

    model: {
        var model = newMsgs
        return model
    }

    Component.onCompleted: listView.positionViewAtEnd()

    listView.backgroundColor: "white"
    listView.anchors.bottomMargin: inputBox.height

    delegate: Item {
        id: bubble

        width: parent.width
        height: bg.height + dp(30)

        property bool me: modelData.me

        Rectangle{
            id: touxiang
            color: me ? Theme.tintColor : "pink"
            x: me ? (bubble.width - width - dp(10)) : dp(10)
            y: dp(10)
            width: dp(30)
            height: dp(30)
            radius: dp(180)
        }

        Text {
            x: me ? (bubble.width - width - dp(20) - touxiang.width - dp(7)) : (dp(20) + touxiang.width + dp(7))
            y: dp(7)
            id: usernametext
            wrapMode: Text.WordWrap
            text: modelData.msg_user_name
            font.pixelSize: sp(16)
            color: "gray"
//            color: me ? "white" : "black"
        }

        Rectangle {
            id: bg
            color: me ? Theme.tintColor : "pink"
            radius: dp(10)

            x: me ? (bubble.width - width - dp(20) - touxiang.width) : (dp(20) + touxiang.width)
            y: dp(30)
            width: innerText.width + dp(20)
            height: innerText.implicitHeight + dp(20)

            Text {
                id: innerText
                x: dp(10)
                y: dp(10)
                width: Math.min(innerText.implicitWidth, bubble.parent.width * 0.75)
                wrapMode: Text.WordWrap
                text: modelData.text
                font.pixelSize: sp(16)
                color: me ? "white" : "black"
            }
        }
    }


    // horizontal separator line between input text and
    Rectangle {
        height: px(1)
        anchors.bottom: inputBox.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: "#cccccc"
    }

    //输入聊天信息对话框
    AppTextField {
        id: inputBox
        height: dp(48)
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: dp(8)
        anchors.rightMargin: dp(8)
        font.pixelSize: sp(16)
        placeholderText: "输入聊天内容 ..."
        backgroundColor: "white"
        verticalAlignment: Text.AlignVCenter

        onAccepted: {
//            newMsgs = newMsgs.concat({me: true, text: inputBox.text})
            client.dataChatMsgSend(groupid, client.userName, inputBox.text)
            inputBox.text = ""

        }
    }
}
