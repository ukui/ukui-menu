/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#ifndef UKUIMENUINTERFACE_H
#define UKUIMENUINTERFACE_H
#include <QtCore>
#include <sys/types.h>
#include <unistd.h>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QSettings>

class UkuiMenuInterface
{
private:
    QStringList filePathList;
    QSettings* setting=nullptr;

protected:
    void recursive_search_file(const QString& _filePath);//遍历/usr/share/applications/文件夹
    QStringList get_specified_category_app_list(QString categorystr);//获取指定类型应用列表

public:
    UkuiMenuInterface();
    ~UkuiMenuInterface();
    QVector<QStringList> create_appinfo_vector();//创建应用信息容器
    static QVector<QStringList> appInfoVector;
    static QVector<QString> desktopfpVector;

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

    //获取用户图像
    QString get_user_icon();
    //获取用户姓名
    QString get_user_name();

};

#endif // UKUIMENUINTERFACE_H
