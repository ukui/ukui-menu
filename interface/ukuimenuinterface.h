#ifndef UKUIMENUINTERFACE_H
#define UKUIMENUINTERFACE_H
#include <QtCore>
#include <sys/types.h>
#include <unistd.h>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusObjectPath>
//#include "appinfo.h"

class AppInfo
{
public:
    QString desktopfp;
    QString icon;
    QString name;
    QStringList categories;
    QString exec;
    QString comment;
};

class UkuiMenuInterface : public AppInfo
{
private:
    QStringList filePathList;

protected:
    void recursive_search_file(const QString& _filePath);//遍历/usr/share/applications/文件夹
    QStringList get_specified_category_app_list(QString categorystr);//获取指定类型应用列表
    void remove_repetition_appname(QStringList &list,QStringList cmplist);//移除重复应用名

public:
    UkuiMenuInterface();
    ~UkuiMenuInterface();
    AppInfo* create_appinfo_object();//创建AppInfo对象
    static AppInfo* pAppInfo;


    /**
     * 获取系统应用名称
     * @param desktopfp 为应用.desktop文件所在路径
     * @return 返回应用名称
     */
    QString get_app_name(QString desktopfp);//获取应用名
    QString get_app_english_name(QString desktopfp);//获取英语英文名
    QString get_app_icon(QString desktopfp);//获取应用图像
    QString get_app_categories(QString desktopfp);//获取应用分类
    QString get_app_exec(QString desktopfp);//获取应用命令
    QString get_app_type(QString desktopfp);//获取应用类型
    QString get_app_comment(QString desktopfp);//获取应用注释

    QStringList get_desktop_file_path();//获取系统deskyop文件路径

    QString get_desktop_path_by_app_name(QString appname);//根据应用名获取deskyop文件路径
    QString get_desktop_path_by_app_english_name(QString appname);//根据应用英文名获取deskyop文件路径

    QStringList sort_app_name();//应用排序
    QVector<QStringList> get_alphabetic_classification();//获取字母分类
    QVector<QStringList> get_functional_classification();//获取功能分类
    bool matching_app_categories(QString desktopfp,QStringList categorylist);//匹配应用Categories

    QString get_app_name_pinyin(QString appname);//获取应用名拼音

    QStringList get_app_diff_first_letter_pos();

    //功能分类
    QStringList get_recent_app_list();
    QStringList get_network_app_list();
    QStringList get_social_app_list();
    QStringList get_av_app_list();
    QStringList get_develop_app_list();
    QStringList get_graphics_app_list();
    QStringList get_game_app_list();
    QStringList get_office_app_list();
    QStringList get_education_app_list();
    QStringList get_systemadmin_app_list();
    QStringList get_other_app_list();

    //获取用户图像
    QString get_user_icon();
    //获取用户姓名
    QString get_user_name();

};

#endif // UKUIMENUINTERFACE_H
