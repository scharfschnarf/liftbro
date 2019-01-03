import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import WDCData 1.0

Repeater {
    id: setRepeater
    property int exercise;
    property int setsCount: singleWorkoutModel.columnCount(singleWorkoutModel.index(exercise, 0));

    model: setsCount
    SetButtons {
        property int currentSet: index;
        property int reps:   singleWorkoutModel.getParam(WDC.Reps,   exercise, currentSet);
        property int weight: singleWorkoutModel.getParam(WDC.Weight, exercise, currentSet);

        buttonColor: "#c72121"
        textTop: reps
        hasBottom: (weight > -1);
        textBottom: weight + " kg"

        // Config dialogs for event handlers

        Dialog {
            property int currentSet: index.currentSet;
            property int reps: parent.reps;

            id: repsChangeDialog
            visible: false
            title: "Set amount of repetitions for set " + currentSet

            standardButtons: StandardButton.Save | StandardButton.Cancel

            contentItem: Column {
                spacing: 4

                SpinBox {
                    id: repsCount
                    maximumValue: 2147483647
                    value: parent.reps;
                }
                Button {
                    id: deleteButton
                    text: "Tap to delete set"
                    onClicked: {
                        repsChangeDialog.close()
                        singleWorkoutModel.deleteSet(setRepeater.exercise, reps)
                        setRepeater.setsCount = singleWorkoutModel.columnCount(singleWorkoutModel.index(setRepeater.exercise, 0));
                        setRepeater.model = setRepeater.setsCount
                    }
                }
            }
            onAccepted: singleWorkoutModel.setParam(reps, WDC.Reps, setRepeater.exercise, currentSet)
        }

        onClickTop:    singleWorkoutModel.setParam(++reps,   WDC.Reps,   row, series)
        onClickBottom: singleWorkoutModel.setParam(++weight, WDC.Weight, row, series)
        onClickHoldTop: repsChangeDialog.visible = true

    }
}





/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
