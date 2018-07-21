//anthor:guchangrong
//anthor:duhongpeng
import VPlayApps 1.0
import QtQuick 2.0

ListPage {
    id: moodpage
    title: "我的心情簿"

    property var newMsgs: []

    Connections {
        target: client
        onSignalAddMood: {
            console.debug("s")
            newMsgs = newMsgs.concat({text: msg, msg_user_name: user_name})
        }
    }

    //添加心情的按钮
    rightBarItem: IconButtonBarItem {
        icon: IconType.plus
        onClicked: InputDialog.inputTextMultiLine(app,
                                                  qsTr("发表心情"),
                                                  qsTr("说说心情..."),
                                                  function(ok, text) {
                                                      if(ok) client.dataAddMoodSend(text)
                                                  })
    }

    model: {
        var model = newMsgs
        return model
    }

    delegate: Item {
        id: bubble
        width: parent.width
        height: bg.height + touxiang.height + dp(30)

        Rectangle {
            width: parent.width
            height: parent.height - dp(20)
            color: "blue"
            y: dp(10)
            radius: dp(5)
            opacity: 0.4
        }

        Rectangle{
            id: touxiang
            color: Theme.tintColor
            x: bg.x
            y: dp(13)
            width: dp(45)
            height: dp(45)
            radius: dp(180)
        }

        Text {
            x: touxiang.width + dp(15)
            y: touxiang.y + dp(8)
            id: usernametext
            wrapMode: Text.WordWrap
            text: modelData.msg_user_name
            font.pixelSize: sp(20)
            color: "gray"
        }

        Rectangle {
            id: bg
            color: Theme.tintColor
            radius: dp(10)

            x: dp(10)
            y: touxiang.y + dp(49)
            width: parent.width - dp(10*2)
            height: innerText.implicitHeight + dp(20)

            Text {
                id: innerText
                x: dp(10)
                y: dp(10)
                width: parent.width
                wrapMode: Text.WordWrap
                text: modelData.text
                font.pixelSize: sp(16)
                color: "white"
            }
        }
    }

}
