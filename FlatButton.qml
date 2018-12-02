import QtQuick 2.7

Rectangle {
    property alias text: buttonText.text
    property alias textWeight: buttonText.font.weight
    property alias textHeight: buttonText.font.pixelSize
    signal click();
    signal clickHold();
    signal doubleClick();

    border.width: 0
    visible: true
    Text {
        anchors.fill: parent
        id: buttonText
        color: "#ffffff"
        text: text
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: height / 2
        font.weight: Font.Black
    }
    MouseArea {
        id: events
        height: 48
        anchors.fill: parent

        onClicked: parent.click()
        onPressAndHold: parent.clickHold()
        onDoubleClicked: parent.doubleClick()
    }
}
