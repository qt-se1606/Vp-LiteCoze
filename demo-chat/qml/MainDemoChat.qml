//古长蓉
import VPlayApps 1.0
import QtQuick 2.0
import "pages"

Page {
    id: maindemochat
    property alias navigation: navigation


//创建菜单栏
    Navigation {
        id: navigation
        drawer.drawerPosition: drawer.drawerPositionLeft

        NavigationItem{
            title: "群聊"
            icon: IconType.group
            NavigationStack{
                GroupPage{ }
            }
        }

        NavigationItem{
            title: "关注"
            icon: IconType.list
            NavigationStack{
                PeopleChatPage{ }
            }
        }
        NavigationItem{
            title: "心情簿"
            icon: IconType.book
            NavigationStack{
                Moodpage{}
            }
        }

        NavigationItem{
            title: "我"
            icon: IconType.user
            NavigationStack{
                MyPage{ }
            }
        }
    }

}
