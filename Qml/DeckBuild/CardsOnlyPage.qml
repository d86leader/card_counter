import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.1
import "../MyControls" as MC
import "../Common" as C
import "./OpenCardEditor.js" as Open

MC.Page {
    id: page
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
