import QtQuick 2.9
import QtQuick.Controls 2.2

Menu {
    id: contextMenu

    signal editChosen()
    signal cloneChosen()
    signal deleteChosen()

    MenuItem {
        text: qsTr("Edit card")
        onClicked: {
            contextMenu.editChosen()
        }
    }

    MenuItem {
        text: qsTr("Clone card")
        onClicked: {
            contextMenu.cloneChosen()
        }
    }

    MenuItem {
        text: qsTr("Delete card")
        onClicked: {
            contextMenu.deleteChosen()
        }
    }
}

