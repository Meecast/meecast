import Qt 4.7

Rectangle {
    width: 640
    height: 480

    ListModel {
        id: testmodel
        ListElement {
            content: "One"
        }
        ListElement {
            content: "Two"
        }
        ListElement {
            content: "Three"
        }
    }
    ComboBox {
        text: "test"
        model: testmodel
    }
}
