import QtQuick 2.0

Item {
    property string textTop;
    property bool hasBottom;
    property string textBottom;
    property color buttonColor;

    property int setWidth: 48;
    property int heightTop: 48;
    property int heightBottom: 24;
    property int spacing: 4;

    signal clickTop();
    signal clickHoldTop();
    signal doubleClickTop();
    signal clickBottom();
    signal clickHoldBottom();
    signal doubleClickBottom();

    clip: true
    width: setWidth
    height: heightTop + spacing + heightBottom

    FlatButton {
        id: topButton
        width: parent.width
        height: heightTop
        color: buttonColor;
        text: textTop

        onClick: parent.clickTop()
        onDoubleClick: parent.doubleClickTop()
        onClickHold: parent.clickHoldTop()
    }

    FlatButton {
        id: bottomButton
        visible: hasBottom
        width: setWidth
        height: heightBottom
        color: buttonColor
        text: textBottom

        onClick: clickBottom()
        onClickHold: clickHoldBottom()
        onDoubleClick: doubleClickBottom()

        anchors.top: topButton.bottom
        anchors.topMargin: spacing

        textWeight: Font.Bold
    }
}
