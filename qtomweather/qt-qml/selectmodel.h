#ifndef SELECTMODEL_H
#define SELECTMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class SelectData
{
public:
    SelectData(const QString &name, const QString &key);

    QString name() const;
    QString key() const;
private:
    QString _name;
    QString _key;
};

class SelectModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SelectModelRoles {
        NameRole = Qt::UserRole + 1,
        KeyRole
    };
    SelectModel(QObject *parent = 0);

    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const;
    void addData(const SelectData &data);
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
private:
    QList<SelectData> _list;
};

#endif // SELECTMODEL_H
