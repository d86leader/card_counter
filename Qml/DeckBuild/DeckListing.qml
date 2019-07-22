import QtQuick 2.9
import QtQuick.Controls 2.2
import "../MyControls" as MC

// Lists all cards in one deck

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
            console.log("delete " + model.title)
            listView.model.remove(index)
        }
    }
}
