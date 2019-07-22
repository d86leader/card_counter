import QtQuick 2.9
import QtQuick.Controls 2.2

// List all cards from all decks

ListView {
    id: listView

    // make the delegate borders overlap, otherwise they are 2 pixel wide and
    // look shitty
    spacing: -1

    ScrollBar.vertical: ScrollBar {
         policy: ScrollBar.AsNeeded
    }

    header: Item {
        width: parent.width
        implicitHeight: searchBar.implicitHeight

        TextField {
            id: searchBar
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            placeholderText: qsTr('<img src="%1">Search')
                .arg("qrc:/Assets/search.svg")
        }
    }
}
