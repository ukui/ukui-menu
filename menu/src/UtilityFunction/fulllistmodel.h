#ifndef FULLLISTMODEL_H
#define FULLLISTMODEL_H
#include <QAbstractListModel>
#include <QIcon>
#include <ukuimenuinterface.h>

class FullListModel : public QAbstractListModel
{
public:
    FullListModel(QObject* parent);
    ~FullListModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void removeRow(QString desktopfp);
    void insertRow(QString desktopfp);
    QVariant data(const QModelIndex &index, int role) const;
    void setData(QStringList data);
    void updateData(QStringList data);

private:
    QStringList m_data;
    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
};

#endif // FULLLISTMODEL_H
