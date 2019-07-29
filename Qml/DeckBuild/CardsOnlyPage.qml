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

            onPressAndHold: contextMenu.open(model, index)
            onClicked: contextMenu.open(model, index)

            Menu {
                id: contextMenu
                readonly property url editPage: Qt.resolvedUrl("../CardEdit/Page.qml")

                MenuItem {
                    text: qsTr("Edit card")
                    onClicked: {
                        var props = {"card": model}
                        var item = Qt.resolvedUrl("../CardEdit/Page.qml")
                        page.openPage(item, props)
                    }
                }
                MenuItem {
                    text: qsTr("Clone card")
                    onClicked: {
                        listView.model.clone(index)
                        var props = {"card": model}
                        var item = Qt.resolvedUrl("../CardEdit/Page.qml")
                        page.openPage(item, props)
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
}
