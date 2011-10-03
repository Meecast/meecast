import Qt 4.7
import com.nokia.meego 1.0

Item {
    id: searchfield
    width: parent.width

    property QtObject model: undefined
    property QtObject fullModel
    property QtObject filtermodel: ListModel{}

    property string filter: ""

    signal filtered();

    Component.onCompleted: {
        fullModel = model;
        fillModel();

    }
    function fillModel()
    {
        filtermodel.clear();
        for (var i = 0; i < fullModel.count; i++) {
            filtermodel.append(fullModel.get(i))
        }
    }
    function find()
    {
        fillModel();
        if (filter.length > 0){
            for (var i=filtermodel.count-1; i >= 0; i--){
                if (filtermodel.get(i).name.indexOf(filter) != 0){
                    filtermodel.remove(i);

                }
            }
            searchfield.filtered();
        }
    }

    Row {
        width: parent.width
        TextField {
            id: searchInput
            placeholderText: Config.tr("Enter search")
            width: parent.width - searchButton.width

            Image {
                id: clearText
                source: 'image://theme/icon-m-input-clear' // icon-m-common-search
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        filter = "";
                        fillModel();
                        searchfield.filtered();
                    }
                }
            }
            maximumLength: 50

            Keys.onPressed: {
                console.log("pressed "+event.key+" text = "+searchInput.text);
                event.accepted = true;
            }

        }
        Button {
            id: searchButton
            text: "Search"
            width: 100
            onClicked: {
                console.log("click = "+searchInput.text);
                filter = searchInput.text;
                searchfield.find();
            }
        }
    }
}

