//anthour:guchangrong
//针对设计目标作出修改20180719
import VPlayApps 1.0
import QtQuick 2.0

ListPage {
        id: peoplecoze
        title: "我的关注"

        rightBarItem: IconButtonBarItem {
            icon: IconType.plus
            onClicked: {
//                client.dataAddGroupSend(text)
                roomloader.sourceComponent = makefriends
        }
    }
        Loader{
            id: roomloader
            anchors.centerIn: parent
        }

        //与朋友加好友的组件，添加日期20180703
        Component{
            id: makefriends
            Item {
                width: app.width
                height: app.height

                //设置背景为半透明
                Rectangle{
                    id: bigrectangel
                    width: app.width
                    height: app.height
                    color: "black"
                    opacity: 0.5
                }

                //创建对话框
                Rectangle{
                    id: createdialog
                    anchors.centerIn: bigrectangel
                    color: "White"
                    width: 0.8 * bigrectangel.width
                    height: 0.3 * bigrectangel.height
                    radius: 4

                    //标题
                    AppText{
                        id: toprow
                        anchors.top: createdialog.top
                        anchors.topMargin: dp(15)
                        anchors.horizontalCenter: createdialog.horizontalCenter
                        text: qsTr("添加关注")
                        fontSize: 32
                    }

                    //提示及输入框
                    AppText{
                        id: leftsecondrow
                        width: 1/4 * createdialog.width
                        anchors.left: createdialog.left
                        anchors.leftMargin: dp(30)
                        anchors.top: toprow.bottom
                        anchors.topMargin: dp(30)
                        text: qsTr("好友ID")
                        font.pixelSize: sp(16)
                        fontSize: 16
                    }
                    AppTextField{
                        id: textroomname
                        width: 3/5 * createdialog.width
                        height: 1/4 * createdialog.height
                        anchors.left: leftsecondrow.right
                        anchors.leftMargin: dp(4)
                        anchors.top: toprow.bottom
                        showClearButton: true
                        font.pixelSize: sp(16)
                        borderColor: Theme.tintColor
                        borderWidth: !Theme.isAndroid ? dp(2) : 0
                    }
                    //添加按钮
                    AppButton{
                        id: createbutton
                        width: 1/3 * createdialog.width
                        height: 1/4 * createdialog.height
                        anchors.left: createdialog.left
                        anchors.leftMargin: dp(20)
                        anchors.bottom: createdialog.bottom
                        anchors.bottomMargin: dp(10)
                        text: qsTr("添加")
                        flat: false
                        onClicked: {
                            client.dataAddGroupSend(textroomname.text)
                        }
                    }
                    //取消按钮
                    AppButton{
                        id: canclebutton
                        width: 1/3 * createdialog.width
                        height: 1/4 * createdialog.height
                        anchors.right: createdialog.right
                        anchors.rightMargin: dp(20)
                        anchors.bottom: createdialog.bottom
                        anchors.bottomMargin: dp(10)
                        text: qsTr("取消")
                        flat: false
                        onClicked: {
                            roomloader.sourceComponent = undefined
                        }
                    }
                }
            }
        }
}

