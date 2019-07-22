import QtQuick 2.9
import QtQuick.Controls 2.2
import "../MyControls" as MC

// List all cards from all decks

ListView {
    id: listView
    spacing: -1

    ScrollBar.vertical: ScrollBar {
         policy: ScrollBar.AsNeeded
    }

    header: MC.SearchField {
        width: parent.width
    }

    footer: MC.Button {
        width: parent.width
        text: qsTr("Create card")
        onClicked: {
            console.log("create card")
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
    }
}
