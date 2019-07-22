import QtQuick 2.9
import QtQuick.Controls 2.2
import "../MyControls" as MC

MC.Page {
    id: page
    title: qsTr("Game")

    SwipeView {
        id: view
        interactive: false
        anchors.top: parent.top
        anchors.bottom: tabBar.top
        anchors.left: parent.left
        anchors.right: parent.right

        currentIndex: tabBar.currentIndex
        Binding {
            target: tabBar
            property: "currentIndex"
            value: view.currentIndex
        }

        Listing {model: someModel; clip: true}
        Listing {model: someModel; clip: true}
        Listing {model: someModel; clip: true}
    }

    TabBar {
        id: tabBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        TabButton {
            text: qsTr("Deck")
        }
        TabButton {
            text: qsTr("Hand")
        }
        TabButton {
            text: qsTr("Discard")
        }
    }

    ListModel {
        id: someModel
        ListElement {
            title: "Card 1"
            probability: 0.2
        }
        ListElement {
            title: "Card 2"
            probability: 0.6
        }
        ListElement {
            title: "Card 3"
            probability: 0
        }
    }
}
