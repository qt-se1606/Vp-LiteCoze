import VPlayApps 1.0
import QtQuick 2.0
import QtQuick.Layouts 1.1

ListPage {
    id:grouppage
    title: "群聊列表"
    property var newGroup:[]

    Connections {
        target: client
        onSignalAddGroup: {
            console.debug("fas")
            newGroup = newGroup.concat({text: group_name, idText: group_id})
        }
    }

    //右侧添加图标及实现
    rightBarItem: IconButtonBarItem {
        icon: IconType.plus
        onClicked:InputDialog.inputTextMultiLine(app,
                                                 qsTr("创建房间"),
                                                 qsTr("房间名..."),
                                                 function(ok, text) {
                                                   if(ok) client.dataAddGroupSend(text)
                                                 })
    }



    // 组列表的模板，杜虹朋
    delegate: SimpleRow {
        image.radius: image.height
        image.fillMode: Image.PreserveAspectCrop
        autoSizeImage: true
        style.showDisclosure: false
        imageMaxSize: dp(48)
        detailTextItem.maximumLineCount: 1
        detailTextItem.elide: Text.ElideRight

        onSelected: {
            navigationStack.push(detailPageComponent, {
                                     person: item.text,
                                     // newMsgs: [{text: item.detailText, me: true}]
                                     groupid: item.idText
                                 })
        }
    }

    Component {
        id: detailPageComponent

        ConversationPage {
            id: conversationpage
        }
    }

    model: {
        var model = newGroup
        return model
    }
}
