import QtQuick 2.9
import QtQuick.Controls 2.2
import "../MyControls" as MC
import "../Common" as C

// List all cards from all decks

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
        deleteDirection: "right"

        text: model.title

        swipe.left: Label {
            id: addLabel
            text: qsTr("Add")
            color: "#111111" // simple black cuts my eyes idk
            verticalAlignment: Label.AlignVCenter
            padding: 12
            height: parent.height
            width: parent.width / 3
            anchors.left: parent.left

            // In Qt 5.9 this rectangle doesn't show for some reason.
            // It shows correctly in Qt 5.12, so I can ignore this bug when testing
            background: Rectangle {
                color: "lightgreen"
                border.width: 1
                border.color: swiper.background.border.color
            }
        }

        // delete without pressing button, but immediately on drag completion
        swipe.onCompleted: {
            listView.model.moveToSibling(index)
        }

        onPressAndHold: contextMenu.open(model, index)
        onClicked: contextMenu.open(model, index)

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
