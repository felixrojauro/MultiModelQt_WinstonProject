import QtQuick 2.0
import QtQuick.Controls 1.3
import WEOS 1.0

Rectangle {
    id: uiStream
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
    property bool singleVaporPhase: true // True = only vapor, False = liquid.  Overriden by multiPhase
    property bool isCompostionInput: true
    property bool showComposition: true

    property bool expandCompostion: false
    property bool visableComposition: true
    property bool reportComposition: true

    property int rowTotalWidth: typeWidth + unitWidth + 2*rowSpacing + ((valueWidth + rowSpacing) * (multiPhase ? 1:3)) + (editMenuIndex==0 ? 0:(menuWidth+rowSpacing))

    color: "#ffffff"
    border.color: "black"
    border.width: 2
    height: 250  // or less
//    width: rowTotalWidth + 2*rowSpacing
	width: 600
    Behavior on rowTotalWidth {NumberAnimation {duration: 300}}


	TStream
	{
		id: stream

		Component.onCompleted:
		{
			stream.loadCompBaseModel()
			stream.loadPropModel()
			stream.loadCompModel()
		}

	}  // Create C++ object
    //property var cmodel: stream.compModel

    Flickable {
        anchors {left:parent.left; leftMargin:rowSpacing; top:parent.top; topMargin:rowSpacing;
            right:parent.right; rightMargin:rowSpacing; bottom:parent.bottom; bottomMargin:rowSpacing}
        clip: true
        flickableDirection: Flickable.VerticalFlick
        contentHeight: mainColumn.height
        //contentWidth: mainColumn.width
        Column {
            id: mainColumn
            spacing:0

            Loader { sourceComponent: titleRow }
            Loader { sourceComponent: compositionTitle }
            Loader { sourceComponent: compositionSum }
            Repeater { // Create list of molecular compositions
                id: compositionList
				model: stream.compModel // Would like to use C++ model
				delegate: CompostionRow {
					value1: value
					compName: type
					isInput: isInput //isInput
					isVisible: (isVisable && showComposition)
                    editMenuIndex: uiStream.editMenuIndex
				}

//                model: compositionModel  //QML model
//                delegate: CompostionRow {
//                    value1: value
//                    compName: type
//                    isInput: isCompostionInput
//                    isVisible: (visableComposition && showComposition)
//                    editMenuIndex: uiStream.editMenuIndex
//            }
                Component.onCompleted: {
                //console.log("Mole comp model size is" + model.count)
            }
            }
            Repeater { // Create list of properties
                id: propertyList
				model: stream.propModel
//				model: propertyModel     // Would like to use C++ model
                delegate: StreamPropertyRow {
					id: idPropertyRow
					value1: value
					propName: type
					unitComboBoxModel: unitList
                    editMenuIndex: uiStream.editMenuIndex
                }
            }
        }
    }

    Component {
        id: titleRow
        Row {
            spacing: rowSpacing
            height: rowHeight

            TextEdit {
                id: titleText
                width: valueWidth + typeWidth + unitWidth - menuWidth
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                text: stream.title
            }

            Image {
                id: settingPic
                width: menuWidth
                height: menuWidth
                fillMode: Image.PreserveAspectFit
                source: (editMenuIndex == 0) ? "images/menu.png" : (editMenuIndex == 1) ? "images/eye.ico" : "images/pencil.png"

                MouseArea {
                    id: settingMouseArea
                    anchors.fill:parent
                    onClicked: {
                        editMenuIndex++
                        if (editMenuIndex > 2)
                            editMenuIndex = 0
                    }
                }
            }
        }
    }
    Component {
        id: compositionTitle
        Rectangle {
//            property bool rowVisible: true
//            property bool isReportable: true
            width: rowTotalWidth
            height: (visableComposition || editMenuIndex > 0) ? rowHeight + 4 : 0
            opacity: height==0 ? 0 : 1
            radius: 2
            border.width: 1
            border.color: isCompostionInput ? "blue" : "grey"
            Behavior on height {NumberAnimation {duration: 300}}
            enabled: height>0
            Row {
                id: compositionTitleRow
                spacing: rowSpacing
                height: rowHeight
                x:2; y:2
                Text {
                    id: compTitle
                    width: valueWidth + typeWidth + rowSpacing
                    height: rowHeight
                    text: "Composition"
                    color: showComposition ? "Black" : "White"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: showComposition -= 1
                    }
                    Rectangle {
                        z: -1
                        anchors.fill: parent
                        color: showComposition ? "White" : "Black"
                    }
                }
                ComboBox {
                    id: compUnitBox
                    width: unitWidth
                    height: rowHeight
					model: stream.compbase.unitList //compUnits
                    onCurrentIndexChanged: stream.setCompUnitIndex(currentIndex)
                }
                Image {
                    id: editPic
                    width: (editMenuIndex == 0) ? 0 : menuWidth
                    visible: width>0
                    height: menuWidth
                    source: (editMenuIndex == 1) ? "images/eye.ico" : "images/pencil.png"
                    opacity: (editMenuIndex == 1) ? (visableComposition ? 1 : 0.5) : (reportComposition ? 1 : 0.5)
                    fillMode: Image.PreserveAspectFit
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (editMenuIndex == 1)
                                visableComposition -= 1;
                            else
                                reportComposition -= 1;
                        }
                    }
                    Behavior on width {NumberAnimation {duration: 300}}
                }
            }
        }
    }

    Component {
        id: compositionSum
        Rectangle {
            width: rowTotalWidth
            height: (showComposition && visableComposition) ? rowHeight + 4 : 0
            enabled: height>0
            radius: 2
            border.width: 1
            border.color: isCompostionInput ? "blue" : "grey"
            //border.color: (Math.abs(stream.compSum-1.0)*Math.pow(10.0,precision) < 1.0) ? "grey" : "red"
            opacity: height==0 ? 0 : 1
            Behavior on height {NumberAnimation {duration: 300}}
            Row {
                id: compositionSumRow
                spacing: rowSpacing
                height: showComposition ? rowHeight : 0
                opacity: height==0 ? 0 : 1
                Behavior on height {NumberAnimation {duration: 300}}
                x:2; y:2
                Text {
                    id: compSumText
                    width: valueWidth + typeWidth + rowSpacing
                    height: parent.height
                    text: "Sum = " //+ stream.compSum.toLocaleString(Qt.locale(),'f', precision)
                }
                Rectangle {
                    id: compSumButton
                    width: unitWidth - rowHeight - rowSpacing
                    height: parent.height
                    color: "whitesmoke"
                    border.color: "grey"
                    border.width: 1
                    radius: 2
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: "normalize"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
//                            stream.normalize();
//                            console.log(" compList count = " + compList.count)
//                            for (var a=0; a<compList.count; a++)
//                                compList.itemAt(a).value1 = stream.compValue(a);
                        }
                    }
                }
            }
        }
    }
