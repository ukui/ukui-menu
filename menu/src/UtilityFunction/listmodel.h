#ifndef LISTMODEL_H
#define LISTMODEL_H
#include <QAbstractListModel>
#include <QIcon>
#include <ukuimenuinterface.h>

class ListModel : public QAbstractListModel
{
public:
    ListModel(QObject* parent);
    ~ListModel();
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    QVariant data(const QModelIndex &index, int role) const;
    void setData(const QVector<QStringList>& data);
    void updateData(const QVector<QStringList>& data);

private:
    QVector<QStringList> m_data;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
};

#endif // LISTMODEL_H
