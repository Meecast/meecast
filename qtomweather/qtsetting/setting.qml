import Qt 4.7

Rectangle {
    width: 640
    height: 480

    ListModel {
        id: testmodel
        ListElement {
            key: 0
            content: "One"
        }
        ListElement {
            key: 1
            content: "Two"
        }
        ListElement {
            key: 2
            content: "Three"
        }
        ListElement {
            key: 3
            content: "One"
        }
        ListElement {
            key: 4
            content: "Two"
        }
        ListElement {
            key: 5
            content: "Three"
        }
        ListElement {
            key: 6
            content: "One"
        }
        ListElement {
            key: 7
            content: "Two"
        }
        ListElement {
            key: 8
            content: "Three"
        }
    }
    ComboBox {
        text: "test"
        key: "";
        model: testmodel
        onClicked: {console.log("result = " + key + " " + text);}
    }
}
