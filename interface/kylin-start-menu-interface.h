#ifndef KYLINSTARTMENUINTERFACE_H
#define KYLINSTARTMENUINTERFACE_H
#include <QtCore>

class KylinStartMenuInterface
{
private:
    static QStringList filePathList;
    static void recursive_search_file(const QString& _filePath);//遍历/usr/share/applications/文件夹
    static QStringList get_specified_category_app_list(QString categorystr);//获取指定类型应用列表
    static void remove_repetition_appname(QStringList &list,QStringList cmplist);//移除重复应用名


public:
    KylinStartMenuInterface();

    /**
     * 获取系统应用名称
     * @param desktopfp 为应用.desktop文件所在路径
     * @return 返回应用名称
     */
    static QString get_app_name(QString desktopfp);
    static QString get_app_icon(QString desktopfp);//获取应用图像
    static QString get_app_categories(QString desktopfp);//获取应用分类
    static QString get_app_exec(QString desktopfp);//获取应用命令
    static QString get_app_type(QString desktopfp);//获取应用类型
    static QString get_app_comment(QString desktopfp);//获取应用注释

    static QStringList get_desktop_file_path();//获取系统deskyop文件路径

    static QString get_desktop_path_by_app_name(QString appname);//根据应用名获取deskyop文件路径

    static QStringList sort_app_name();//应用排序

    static QString get_app_name_pinyin(QString appname);//获取应用名拼音

    //功能分类
    static QStringList get_recent_app_list();
    static QStringList get_network_app_list();
    static QStringList get_social_app_list();
    static QStringList get_av_app_list();
    static QStringList get_develop_app_list();
    static QStringList get_graphics_app_list();
    static QStringList get_game_app_list();
    static QStringList get_office_app_list();
    static QStringList get_education_app_list();
    static QStringList get_systemadmin_app_list();
    static QStringList get_other_app_list();


};

#endif // KYLINSTARTMENUINTERFACE_H
