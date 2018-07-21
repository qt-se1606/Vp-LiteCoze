//anthuor:guchangrong 20180719
//新增编辑信息和颜色修改两个选项
import VPlayApps 1.0
import QtQuick 2.0

ListPage {
    id: mypage
    title: "关于我"

    Loader{
        id: pageloder
        anchors.centerIn: parent
    }

    //编辑用户信息的按钮
    AppButton{
        id: editbutton
        text: qsTr("编辑信息")
        flat: false
        anchors.top: parent.top
        anchors.topMargin: dp(12)
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            var component = Qt.createComponent("Editinformaition.qml")
            if(component.status === Component.Ready)
            {
                component.createObject(app)
                mypage.visible = false
                console.log("dfgh")

            }
        }
    }

    //修改整体颜色按钮
    AppButton{
        id: changecolorbutton
        text: qsTr("修改颜色")
        flat: false
        anchors.top: editbutton.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            var component = Qt.createComponent("Changecolor.qml")
            if(component.status === Component.Ready)
            {
                component.createObject(app)
                mypage.visible = false
                console.log("dfgh")

            }
        }
    }

    //退出当前帐号按钮
    AppButton{
        id:logout
        text: qsTr("退出当前帐号")
        flat: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        onClicked:{
            mainloginpage.visible = true
            mainloginpage.focus = true
            maindemochatloader.sourceComponent = undefined
        }
    }

}