//    ListModel {
//        id:compUnits
//        ListElement { text: "mol frac" }
//        ListElement { text: "mol %" }
//        ListElement { text: "weight frac" }
//        ListElement { text: "weight %" }
//        ListElement { text: "lb-mol/hr" }
//        ListElement { text: "mols/hr" }
//        ListElement { text: "lb/hr" }
//        ListElement { text: "kg/hr" }
//    }
    property var compUnits: ["mol frac", "mol %", "weight frac", "weight %", "lb-mol/hr", "mols/hr", "lb/hr", "kg/hr"]

    ListModel {
        id: compositionModel
        ListElement {
            type: "CH4"
            value: 0.7685
        }
        ListElement {
            type: "C3"
            value: 0.1524
        }
        ListElement {
            type: "C4"
            value: 0.0834
        }
        ListElement {
            type: "C5"
            value: 0.01
        }
    }

    ListModel {
        id: propertyModel
        ListElement {
            type: "P"
            value: 14.7
            unitList: [
                ListElement { text: "psig"},
                ListElement { text: "psia"},
                ListElement { text: "atm"},
                ListElement { text: "bar"},
                ListElement { text: "kPa"},
                ListElement { text: "inHg"}
            ]
        }
        ListElement {
            type: "T"
            value: 60.0
            unitList: [
                ListElement { text: "F"},
                ListElement { text: "R"},
                ListElement { text: "C"},
                ListElement { text: "K"}
            ]
        }
        ListElement {
            type: "Q"
            value: 100.0
            unitList: [
                ListElement { text: "MMscfd"},
                ListElement { text: "lb/hr"},
                ListElement { text: "lb-mols/hr"},
                ListElement { text: "mols/hr"}
            ]
        }
        ListElement {
            type: "MW"
            value: 23.42
            unitList: [
                ListElement { text: "lb/lb-mol"},
                ListElement { text: "g/mol"},
                ListElement { text: "kg/kmol"}
            ]
        }
        ListElement {
            type: "z"
            value: 0.83
            unitList: []
        }
        ListElement {
            type: "rho"
            value: 0.3572
            unitList: [
                ListElement { text: "lb/ft^3"},
                ListElement { text: "kg/m^3"},
                ListElement { text: "lb/in^3"}
            ]
        }
        ListElement {
            type: "h"
            value: 127.85
            unitList: [
                ListElement { text: "Btu/lb"},
                ListElement { text: "kJ/kg"}
            ]
        }
        ListElement {
            type: "s"
            value: 74.687
            unitList: [
                ListElement { text: "Btu/lb-F"},
                ListElement { text: "kJ/kg-C"}
            ]
        }
        ListElement {
            type: "LHV"
            value: 5240
            unitList: [
                ListElement { text: "Btu/lb"},
                ListElement { text: "kJ/kg"}
            ]
        }
    }
}
