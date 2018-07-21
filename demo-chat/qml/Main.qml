//古长蓉
import VPlayApps 1.0
import QtQuick 2.0
import my.client 1.0
import "pages"

App {
    id:app
    // You get free licenseKeys from https://v-play.net/licenseKey
    // With a licenseKey you can:
    //  * Publish your games & apps for the app stores
    //  * Remove the V-Play Splash Screen or set a custom one (available with the Pro Licenses)
    //  * Add plugins to monetize, analyze & improve your apps (available with the Pro Licenses)
    //licenseKey: "<generate one from https://v-play.net/licenseKey>"

    //设置主题颜色
    onInitTheme: {
      Theme.colors.tintColor = "purple"
      Theme.AppButton.fontBold
    }

    //用户登录信息与服务端交互
    Client {
        id: client
        onGetLoginAnswer: {
//            mainloginpage.annotationText.text = sureLogin ? "登录成功":"登录失败"
            if(sureLogin)
            {
                mainloginpage.annotationText.text = "登录成功"
                mainloginpage.txtUsername.text = ""
                mainloginpage.txtPassword.text = ""
                mainloginpage.annotationText.text = ""
                mainloginpage.focus = false
                mainloginpage.visible = false
                maindemochatloader.sourceComponent = maindemochatpage  //登录成功显示主页面
                dataConnectSend()  //发送登录信息
//                console.debug("test")
            }
            if(!sureLogin){
//                console.debug("fewr")
                mainloginpage.annotationText.text = "登录失败"
            }
        }

    }

    Component {
        id: maindemochatpage
        MainDemoChat {}
    }

    Loader {
        id: maindemochatloader
        anchors.fill: parent
    }


   Component {
       id: registerpage
       RegisterPage {}
   }

   Component{
       id: editinfomation
       Editinformaition{}
   }

   Component{
       id: changecolors
       Changecolor {}
   }
   Component{
       id: mypage
       MyPage{ }
   }

    MainLoginPage {
        id: mainloginpage
        biaoti.text: "登录"
        usernametext.text: "用户名"
        powertext.text: "密    码"
//        loginbutton.text: "登录"
    }
}
