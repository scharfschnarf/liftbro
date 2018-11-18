import QtQuick 2.9
import QtQuick.Window 2.2

ListView {
    width: 300;
    height: 300;
    model: workoutListModel
    delegate: Text {
        text: "Time: " + dateTime
    }
}
