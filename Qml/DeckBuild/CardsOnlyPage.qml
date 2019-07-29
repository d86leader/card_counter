import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.1
import CppTypes 1.0
import "../MyControls" as MC
import "../Common" as C
import "./OpenCardEditor.js" as Open

MC.Page {
    id: page
    title: qsTr("All cards")

    CardListing {
        id: listView
        anchors.fill: parent

        model: SqlTableModel {
            tableName: "cards"
        }

        delegate: MC.DelegateButton {
            id: control
            width: parent.width
            text: model.title

            onPressAndHold: contextMenu.open()
            onClicked: contextMenu.open()

            CardContextMenu {
                id: contextMenu
                onEditChosen: {
                    var opener = function(item, props){page.openPage(item, props)}
                    Open.openEditor(model, opener)
                }
                onCloneChosen: {
                    listView.model.clone(index)
                    var opener = function(item, props){page.openPage(item, props)}
                    Open.openEditor(model, opener)
                }
                onDeleteChosen: {
                    listView.model.remove(index)
                }
            }
        }
    }
}
