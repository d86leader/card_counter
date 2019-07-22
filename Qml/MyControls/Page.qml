import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    property list<ToolButton> toolButtons

    signal openPage(var item, var properties)
    signal backPage()
    // Function to be executed before closing/destroying this page
    // Return falsy value to abort
    property var beforeClose: function() {return true}
}
