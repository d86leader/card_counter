import QtQuick 2.9
import QtQuick.Controls 2.2
import "../MyControls" as MC
import "../Common" as C

// Lists all cards in one deck

C.CardListing {
    id: listView

    // to enable forward-backward stack movement
    property Item page

    Binding {
        target: model
        property: "pattern"
        value: searchStr
    }

    footer: MC.Button {
        width: parent.width
        text: qsTr("Create card")
        onClicked: {
            listView.model.create(listView.model.rowCount())
        }
    }

    delegate: MC.SwipeDelegate {
        id: swiper
        width: parent.width

        text: model.title

        swipe.right: Label {
            id: deleteLabel
            text: qsTr("Delete")
            color: "white"
            verticalAlignment: Label.AlignVCenter
            padding: 12
            height: parent.height
            width: swiper.width / 3
            anchors.right: parent.right

            background: Rectangle {
                color: "tomato"
                border.width: 1
                border.color: swiper.background.border.color
            }
        }

        // delete without pressing button, but immediately on drag completion
        swipe.onCompleted: {
            listView.model.moveToSibling(index)
        }

        onPressAndHold: contextMenu.open()
        onClicked: contextMenu.open()

        Menu {
            id: contextMenu

            MenuItem {
                text: qsTr("Edit card")
                onClicked: {
                    var item = Qt.resolvedUrl("../CardEdit/Page.qml")
                    var props = {"card": model}
                    page.openPage(item, props)
                }
            }
            MenuItem {
                text: qsTr("Clone card")
                onClicked: {
                    listView.model.clone(index)
                    var props = {"card": model}
                    var item = Qt.resolvedUrl("../CardEdit/Page.qml")
                    page.openPage(item, props)
                }
            }
            MenuItem {
                text: qsTr("Delete card")
                onClicked: {
                    listView.model.remove(index)
                }
            }
        }
    }
}
