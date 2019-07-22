import QtQuick 2.9
import QtQuick.Controls 2.2
import "../MyControls" as MC
import "../Common" as C

C.CardListing {
    id: listView

    delegate: MC.SwipeDelegate {
        id: swipeDelegate
        width: parent.width

        contentItem: Column {
            Label {
                text: model.title
            }
            Label {
                text: qsTr("%1%").arg(model.probability * 100)
            }
        }

        swipe.left: MC.SwipeSideButton {
            id: pickLabel
            height: parent.height
            anchors.left: parent.left
            text: qsTr("Pick")

            color: "#111111" // simple black cuts my eyes idk
            bgColor: "lightgreen"
            borderColor: swipeDelegate.background.border.color

            SwipeDelegate.onClicked: {
                console.log("picked")
            }
        }

        swipe.right: MC.SwipeSideButton {
            id: deleteLabel
            height: parent.height
            anchors.right: parent.right
            text: qsTr("Banish")

            color: "white"
            bgColor: "tomato"
            borderColor: swipeDelegate.background.border.color

            SwipeDelegate.onClicked: {
                listView.model.remove(index)
                console.log("banish")
            }
        }
    }
}