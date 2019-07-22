import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.1
import "../MyControls" as MC
import "../Common" as C

MC.Page {
    title: qsTr("All cards")

    C.CardListing {
        id: listView
        anchors.fill: parent

        model: someModel

        footer: MC.Button {
            width: parent.width
            text: qsTr("Create card")
            onClicked: {
                console.log("create card")
            }
        }

        delegate: MC.DelegateButton {
            id: control
            width: parent.width
            text: model.title

            onPressAndHold: contextMenu.open()

            Menu {
                id: contextMenu

                MenuItem {
                    text: qsTr("Edit card")
                    onClicked: {
                        console.log("edit card " + model.title)
                    }
                }

                MenuItem {
                    text: qsTr("Clone card")
                    onClicked: {
                        console.log("Clone card " + model.title)
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
