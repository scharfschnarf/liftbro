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
            Repeater {
                // SET DELEGATE
                model: singleWorkoutModel.columnCount(singleWorkoutModel.index(row, 0))
                SetButtons {
                    property int series: logValue("index passed as series", index)
                    property int reps:   singleWorkoutModel.getParam(WDC.Reps, row, series)
                    property int weight: singleWorkoutModel.getParam(WDC.Weight, row, series)

                    buttonColor: "#c72121"
                    textTop: reps
                    hasBottom: (weight > -1);
                    textBottom: weight + " kg"

                    onClickTop:    singleWorkoutModel.setParam(++reps,   WDC.Reps,   row, series)
                    onClickBottom: singleWorkoutModel.setParam(++weight, WDC.Weight, row, series)
                    onClickHoldTop:    singleWorkoutModel.setParam(reps   = -1, WDC.Reps,   row, series)
                    onClickHoldBottom: singleWorkoutModel.setParam(weight = 0,  WDC.Weight, row, series)
                }
            }
        }
    }

    ScrollIndicator.horizontal: ScrollIndicator { }
    ScrollIndicator.vertical: ScrollIndicator { }
}
