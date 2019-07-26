import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.1
import CppTypes 1.0
import "../MyControls" as MC

MC.Page {
    id: mainPage
    title: "Deck build"
    property alias gameId: commonModel.gameId

    // two tabs: cards to add to deck, and cards already in deck
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

        CardListing {
            model: commonModel.notIncludedModel
            clip: true
            page: mainPage
        }
        DeckListing {
            model: commonModel.deckModel
            clip: true
            page: mainPage
        }
    }

    DeckBuildModel {
        id: commonModel
    }

    // tab switcher
    TabBar {
        id: tabBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        // open page on deck listing
        currentIndex: 1

        TabButton {
            text: qsTr("All Cards")
        }
        TabButton {
            text: qsTr("Deck")
        }
    }
}
