import VPlayApps 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.1

Page {
    id: registerpage
    signal registerSuccessed

    //向服务器发送注册信号
    Connections {
        target: client
        onGetRegisterAnswer: {
            ifregist.text = registerAnswer
        }
    }

    NavigationStack{

        Page {
            id: register

            //返回图标
            leftBarItem: IconButtonBarItem {
                icon: IconType.backward
                onClicked: {
                    mainloginpage.visible = true
                    mainloginpage.focus = true

                    maindemochatloader.sourceComponent = undefined
                }
            }

            // 注册页面背景，修改格式20180702
            Rectangle {
                id: registerForm
                anchors.horizontalCenter: parent.horizontalCenter
                //                anchors.top: register.top
                //                anchors.topMargin: dp(43)
                y: 1/5 * registerpage.height
                color: "lightblue"
                width: content.width + dp(48)
                height: content.height + dp(40) + appbutton.height
                radius: dp(4) //半径
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

            // 登录页面排版
            GridLayout {
                id: content
                anchors.top: registerForm.top
                anchors.horizontalCenter: registerForm.horizontalCenter
                columnSpacing: dp(20)
                rowSpacing: dp(10)
                columns: 2

                // 标题
                AppText {
                    Layout.topMargin: dp(20)
                    Layout.bottomMargin: dp(12)
                    Layout.columnSpan: 2
                    Layout.alignment: Qt.AlignHCenter
                    text: "注册"
                    fontSize: 32
                }

                // 用户名提示和输入框
                AppText {
                    text: qsTr("用户名")
                    font.pixelSize: sp(16)
                    fontSize: 16
                }

                AppTextField {
                    id: txtUsername
                    Layout.preferredWidth: dp(200) //标题栏长度
                    showClearButton: true //desplay clear
                    font.pixelSize: sp(16) //输入字体大小
                    borderColor: Theme.tintColor
                    borderWidth: !Theme.isAndroid ? dp(2) : 0
                }

                // 密码提示和输入框
                AppText {
                    text: qsTr("密  码")
                    font.pixelSize: sp(16)
                    fontSize: 16
                }

                AppTextField {
                    id: txtPassword
                    Layout.preferredWidth: dp(200)
                    showClearButton: true
                    font.pixelSize: sp(16)
                    borderColor: Theme.tintColor
                    borderWidth: !Theme.isAndroid ? dp(2) : 0
                    echoMode: TextInput.Password
                }

                AppText {
                    id: ifregist
                    font.pixelSize: sp(16)
                    fontSize: 16
                    Layout.columnSpan: 2
                    Layout.alignment: Qt.AlignHCenter
                    //                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            //注册按钮
            AppButton {
                id: appbutton
                anchors.bottom: registerForm.bottom
                anchors.bottomMargin: dp(20)
                text: qsTr("注册")
                flat: false
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {

                    // simulate successful login
                    console.debug("register in ...")
                    client.registerName = txtUsername.text
                    client.registerPower = txtPassword.text
                    client.dataRegisterSend()
                    ifregist.text = "正在努力加载 ..."
                }
            }

        }
    }
}

