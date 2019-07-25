import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.3
import "../MyControls" as MC
import "../Common" as C

MC.Page {
    id: page
    title: qsTr("Edit %1").arg(card.title)

    // what we're editing
    property var card

    Flickable {
        anchors.fill: parent
        contentWidth: width

        ScrollBar.vertical: ScrollBar {
             policy: ScrollBar.AsNeeded
        }


        ColumnLayout {
            width: page.width
            // borders again
            spacing: -1

            MC.Button {
                Layout.fillWidth: true
                hasBorder: true
                text: "Name"
                font.bold: false
                font.capitalization: Font.MixedCase
                horizontalAlignment: Text.AlignLeft

                onClicked: {
                    renameDialog.open()
                }

                Dialog {
                    id: renameDialog
                    title: qsTr("Rename")

                    TextField {
                        id: nameLabel
                        // avoid binding loops with one assignment
                        Component.onCompleted: {text = card.title}
                    }

                    standardButtons: StandardButton.Ok | StandardButton.Cancel
                    onAccepted: {
                        card.title = nameLabel.text
                    }
                    onRejected: {}
                }
            }
        }
    }
}
