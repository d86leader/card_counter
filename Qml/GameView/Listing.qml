import QtQuick 2.9
import QtQuick.Controls 2.2
import "../MyControls" as MC
import "../Common" as C

C.CardListing {
    id: listView

    header: null

    footer: MC.Button {
        width: parent.width
        text: qsTr("Sway all")
        onClicked: {
            listView.model.dropAll()
        }
    }

    delegate: MC.SwipeDelegate {
        id: swipeDelegate
        width: parent.width

        contentItem: Column {
            Label {
                text: model.title
            }
            Label {
                text: qsTr("%1: %2%").arg(model.amount).arg(model.probability * 100)
            }
        }

        swipe.left: MC.SwipeSideButton {
            id: pickLabel
            height: parent.height
            anchors.left: parent.left
            width: implicitWidth * 2
            text: qsTr("Pick")

            color: "#111111" // simple black cuts my eyes idk
            bgColor: "lightgreen"
            borderColor: swipeDelegate.background.border.color

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.model.pick(index)
                }
            }
        }

        swipe.right: MC.SwipeSideButton {
            id: deleteLabel
            height: parent.height
            anchors.right: parent.right
            width: implicitWidth * 2
            text: qsTr("Banish")

            color: "white"
            bgColor: "tomato"
            borderColor: swipeDelegate.background.border.color

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.model.banish(index)
                }
            }
        }
    }
}
