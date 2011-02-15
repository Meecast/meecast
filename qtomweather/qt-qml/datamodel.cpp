#include "datamodel.h"

DataModel::DataModel(DataItem* prototype, QObject *parent) :
    QAbstractListModel(parent), _prototype(prototype)
{
    setRoleNames(_prototype->roleNames());
}

int
DataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _list.size();
}

QVariant
DataModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _list.size())
        return QVariant();
    return _list.at(index.row())->data(role);
}

DataModel::~DataModel()
{
    delete _prototype;
}

void
DataModel::clear()
{
    qDeleteAll(_list);
    _list.clear();
    //emit dataChanged(this->createIndex(0, 0), this->createIndex(count, 0));
    this->reset();
}

void
DataModel::appendRow(DataItem *item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _list.append(item);
    endInsertRows();
    //return (_list.size() - 1);
}
void
DataModel::update(QString filename)
{
    this->clear();
    DataItem *forecast_data = NULL;

    int i = 0;
    Core::DataParser* dp = NULL;
    Core::Data *temp_data = NULL;
    if (!filename.isEmpty()){
        try{
            dp = new Core::DataParser(filename.toStdString(),
                                      Core::AbstractConfig::prefix+Core::AbstractConfig::schemaPath+"data.xsd");
        }
        catch(const std::string &str){
            std::cerr<<"Error in DataParser class: "<< str << std::endl;
            //return NULL;
        }
        catch(const char *str){
            std::cerr<<"Error in DataParser class: "<< str << std::endl;
            //return NULL;
        }
    }

    while  (dp != NULL && (temp_data = dp->data().GetDataForTime(time(NULL) + i))) {
        i = i + 3600*24;
        forecast_data = new DataItem(temp_data);
        forecast_data->Text(forecast_data->Text().c_str());
        this->appendRow(forecast_data);

    }
    this->reset();
}
