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
            //width: parent.width - searchButton.width
            width: parent.width

            Image {
                id: clearText
                source: 'image://theme/icon-m-input-clear' // icon-m-common-search
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        searchInput.text = "";
                        filter = "";
                        fillModel();
                        searchfield.filtered();
                    }
                }
            }
            maximumLength: 50

            Keys.onPressed: {
                if (event.key == Qt.Key_Return || event.key == Qt.Key_Enter){
                    filter = searchInput.text;
                    filter = filter.replace(/\s*$/g,"");
                    filter = filter.replace(/^\s*/g,"");
                    searchfield.find();
                }
                event.accepted = true;
                parent.focus = true;
            }

        }
        /*
        Button {
            id: searchButton
            text: "Search"
            width: 100
            onClicked: {
                console.log("click = "+searchInput.text);
                filter = searchInput.text;
                searchfield.find();
            }
        }*/
    }
}

