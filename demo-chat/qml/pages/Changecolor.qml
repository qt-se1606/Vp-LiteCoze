//author:guchangrong 2018 07 19
//修改主题颜色，每一个按钮对应一个颜色
import VPlayApps 1.0
import QtQuick 2.0

Page {
    title: "修改颜色"
    backgroundColor: "white"
    AppButton{
        id: color1
        text: qsTr("少女粉")
        flat: false
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            Theme.colors.tintColor = "pink"
        }
    }
    AppButton{
        id: color2
        text: qsTr("胖次蓝")
        flat: false
        anchors.top: color1.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            Theme.colors.tintColor = "lightblue"
        }
    }
    AppButton{
        id: color3
        text: qsTr("基佬紫")
        flat: false
        anchors.top: color2.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            Theme.colors.tintColor = "purple"
        }
    }
    AppButton{
        id: color4
        text: qsTr("高端黑")
        flat: false
        anchors.top: color3.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            Theme.colors.tintColor = "lightblack"
        }
    }
    AppButton{
        id: color5
        text: qsTr("咸蛋黄")
        flat: false
        anchors.top: color4.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            Theme.colors.tintColor = "yellow"
        }
    }
    AppButton{
        id: color6
        text: qsTr("姨妈红")
        flat: false
        anchors.top: color5.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            Theme.colors.tintColor = "red"
        }
    }
//    AppButton{
//        id: out
//        text: qsTr("返回")
//        flat: false
//        anchors.bottom: parent.bottom
//        anchors.horizontalCenter: parent.horizontalCenter
//        onClicked: {
//            maindemochatloader.sourceComponent = mypage
//        }
//    }
}
