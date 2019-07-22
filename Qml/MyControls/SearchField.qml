import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    property real margins: 10
    property alias text: searchBar.text

    implicitWidth: searchBar.implicitWidth + margins*2
    implicitHeight: searchBar.implicitHeight

    TextField {
        id: searchBar
        anchors.fill: parent
        anchors.leftMargin: margins
        anchors.rightMargin: margins

        placeholderText: qsTr('<img src="%1">Search')
            .arg("qrc:/Assets/search.svg")
    }
}
