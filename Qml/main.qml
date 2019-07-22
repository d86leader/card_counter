import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "./GameView" as GameView
import "./DeckBuild" as DeckBuild

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Stack")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        RowLayout {
            width: parent.width

            ToolButton {
                id: toolButton
                text: stackView.depth > 1 ? "\u25C0" : "\u2630"
                font.pixelSize: Qt.application.font.pixelSize * 1.6
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop()
                    } else {
                        drawer.open()
                    }
                }
            }

            Label {
                text: stackView.currentItem.title
                Layout.fillWidth: true
                font.bold: true
            }

            // all page's buttons go here
            Row {
                data: stackView.currentItem.toolButtons
            }
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Deck builder")
                width: parent.width
                onClicked: {
                    stackView.push("DeckBuild/Page.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Game view")
                width: parent.width
                onClicked: {
                    stackView.push("GameView/Page.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: DeckBuild.Page {}
        anchors.fill: parent
    }
}
