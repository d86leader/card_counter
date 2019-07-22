import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

Label {
    id: control

    property color bgColor
    property color borderColor

    padding: 12
    verticalAlignment: Label.AlignVCenter

    background: Rectangle {
        color: bgColor
        border.width: 1
        border.color: borderColor

        Ripple {
            width: parent.width
            height: parent.height

            clip: visible
            anchor: control
            active: control.SwipeDelegate.pressed
            color: control.Material.rippleColor
        }
    }
}
