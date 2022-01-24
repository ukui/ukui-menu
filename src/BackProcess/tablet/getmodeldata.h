#ifndef GETMODELDATA_H
#define GETMODELDATA_H
#include "ukuimenuinterface.h"
#include <peony-qt/file-enumerator.h>
#include <peony-qt/file-info-job.h>
#include <peony-qt/file-info.h>

class GetModelData : public QObject
{
    Q_OBJECT

public:
    GetModelData();

public:
    /**
     * @brief 获取默认窗口全部分类数据
     * @return
     */
    QVector<QStringList> getMinAllData();
    /**
     * @brief 获取默认窗口字母分类数据
     * @return
     */
    QVector<QStringList> getMinLetterData();
    /**
     * @brief 获取默认窗口功能分类数据
     * @return
     */
    QVector<QStringList> getMinFuncData();
    /**
     * @brief 获取收藏列表数据
     * @return
     */
    QStringList getcollectData();
    /**
     * @brief 获取功能分类类别列表
     * @return
     */
    QStringList getFuncClassificationList();
    /**
     * @brief 获取字母分类类别列表
     * @return
     */
    QStringList getLetterClassificationList();
    /**
     * @brief 获取对应类别所在行数
     * @return
     */
    QStringList getFuncClassificationBtnRowList();
    QStringList getLetterClassificationBtnRowList();
    /**
     * @brief 获取最近页面所展示应用列表
     * @return
     */
    QVector<QStringList> getRecentData();

    /**
     * @brief 重新加载各个部分数据，用于初始化和整体更新
     */
    void loadDesktopVercor();

protected:
    /**
     * @brief 插入分类按键
     * @param btnname
     */
    void insertClassificationBtn(QString btnname);
    /**
     * @brief 插入应用列表
     * @param appnamelist
     */
    void insertAppList(QStringList appnamelist);
    /**
     * @brief 修改时间比较
     * @param arg_1
     * @param arg_2
     * @return
     */
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
