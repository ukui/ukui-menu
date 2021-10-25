#ifndef GETMODELDATA_H
#define GETMODELDATA_H
#include "src/Interface/ukuimenuinterface.h"
#include <peony-qt/file-enumerator.h>
#include <peony-qt/file-info-job.h>
#include <peony-qt/file-info.h>

class GetModelData : public QObject
{
    Q_OBJECT

public:
    GetModelData();

public:
    QVector<QStringList> getMinAllData();
    QVector<QStringList> getMinLetterData();
    QVector<QStringList> getMinFuncData();
    QStringList getcollectData();
    QStringList getFuncClassificationList();
    QStringList getLetterClassificationList();
    QStringList getFuncClassificationBtnRowList();
    QStringList getLetterClassificationBtnRowList();
    QVector<QStringList> getRecentData();

    void loadDesktopVercor();

protected:
    void insertClassificationBtn(QString btnname);
    void insertAppList(QStringList appnamelist);
    static bool cmpApp(QStringList &arg_1, QStringList &arg_2);

private:
    UkuiMenuInterface *m_ukuiMenuInterface = nullptr;
    QVector<QStringList> m_minAllData;
    QStringList m_collectData;
    QVector<QStringList> m_minLetterData;
    QStringList m_letterList;
    QStringList m_letterBtnRowList;
    QVector<QStringList> m_minFuncData;
    QStringList m_classificationList;
    QStringList m_classificationBtnRowList;
    int m_funcRow;
    Peony::FileEnumerator *enumerator = nullptr;
    QList<std::shared_ptr<Peony::FileInfo>> m_childrens;
};

#endif // GETMODELDATA_H
