import QtQuick 2.0
import QtQuick.Controls 1.3

Rectangle {
    id: propRec
    property int fontSize: 10
    property int rowHeight: fontSize + 4
    property int valueWidth: 60 // width of value box
    property int typeWidth: 20
    property int unitWidth: 75  // width of unit dropdown
    property int menuWidth: fontSize    // Width of menu icon
    property int rowSpacing: 3
    property int precision: 4   // value decimal places
    property int editMenuIndex: 0   // value to show menu toggles (show and report)
    property bool multiPhase: true
    property int rowTotalWidth: typeWidth + unitWidth + 2*rowSpacing + ((valueWidth + rowSpacing) * (multiPhase ? 1:3)) + (editMenuIndex==0 ? 0:(menuWidth+rowSpacing))

    property bool isInput: false
    property bool isVisible: true
    property bool isReportable: true

    property double value1: 0
    property alias propName: propType.text
    property alias unitList: unitCB.model
    property bool showUnitBox: unitList.count > 0
    signal inputChanged(double valueSet)
    signal unitChanged(int unitSet)

    width: rowTotalWidth
    height: (isVisible || editMenuIndex>0) ? rowHeight + 4 : 0
    opacity: height==0 ? 0 : 1
    radius: 2
    border.width: 1
    border.color: isInput ? "blue" : "grey"
    Behavior on height {NumberAnimation {duration: 300}}
    Behavior on rowTotalWidth {NumberAnimation {duration: 300}}
    enabled: height>0
    clip: true
    Row {
        id: propRow
        spacing: rowSpacing
        height: propRec.height - 4
        x:2; y:2
        TextInput {
            id: propValue1
            height: rowHeight
            width: valueWidth
            text: value1.toLocaleString(Qt.locale(),'f', precision)
            readOnly: !isInput
            color: isInput ? "darkblue" : "black"
            onAccepted: {
                console.log("value1 is " + value1 + "   text = " + text)
                value1 = text;
                console.log("value1 is " + value1 + "   text = " + text)
            }
        }
        Text {
            id: propType
            width: typeWidth + (showUnitBox ? 0 : unitWidth + rowSpacing)
            height: rowHeight
            text: ""
            clip: true
        }
        ComboBox {
            id: unitCB
            width: unitWidth
            height: rowHeight
//            opacity: (model.count>0) ? 1 : 0
            visible: showUnitBox //(model.count>0)
            onCurrentIndexChanged: { propRec.unitChanged(currentIndex) }
        }
        Image {
            id: menuPic
            width: (editMenuIndex == 0) ? 0 : menuWidth
            Behavior on width {NumberAnimation {duration: 300}}
            visible: width>0
            height: menuWidth
            source: (editMenuIndex == 1) ? "images/eye.ico" : "images/pencil.png"
            opacity: (editMenuIndex == 1) ? (isVisible ? 1 : 0.5) : (isReportable ? 1 : 0.5)
            fillMode: Image.PreserveAspectFit
            MouseArea {
                anchors.fill: parent
                onClicked:  {
                    if (editMenuIndex == 1) {
                        isVisible -= 1;
                    }
                    else {
                        isReportable -= 1;
                    }
                }
            }
        }
    }
}
