import QtQuick 2.9
import QtQuick.Controls 2.2
import CppTypes 1.0
import "../MyControls" as MC

MC.Page {
    id: page
    title: qsTr("Game")

    property alias gameId: gameModel.gameId

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

        Listing {model: gameModel.shop; clip: true}
        Listing {model: gameModel.deck; clip: true}
        Listing {model: gameModel.hand; clip: true}
        Listing {model: gameModel.discard; clip: true}
    }

    TabBar {
        id: tabBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        TabButton {
            text: qsTr("Shop")
        }
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

    GameModel {
        id: gameModel
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
