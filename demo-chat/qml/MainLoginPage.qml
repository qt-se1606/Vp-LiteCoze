//古长蓉
import VPlayApps 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.1
import "pages"

Page {
    id: loginPage
    signal loginSucceeded
    //    enabled: visible
    //    visible: true
    backgroundColor: "White"

    property alias annotationText: annotationText
    property alias txtUsername: txtUsername
    property alias txtPassword: txtPassword
    property alias biaoti: biaoti
    property alias usernametext: usernametext
    property alias powertext: powertext
//    property alias loginbutton: loginbutton


    // 登录界面背景，修改日期20180701,手机集成登录页面无法显示中文
    Rectangle {
        id: loginForm
        anchors.centerIn: parent
        color: "lightblue"
        width: 0.8 * loginPage.width
        height: 0.45 * loginPage.height
        radius: dp(4)
        opacity: 0.4
        gradient: Gradient {
                  GradientStop{
                      position: 0.0; color: "blue"
                  }
                  GradientStop{
                      position: 1.0; color: "red"
                  }
              }
}
        // 标题
        AppText {
            id: biaoti
            height: 1/4 * loginForm.height
            anchors.top: loginForm.top
            anchors.topMargin: dp(15)
            anchors.horizontalCenter: loginForm.horizontalCenter
//            color: "white"
            font.pixelSize: dp(36)
        }

        // 用户名提示和输入框
        AppText {
            id: usernametext
            width: 1/4 * loginForm.width
            height: 1/8 * loginForm.height
            anchors.top: biaoti.bottom
            anchors.topMargin: dp(8)
            anchors.left: loginForm.left
            anchors.leftMargin: dp(20)
//            color: "white"
            font.pixelSize: sp(16)
        }

        AppTextField {
            id: txtUsername
            width: 0.6 * loginForm.width
            height: 1/9 * loginForm.height
            anchors.top: biaoti.bottom
            anchors.topMargin: dp(8)
            anchors.left: usernametext.right
            showClearButton: true //desplay clear
            font.pixelSize: sp(16) //输入字体大小
            borderColor: Theme.tintColor
            borderWidth: !Theme.isAndroid ? dp(2) : 0
        }

        // 密码提示和输入框
        AppText {
            id: powertext
            width: 1/4 * loginForm.width
            height: 1/8 * loginForm.height
            anchors.top: usernametext.bottom
            anchors.topMargin: dp(8)
            anchors.left: loginForm.left
            anchors.leftMargin: dp(20)
//            color: "white"
            font.pixelSize: sp(16)
        }

        AppTextField {
            id: txtPassword
            width: 0.6 * loginForm.width
            height: 1/9 * loginForm.height
            anchors.top: txtUsername.bottom
            anchors.topMargin: dp(8)
            anchors.left: powertext.right
            showClearButton: true
            font.pixelSize: sp(16)
            borderColor: Theme.tintColor
            borderWidth: !Theme.isAndroid ? dp(2) : 0
            echoMode: TextInput.Password

        }
        AppText{
            id: annotationText
            anchors.bottom: loginbutton.top
            anchors.bottomMargin: dp(4)
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: sp(16)
        }

        // 登录按钮，进入主界面
        AppButton{
            id: loginbutton
            anchors.top: txtPassword.bottom
            anchors.topMargin: dp(30)
            text: qsTr("LOGIN")
            flat: false
            textColor: "white"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                annotationText.text = "正在努力加载..."
                client.userName = txtUsername.text
                client.userPower = txtPassword.text
                client.dataLoginSend()
            }
        }

        //注册按钮，跳转至注册界面
        AppButton {
            anchors.top: loginbutton.bottom
//            anchors.topMargin: dp(4)
            text: qsTr("No account yet? Register now")
            flat: true
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                loginPage.forceActiveFocus() // move focus away from text fields
                maindemochatloader.sourceComponent = registerpage
                mainloginpage.visible = false
                mainloginpage.focus = false
                // call your server code to register here
                console.debug("registering...")
            }
        }

}



