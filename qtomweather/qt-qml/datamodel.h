#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include "dataitem.h"

class DataModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit DataModel(DataItem* prototype, QObject* parent = 0);
    ~DataModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void appendRow(DataItem* item);
    void clear();
    Q_INVOKABLE void update();

private:
    DataItem* _prototype;
    QList<DataItem*>_list;
};

#endif // DATAMODEL_H
