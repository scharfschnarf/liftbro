import QtQuick 2.9
import QtQuick.Controls 2.2
import WDCData 1.0
//import "FlatButton.qml"

ListView {
    function logValue(text, value) {
        console.log(text, " = ", value);
        return value
    }

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

    model: logValue("rowCount for model", singleWorkoutModel.rowCount())
    delegate: Column {
        property int row: logValue("index passed as row", index)
        spacing: 8
        ItemDelegate {
            text: logValue("name passed in item delegate", singleWorkoutModel.getParam(WDC.Name, row))
            highlighted: true
            display: AbstractButton.TextOnly
        }
        Row {
            spacing: 8
            SingleSet {
                exercise:  row
                //setsCount: logValue("number of sets passed as model: ", singleWorkoutModel.columnCount(singleWorkoutModel.index(row, 0)))
            }
        }
    }

    ScrollIndicator.horizontal: ScrollIndicator { }
    ScrollIndicator.vertical: ScrollIndicator { }
}
