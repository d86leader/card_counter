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

                Keys.onBackPressed: stackView.backPage()
                Keys.onEscapePressed: stackView.backPage()
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
                    stackView.openPage("DeckBuild/Page.qml", {})
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Game view")
                width: parent.width
                onClicked: {
                    stackView.openPage("GameView/Page.qml", {})
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("View all cards")
                width: parent.width
                onClicked: {
                    stackView.openPage("DeckBuild/CardsOnlyPage.qml", {})
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView

        function openPage(item, properties) {
            var newItem = stackView.push(item, properties)
            if (newItem == null) {
                console.log("Couldn't create a page!")
            } else {
                connectStackActions(newItem)
            }
        }

        function backPage() {
            var should = stackView.currentItem.beforeClose()
            if (should) {
                stackView.pop()
            }
        }

        function connectStackActions(item) {
            item.openPage.connect(stackView.openPage)
            item.backPage.connect(stackView.backPage)
        }

        initialItem: DeckBuild.Page {
            id: initial
            Component.onCompleted: stackView.connectStackActions(initial)
        }
        anchors.fill: parent
    }
}
