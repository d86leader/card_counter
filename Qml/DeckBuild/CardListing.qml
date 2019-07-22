import QtQuick 2.9
import QtQuick.Controls 2.2
import "../MyControls" as MC
import "../Common" as C
import "./OpenCardEditor.js" as Open

// List all cards from all decks

C.CardListing {
    id: listView

    // to enable forward-backward stack movement
    property Item page

    footer: MC.Button {
        width: parent.width
        text: qsTr("Create card")
        onClicked: {
            console.log("create card")
            var item = Qt.resolvedUrl("../CardEdit/Page.qml")
            page.openPage(item, {})
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
            console.log("add " + model.title)
            listView.model.remove(index)
        }

        onPressAndHold: contextMenu.open()

        CardContextMenu {
            id: contextMenu
            onEditChosen: {
                var opener = function(item, props){page.openPage(item, props)}
                Open.openEditor(model, opener)
            }
            onCloneChosen: {
                console.log("Clone card " + model.title)
                var opener = function(item, props){page.openPage(item, props)}
                Open.openEditor(model, opener)
            }
            onDeleteChosen: {
                listView.model.remove(index)
            }
        }
    }
}