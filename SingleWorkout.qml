import QtQuick 2.9
import QtQuick.Controls 2.2


ListView {
    id: listView
    width: 600
    height: 600

     //contentWidth: headerItem.width
    //flickableDirection: Flickable.HorizontalAndVerticalFlick
/*
    header: Row {
        spacing: 1
        function itemAt(index) { return repeater.itemAt(index) }
        Repeater {
            id: repeater
            model:
            Label {
                text: modelData
                font.bold: true
                font.pixelSize: 20
                padding: 10
                background: Rectangle { color: "silver" }
            }
        }
    }
*/

    model: singleWorkoutModel.rowCount()
    delegate: Column {
        property int row: index
        spacing: 8
        ItemDelegate {
            text: singleWorkoutModel.getParam("name", row)
            highlighted: true
            display: AbstractButton.TextOnly
        }
        Row {
            spacing: 1
            Repeater {
                model: singleWorkoutModel.columnCount(singleWorkoutModel.index(row, 0))
                ItemDelegate {
                    property int series: index
                    text: qsTr("%1 reps").arg(singleWorkoutModel.getParam("reps", row, series))
                }
            }
        }
    }

        /*Column {
        id: delegate
        property int row: index
        Row {
            spacing: 1
            Repeater {
                model: 5
                ItemDelegate {
                    property int column: index
                    text: qsTr("%1x%2").arg(delegate.row).arg(column)
                    width: listView.headerItem.itemAt(column).width
                }
            }
        }
        Rectangle {
            color: "silver"
            width: parent.width
            height: 1
        }
    }*/

    ScrollIndicator.horizontal: ScrollIndicator { }
    ScrollIndicator.vertical: ScrollIndicator { }
}
