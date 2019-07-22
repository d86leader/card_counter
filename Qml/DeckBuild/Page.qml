import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.1
import "../MyControls" as MC

MC.Page {
    id: mainPage
    title: "Deck build"

    toolButtons: ToolButton {
        // a menu with several deck options
        text: "⋮"
        onClicked: menu.open()

        Menu {
            id: menu

            MenuItem {
                text: qsTr("Change name")
                onClicked: renameDialog.open()

                Dialog {
                    id: renameDialog
                    title: qsTr("Rename")

                    TextField {
                        id: nameLabel
                        // avoid binding loops with one assignment
                        Component.onCompleted: {text = "Old name"}
                    }

                    standardButtons: StandardButton.Ok | StandardButton.Cancel
                    onAccepted: {
                        console.log("Renamed: " + nameLabel.text)
                    }
                    onRejected: {}
                }
            }
        }
    }

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

        CardListing {model: someModel; clip: true; page: mainPage}
        DeckListing {model: someModel; clip: true; page: mainPage}
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

    ListModel {
        id: someModel
        ListElement {
            title: "Card 1"
        }
        ListElement {
            title: "Card 2"
        }
        ListElement {
            title: "Card 3"
        }
        ListElement {
            title: "Card 3"
        }
        ListElement {
            title: "Card 3"
        }
        ListElement {
            title: "Card 3"
        }
        ListElement {
            title: "Card 3"
        }
        ListElement {
            title: "Card 3"
        }
        ListElement {
            title: "Card 3"
        }
        ListElement {
            title: "Card 3"
        }
    }
}
