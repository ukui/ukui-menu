#ifndef GETMODELDATA_H
#define GETMODELDATA_H
#include "src/Interface/ukuimenuinterface.h"

class GetModelData : public QObject
{
    Q_OBJECT

public:
    GetModelData();

public:
    QVector<QStringList> getMinAllData();
    QVector<QStringList> getMinLetterData();
    QVector<QStringList> getMinFuncData();
    QVector<QString> getcollectData();
    QStringList getFuncClassificationList();
    QStringList getLetterClassificationList();
    QStringList getFuncClassificationBtnRowList();
    QStringList getLetterClassificationBtnRowList();

    void loadDesktopVercor();

protected:
    void insertClassificationBtn(QString btnname);
    void insertAppList(QStringList appnamelist);

private:
    UkuiMenuInterface *m_ukuiMenuInterface = nullptr;
    QVector<QStringList> m_minAllData;
    QVector<QString> m_collectData;
    QVector<QStringList> m_minLetterData;
    QStringList m_letterList;
    QStringList m_letterBtnRowList;
    QVector<QStringList> m_minFuncData;
    QStringList m_classificationList;
    QStringList m_classificationBtnRowList;
    int m_funcRow;
};

#endif // GETMODELDATA_H
