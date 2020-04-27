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

#include "ukuimenuinterface.h"
#include <glib.h>
#include <QDir>
#include <QDebug>
#include <QCollator>
#include <QLocale>
#include <QStringList>
#include "ukuichineseletter.h"

UkuiMenuInterface::UkuiMenuInterface()
{
    QString path=QDir::homePath()+"/.config/ukui/ukui-menu.ini";
    setting=new QSettings(path,QSettings::IniFormat);
}

UkuiMenuInterface::~UkuiMenuInterface()
{
//    delete[] pAppInfo;
}

//文件递归查询
void UkuiMenuInterface::recursiveSearchFile(const QString& _filePath)
{
    GError** error=nullptr;
    GKeyFileFlags flags=G_KEY_FILE_NONE;
    GKeyFile* keyfile=g_key_file_new ();

    QDir dir(_filePath);
    if (!dir.exists()) {
        return;
    }

    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    list.removeAll(QFileInfo("/usr/share/applications/screensavers"));
    if(list.size()< 1 ) {
        return;
    }
    int i=0;

    //递归算法的核心部分
    do{
        QFileInfo fileInfo = list.at(i);
        //如果是文件夹，递归
        bool isDir = fileInfo.isDir();
        if(isDir) {
            recursiveSearchFile(fileInfo.filePath());
        }
        else{
            //过滤后缀不是.desktop的文件
            QString filePathStr=fileInfo.filePath();
            QFileInfo fileinfo(filePathStr);
            QString file_suffix=fileinfo.suffix();
            if(QString::compare(file_suffix,"desktop")!=0)
            {
                i++;
                continue;
            }
            QByteArray fpbyte=filePathStr.toLocal8Bit();
            char* filepath=fpbyte.data();
            g_key_file_load_from_file(keyfile,filepath,flags,error);
            char* ret_1=g_key_file_get_locale_string(keyfile,"Desktop Entry","NoDisplay", nullptr, nullptr);
            if(ret_1!=nullptr)
            {
                QString str=QString::fromLocal8Bit(ret_1);
                if(QString::compare(str, "true")==0)
                {
                    i++;
                    continue;
                }
            }
            //过滤LXQt、KDE
            char* ret=g_key_file_get_locale_string(keyfile,"Desktop Entry","OnlyShowIn", nullptr, nullptr);
            if(ret!=nullptr)
            {
                QString str=QString::fromLocal8Bit(ret);
                if(QString::compare(str, "LXQt;")==0 || QString::compare(str, "KDE;")==0)
                {
                    i++;
                    continue;
                }
            }
            //过滤中英文名为空的情况
            QLocale cn;
            QString language=cn.languageToString(cn.language());
            if(QString::compare(language,"Chinese")==0)
            {
                char* nameCh=g_key_file_get_string(keyfile,"Desktop Entry","Name[zh_CN]", nullptr);
                char* nameEn=g_key_file_get_string(keyfile,"Desktop Entry","Name", nullptr);
                if(QString::fromLocal8Bit(nameCh).isEmpty() && QString::fromLocal8Bit(nameEn).isEmpty())
                {
                    i++;
                    continue;
                }
            }
            else {
                char* name=g_key_file_get_string(keyfile,"Desktop Entry","Name", nullptr);
                if(QString::fromLocal8Bit(name).isEmpty())
                {
                    i++;
                    continue;
                }
            }

            filePathList.append(filePathStr);
        }
        i++;

    } while(i < list.size());

    g_key_file_free(keyfile);

}

//获取系统deskyop文件路径
QStringList UkuiMenuInterface::getDesktopFilePath()
{
    filePathList.clear();
    recursiveSearchFile("/usr/share/applications/");

    filePathList.removeAll("/usr/share/applications/peony-folder-handler.desktop");
    filePathList.removeAll("/usr/share/applications/gnome-software-local-file.desktop");
    filePathList.removeAll("/usr/share/applications/org.gnome.Software.Editor.desktop");
    filePathList.removeAll("/usr/share/applications/apport-gtk.desktop");
    filePathList.removeAll("/usr/share/applications/software-properties-livepatch.desktop");
    filePathList.removeAll("/usr/share/applications/snap-handle-link.desktop");
    filePathList.removeAll("/usr/share/applications/python3.7.desktop");
    filePathList.removeAll("/usr/share/applications/rhythmbox-device.desktop");
    filePathList.removeAll("/usr/share/applications/smplayer_enqueue.desktop");
    filePathList.removeAll("/usr/share/applications/python2.7.desktop");
    filePathList.removeAll("/usr/share/applications/mate-color-select.desktop");
    filePathList.removeAll("/usr/share/applications/shotwell-viewer.desktop");
    filePathList.removeAll("/usr/share/applications/burner-nautilus.desktop");
    filePathList.removeAll("/usr/share/applications/gnome-system-monitor-kde.desktop");
    filePathList.removeAll("/usr/share/applications/hplj1020.desktop");
    filePathList.removeAll("/usr/share/applications/ukui-network-scheme.desktop");
    filePathList.removeAll("/usr/share/applications/ukui-panel.desktop");
    filePathList.removeAll("/usr/share/applications/unity-activity-log-manager-panel.desktop");
    filePathList.removeAll("/usr/share/applications/blueman-adapters.desktop");
    filePathList.removeAll("/usr/share/applications/fcitx-config-gtk3.desktop");
    filePathList.removeAll("/usr/share/applications/im-config.desktop");
    filePathList.removeAll("/usr/share/applications/fcitx-skin-installer.desktop");
    filePathList.removeAll("/usr/share/applications/gcr-prompter.desktop");
    filePathList.removeAll("/usr/share/applications/gcr-viewer.desktop");
    filePathList.removeAll("/usr/share/applications/geoclue-demo-agent.desktop");
    filePathList.removeAll("/usr/share/applications/gnome-disk-image-mounter.desktop");
    filePathList.removeAll("/usr/share/applications/gnome-disk-image-writer.desktop");
    filePathList.removeAll("/usr/share/applications/libreoffice-xsltfilter.desktop");
    filePathList.removeAll("/usr/share/applications/peony-autorun-software.desktop");
    filePathList.removeAll("/usr/share/applications/remmina-file.desktop");
    filePathList.removeAll("/usr/share/applications/remmina-gnome.desktop");
    filePathList.removeAll("/usr/share/applications/ukwm.desktop");
    filePathList.removeAll("/usr/share/applications/nm-applet.desktop");
    filePathList.removeAll("/usr/share/applications/mate-user-guide.desktop");
    filePathList.removeAll("/usr/share/applications/nm-connection-editor.desktop");
    filePathList.removeAll("/usr/share/applications/pavucontrol-qt.desktop");
    filePathList.removeAll("/usr/share/applications/ukui-volume-control.desktop");
    filePathList.removeAll("/usr/share/applications/lximage-qt-screenshot.desktop");
    filePathList.removeAll("/usr/share/applications/lximage-qt.desktop");
    filePathList.removeAll("/usr/share/applications/appurl.desktop");
    filePathList.removeAll("/usr/share/applications/debian-uxterm.desktop");
    filePathList.removeAll("/usr/share/applications/debian-xterm.desktop");
    filePathList.removeAll("/usr/share/applications/fcitx-ui-sogou-qimpanel.desktop");
    filePathList.removeAll("/usr/share/applications/fcitx.desktop");
    filePathList.removeAll("/usr/share/applications/fcitx-configtool.desktop");
    filePathList.removeAll("/usr/share/applications/fcitx-qimpanel-configtool.desktop");
    filePathList.removeAll("/usr/share/applications/peony-computer.desktop");
    filePathList.removeAll("/usr/share/applications/onboard-settings.desktop");
    filePathList.removeAll("/usr/share/applications/xscreensaver-properties.desktop");
    filePathList.removeAll("/usr/share/applications/info.desktop");
    filePathList.removeAll("/usr/share/applications/mate-about.desktop");
    filePathList.removeAll("/usr/share/applications/pcmanfm-qt.desktop");
    filePathList.removeAll("/usr/share/applications/qlipper.desktop");
    filePathList.removeAll("/usr/share/applications/ktelnetservice5.desktop");
    filePathList.removeAll("/usr/share/applications/ukui-power-preferences.desktop");
    filePathList.removeAll("/usr/share/applications/ukui-power-statistics.desktop");
    filePathList.removeAll("/usr/share/applications/software-properties-drivers.desktop");
    filePathList.removeAll("/usr/share/applications/software-properties-gtk.desktop");
    filePathList.removeAll("/usr/share/applications/galternatives.desktop");
    filePathList.removeAll("/usr/share/applications/gnome-session-properties.desktop");
    filePathList.removeAll("/usr/share/applications/pcmanfm-qt-desktop-pref.desktop");
    filePathList.removeAll("/usr/share/applications/org.gnome.font-viewer.desktop");
    filePathList.removeAll("/usr/share/applications/gucharmap.desktop");
    filePathList.removeAll("/usr/share/applications/xdiagnose.desktop");
    filePathList.removeAll("/usr/share/applications/gnome-language-selector.desktop");
    filePathList.removeAll("/usr/share/applications/indicator-china-weather.desktop");
    filePathList.removeAll("/usr/share/applications/mate-notification-properties.desktop");
    filePathList.removeAll("/usr/share/applications/transmission-gtk.desktop");
    filePathList.removeAll("/usr/share/applications/mpv.desktop");
    filePathList.removeAll("/usr/share/applications/atril.desktop");
    filePathList.removeAll("/usr/share/applications/org.kde.kwalletmanager5.desktop");
    filePathList.removeAll("/usr/share/applications/system-config-printer.desktop");
    filePathList.removeAll("/usr/share/applications/vim.desktop");
    filePathList.removeAll("/usr/share/applications/kwalletmanager5-kwalletd.desktop");
    filePathList.removeAll("/usr/share/applications/org.gnome.DejaDup.desktop");
    filePathList.removeAll("/usr/share/applications/redshift.desktop");
    filePathList.removeAll("/usr/share/applications/python3.8.desktop");
    filePathList.removeAll("/usr/share/applications/yelp.desktop");
    filePathList.removeAll("/usr/share/applications/peony-computer.desktop");
    filePathList.removeAll("/usr/share/applications/peony-home.desktop");
    filePathList.removeAll("/usr/share/applications/peony-trash.desktop");
    filePathList.removeAll("/usr/share/applications/peony.desktop");
    return filePathList;
}

QVector<QStringList> UkuiMenuInterface::appInfoVector=QVector<QStringList>();
QVector<QString> UkuiMenuInterface::desktopfpVector=QVector<QString>();
QVector<QStringList> UkuiMenuInterface::alphabeticVector=QVector<QStringList>();
QVector<QStringList> UkuiMenuInterface::functionalVector=QVector<QStringList>();
QVector<QString> UkuiMenuInterface::commonUseVector=QVector<QString>();

//创建应用信息容器
QVector<QStringList> UkuiMenuInterface::createAppInfoVector()
{
    desktopfpVector.clear();
    QVector<QStringList> appInfoVector;
    QVector<QStringList> vector;
    vector.append(QStringList()<<"Network");//1网络
    vector.append(QStringList()<<"Messaging");//2社交
    vector.append(QStringList()<<"Audio"<<"Video");//3影音
    vector.append(QStringList()<<"Development");//4开发
    vector.append(QStringList()<<"Graphics");//5图像
    vector.append(QStringList()<<"Game");//6游戏
    vector.append(QStringList()<<"Office"<<"Calculator"<<"Spreadsheet"<<"Presentation"<<"WordProcessor");//7办公
    vector.append(QStringList()<<"Education");//8教育
    vector.append(QStringList()<<"System"<<"Settings"<<"Security");//9系统

    QStringList desktopfpList=getDesktopFilePath();

    for(int i=0;i<desktopfpList.count();i++)
    {
        QStringList appInfoList;
        QString desktopfp=desktopfpList.at(i);
        QString icon=getAppIcon(desktopfpList.at(i));
        QString name=getAppName(desktopfpList.at(i));
        QString exec=getAppExec(desktopfpList.at(i));
        QString comment=getAppComment(desktopfpList.at(i));

        desktopfpVector.append(desktopfp);

        appInfoList<<desktopfp<<icon<<name<<exec<<comment;
        bool is_owned=false;
        for(int j=0;j<vector.size();j++)
        {
            if(matchingAppCategories(desktopfpList.at(i),vector.at(j)))//有对应分类
            {
                is_owned=true;
                appInfoList.append(QString::number(j+1));
            }
        }
        if(!is_owned)//该应用无对应分类
            appInfoList.append(QString::number(10));

        appInfoVector.append(appInfoList);
    }

    return appInfoVector;
}

//获取应用名称
QString UkuiMenuInterface::getAppName(QString desktopfp)
{
    GError** error=nullptr;
    GKeyFileFlags flags=G_KEY_FILE_NONE;
    GKeyFile* keyfile=g_key_file_new ();

    QByteArray fpbyte=desktopfp.toLocal8Bit();
    char* filepath=fpbyte.data();
    g_key_file_load_from_file(keyfile,filepath,flags,error);

    char* name=g_key_file_get_locale_string(keyfile,"Desktop Entry","Name", nullptr, nullptr);
    QString namestr=QString::fromLocal8Bit(name);

//    QString namestr;
//    QLocale cn;
//    QString language=cn.languageToString(cn.language());
//    if(QString::compare(language,"Chinese")==0)
//    {
//        char* name;
//        name=g_key_file_get_locale_string(keyfile,"Desktop Entry","Name[zh_CN]", nullptr, nullptr);
//        if(QString::fromLocal8Bit(name).isEmpty())
//            name=g_key_file_get_locale_string(keyfile,"Desktop Entry","Name", nullptr, nullptr);
//        namestr=QString::fromLocal8Bit(name);
//    }
//    else {
//        char* name=g_key_file_get_locale_string(keyfile,"Desktop Entry","Name", nullptr, nullptr);
//        namestr=QString::fromLocal8Bit(name);
//    }

    g_key_file_free(keyfile);
    return namestr;
}

//获取英应用英文名
QString UkuiMenuInterface::getAppEnglishName(QString desktopfp)
{
    GError** error=nullptr;
    GKeyFileFlags flags=G_KEY_FILE_NONE;
    GKeyFile* keyfile=g_key_file_new ();

    QByteArray fpbyte=desktopfp.toLocal8Bit();
    char* filepath=fpbyte.data();
    g_key_file_load_from_file(keyfile,filepath,flags,error);
    char* name=g_key_file_get_string(keyfile,"Desktop Entry","Name", nullptr);
    QString namestr=QString::fromLocal8Bit(name);
    return namestr;
}

//获取应用分类
QString UkuiMenuInterface::getAppCategories(QString desktopfp)
{
    GError** error=nullptr;
    GKeyFileFlags flags=G_KEY_FILE_NONE;
    GKeyFile* keyfile=g_key_file_new ();

    QByteArray fpbyte=desktopfp.toLocal8Bit();
    char* filepath=fpbyte.data();
    g_key_file_load_from_file(keyfile,filepath,flags,error);
    char* category=g_key_file_get_locale_string(keyfile,"Desktop Entry","Categories", nullptr, nullptr);
    g_key_file_free(keyfile);
    return QString::fromLocal8Bit(category);
}

//获取应用图标
QString UkuiMenuInterface::getAppIcon(QString desktopfp)
{   
    GError** error=nullptr;
    GKeyFileFlags flags=G_KEY_FILE_NONE;
    GKeyFile* keyfile=g_key_file_new ();

    QByteArray fpbyte=desktopfp.toLocal8Bit();
    char* filepath=fpbyte.data();
    g_key_file_load_from_file(keyfile,filepath,flags,error);
    char* icon=g_key_file_get_locale_string(keyfile,"Desktop Entry","Icon", nullptr, nullptr);
    g_key_file_free(keyfile);
    return QString::fromLocal8Bit(icon);
}

//获取应用命令
QString UkuiMenuInterface::getAppExec(QString desktopfp)
{
    GError** error=nullptr;
    GKeyFileFlags flags=G_KEY_FILE_NONE;
    GKeyFile* keyfile=g_key_file_new ();

    QByteArray fpbyte=desktopfp.toLocal8Bit();
    char* filepath=fpbyte.data();
    g_key_file_load_from_file(keyfile,filepath,flags,error);
    char* exec=g_key_file_get_locale_string(keyfile,"Desktop Entry","Exec", nullptr, nullptr);
    g_key_file_free(keyfile);
    return QString::fromLocal8Bit(exec);
}

//获取应用注释
QString UkuiMenuInterface::getAppComment(QString desktopfp)
{
    GError** error=nullptr;
    GKeyFileFlags flags=G_KEY_FILE_NONE;
    GKeyFile* keyfile=g_key_file_new ();

    QByteArray fpbyte=desktopfp.toLocal8Bit();
    char* filepath=fpbyte.data();
    g_key_file_load_from_file(keyfile,filepath,flags,error);
    char* comment=g_key_file_get_locale_string(keyfile,"Desktop Entry","Comment", nullptr, nullptr);
    g_key_file_free(keyfile);
    return QString::fromLocal8Bit(comment);

}

//获取应用类型
QString UkuiMenuInterface::getAppType(QString desktopfp)
{
    GError** error=nullptr;
    GKeyFileFlags flags=G_KEY_FILE_NONE;
    GKeyFile* keyfile=g_key_file_new ();

    QByteArray fpbyte=desktopfp.toLocal8Bit();
    char* filepath=fpbyte.data();
    g_key_file_load_from_file(keyfile,filepath,flags,error);
    char* type=g_key_file_get_locale_string(keyfile,"Desktop Entry","Type", nullptr, nullptr);
    g_key_file_free(keyfile);
    return QString::fromLocal8Bit(type);
}

//根据应用名获取deskyop文件路径
QString UkuiMenuInterface::getDesktopPathByAppName(QString appname)
{
    QString desktopfp;
    int index=0;
    while(index<appInfoVector.size())
    {
        if(QString::compare(appInfoVector.at(index).at(2),appname)==0)
        {
            desktopfp=appInfoVector.at(index).at(0);
            break;
        }

        index++;
    }

    return desktopfp;
}

//根据应用英文名获取desktop文件路径
QString UkuiMenuInterface::getDesktopPathByAppEnglishName(QString appname)
{
    int i=0;
    QString desktopfilepath;
    QStringList desktopfpList=getDesktopFilePath();
    for(i=0;i<desktopfpList.count();i++)
    {
        QString name=getAppEnglishName(desktopfpList.at(i));
        if(QString::compare(name,appname)==0)
        {
            desktopfilepath=desktopfpList.at(i);
        }
    }
    return desktopfilepath;
}

QVector<QStringList> UkuiMenuInterface::getAlphabeticClassification()
{
    QVector<QStringList> data;
    QStringList appnameList;
    appnameList.clear();
    QStringList list[27];
    int index=0;
    while(index<appInfoVector.size())
    {
        QString appname=appInfoVector.at(index).at(2);
        QString appnamepy=UkuiChineseLetter::getPinyins(appname);
        char c=appnamepy.at(0).toLatin1();
        switch (c) {
        case 'A':
            list[0].append(appname);
            break;
        case 'B':
            list[1].append(appname);
            break;
        case 'C':
            list[2].append(appname);
            break;
        case 'D':
            list[3].append(appname);
            break;
        case 'E':
            list[4].append(appname);
            break;
        case 'F':
            list[5].append(appname);
            break;
        case 'G':
            list[6].append(appname);
            break;
        case 'H':
            list[7].append(appname);
            break;
        case 'I':
            list[8].append(appname);
            break;
        case 'J':
            list[9].append(appname);
            break;
        case 'K':
            list[10].append(appname);
            break;
        case 'L':
            list[11].append(appname);
            break;
        case 'M':
            list[12].append(appname);
            break;
        case 'N':
            list[13].append(appname);
            break;
        case 'O':
            list[14].append(appname);
            break;
        case 'P':
            list[15].append(appname);
            break;
        case 'Q':
            list[16].append(appname);
            break;
        case 'R':
            list[17].append(appname);
            break;
        case 'S':
            list[18].append(appname);
            break;
        case 'T':
            list[19].append(appname);
            break;
        case 'U':
            list[20].append(appname);
            break;
        case 'V':
            list[21].append(appname);
            break;
        case 'W':
            list[22].append(appname);
            break;
        case 'X':
            list[23].append(appname);
            break;
        case 'Y':
            list[24].append(appname);
            break;
        case 'Z':
            list[25].append(appname);
            break;
        default:
            list[26].append(appname);
            break;
        }

        index++;
    }

    QLocale local;
    QString language=local.languageToString(local.language());
    if(QString::compare(language,"Chinese")==0)
        local=QLocale(QLocale::Chinese);
    else
        local=QLocale(QLocale::English);
    QCollator collator(local);

    for(int i=0;i<26;i++)
    {
        std::sort(list[i].begin(),list[i].end(),collator);
        data.append(list[i]);
    }

    QStringList otherList;
    QStringList numberList;
    for(int i=0;i<list[26].count();i++)
    {
        QChar c=list[26].at(i).at(0);
        if(c<48 || (c>57 && c<65) || c>90)
            otherList.append(list[26].at(i));
        else
            numberList.append(list[26].at(i));
    }
    std::sort(otherList.begin(),otherList.end(),collator);
    std::sort(numberList.begin(),numberList.end(),collator);
    data.append(otherList);
    data.append(numberList);

    return data;
}

QVector<QStringList> UkuiMenuInterface::getFunctionalClassification()
{
    QStringList list[11];
    int index=0;
    while(index<appInfoVector.size())
    {
        int count=appInfoVector.at(index).size()-5;
        for(int i=0;i<count;i++)
        {
            int category=appInfoVector.at(index).at(5+i).toInt();
            switch (category) {
            case 1:
                list[1].append(appInfoVector.at(index).at(2));
                break;
            case 2:
                list[2].append(appInfoVector.at(index).at(2));
                break;
            case 3:
                list[3].append(appInfoVector.at(index).at(2));
                break;
            case 4:
                list[4].append(appInfoVector.at(index).at(2));
                break;
            case 5:
                list[5].append(appInfoVector.at(index).at(2));
                break;
            case 6:
                list[6].append(appInfoVector.at(index).at(2));
                break;
            case 7:
                list[7].append(appInfoVector.at(index).at(2));
                break;
            case 8:
                list[8].append(appInfoVector.at(index).at(2));
                break;
            case 9:
                list[9].append(appInfoVector.at(index).at(2));
                break;
            case 10:
                list[10].append(appInfoVector.at(index).at(2));
                break;
            default:
                break;
            }
        }
        index++;
    }

    QVector<QStringList> data;
    data.clear();
    list[0].clear();
    list[0]=getRecentApp();
    data.append(list[0]);

    QLocale local;
    QString language=local.languageToString(local.language());
    if(QString::compare(language,"Chinese")==0)
        local=QLocale(QLocale::Chinese);
    else
        local=QLocale(QLocale::English);
    QCollator collator(local);
    for(int i=1;i<11;i++)
    {
        std::sort(list[i].begin(),list[i].end(),collator);
        data.append(list[i]);
    }

    return data;
}

bool UkuiMenuInterface::matchingAppCategories(QString desktopfp, QStringList categorylist)
{
    QString category=getAppCategories(desktopfp);
    int index;
    for(index=0;index<categorylist.count();index++)
    {
        if(category.contains(categorylist.at(index),Qt::CaseInsensitive))
            return true;
    }
    if(index==categorylist.count())
        return false;

    return false;
}

QStringList UkuiMenuInterface::getRecentApp()
{
    QStringList recentAppList;
    recentAppList.clear();
    QDateTime dt=QDateTime::currentDateTime();
    int currentDateTime=dt.toTime_t();
    int nDaySec=24*60*60;
    setting->beginGroup("recentapp");
    QStringList recentAppKeys=setting->allKeys();
    for(int i=0;i<recentAppKeys.count();i++)
    {
        if((currentDateTime-setting->value(recentAppKeys.at(i)).toInt())/nDaySec >= 3)
            setting->remove(recentAppKeys.at(i));
    }
    setting->sync();
    for(int i=0;i<setting->allKeys().size();i++)
    {
        QString desktopfp=QString("/usr/share/applications/"+setting->allKeys().at(i));
        QFileInfo fileInfo(desktopfp);
        if(!fileInfo.exists())
            continue;
        QString appname=getAppName(desktopfp);
        recentAppList.append(appname);
    }
    setting->endGroup();
    QLocale local;
    QString language=local.languageToString(local.language());
    if(QString::compare(language,"Chinese")==0)
        local=QLocale(QLocale::Chinese);
    else
        local=QLocale(QLocale::English);
    QCollator collator(local);
    std::sort(recentAppList.begin(),recentAppList.end(),collator);
    return recentAppList;
}

QVector<QString> UkuiMenuInterface::getCommonUseApp()
{
    QDateTime dt=QDateTime::currentDateTime();
    int currentDateTime=dt.toTime_t();
    int nDaySec=24*60*60;
    setting->beginGroup("datetime");
    QStringList dateTimeKeys=setting->allKeys();
    QStringList timeOutKeys;
    timeOutKeys.clear();
    for(int i=0;i<dateTimeKeys.count();i++)
    {
        if((currentDateTime-setting->value(dateTimeKeys.at(i)).toInt())/nDaySec >= 4)
        {
            timeOutKeys.append(dateTimeKeys.at(i));
        }

    }
    setting->endGroup();
    for(int i=0;i<timeOutKeys.count();i++)
    {
        setting->beginGroup("application");
        setting->remove(timeOutKeys.at(i));
        setting->sync();
        setting->endGroup();
        setting->beginGroup("datetime");
        setting->remove(timeOutKeys.at(i));
        setting->sync();
        setting->endGroup();
    }

    setting->beginGroup("lockapplication");
    QStringList lockdesktopfnList=setting->allKeys();
    for(int i=0;i<lockdesktopfnList.count()-1;i++)
        for(int j=0;j<lockdesktopfnList.count()-1-i;j++)
        {
            int value_1=setting->value(lockdesktopfnList.at(j)).toInt();
            int value_2=setting->value(lockdesktopfnList.at(j+1)).toInt();
            if(value_1 > value_2)
            {
                QString tmp=lockdesktopfnList.at(j);
                lockdesktopfnList.replace(j,lockdesktopfnList.at(j+1));
                lockdesktopfnList.replace(j+1,tmp);

            }
        }
    setting->endGroup();
    setting->beginGroup("application");
    QStringList desktopfnList=setting->allKeys();
    for(int i=0;i<desktopfnList.count()-1;i++)
        for(int j=0;j<desktopfnList.count()-1-i;j++)
        {
            int value_1=setting->value(desktopfnList.at(j)).toInt();
            int value_2=setting->value(desktopfnList.at(j+1)).toInt();
            if(value_1 < value_2)
            {
                QString tmp=desktopfnList.at(j);
                desktopfnList.replace(j,desktopfnList.at(j+1));
                desktopfnList.replace(j+1,tmp);

            }
        }
    setting->endGroup();

    QVector<QString> data;
    Q_FOREACH(QString desktopfn,lockdesktopfnList)
    {
        QString desktopfp=QString("/usr/share/applications/"+desktopfn);
        QFileInfo fileInfo(desktopfp);
        if(!fileInfo.exists())
            continue;
        data.append(desktopfp);
    }
    Q_FOREACH(QString desktopfn,desktopfnList)
    {
        QString desktopfp=QString("/usr/share/applications/"+desktopfn);
        QFileInfo fileInfo(desktopfp);
        if(!fileInfo.exists())
            continue;
        data.append(desktopfp);
    }

    return data;
}

//获取应用拼音
QString UkuiMenuInterface::getAppNamePinyin(QString appname)
{
    return UkuiChineseLetter::getPinyins(appname);
}

//获取指定类型应用列表
QStringList UkuiMenuInterface::getSpecifiedCategoryAppList(QString categorystr)
{
    QByteArray categorybyte=categorystr.toLocal8Bit();
    char* category=categorybyte.data();

    QStringList desktopfpList=getDesktopFilePath();
    QStringList appnameList;
    appnameList.clear();
    for(int index=0;index<desktopfpList.count();index++)
    {
        QString appcategorystr=getAppCategories(desktopfpList.at(index));
        if(!appcategorystr.isEmpty())
        {
            QByteArray appcategorybyte=appcategorystr.toLocal8Bit();
            char* appcategory=appcategorybyte.data();
            int len=static_cast<int>(strlen(appcategory)-6);
            for(int i=0;i<len;i++)
            {
                if(strncmp(appcategory+i,category,strlen(category))==0)
                {
                    QString appname=getAppName(desktopfpList.at(index));
                    if(QString::compare(appname,"访问提示")==0)
//                        qDebug()<<desktopfpList.at(index);
                    appnameList.append(appname);
                    break;
                }
            }

        }
    }
    return appnameList;

}

//获取用户图像
QString UkuiMenuInterface::getUserIcon()
{
    qint64 uid=static_cast<qint64>(getuid());
    QDBusInterface iface("org.freedesktop.Accounts",
                         "/org/freedesktop/Accounts",
                         "org.freedesktop.Accounts",
                         QDBusConnection::systemBus());
    QDBusReply<QDBusObjectPath>objPath=iface.call("FindUserById",uid);

    QDBusInterface useriface("org.freedesktop.Accounts",
                             objPath.value().path(),
                             "org.freedesktop.DBus.Properties",
                             QDBusConnection::systemBus());
    QDBusReply<QVariant> var=useriface.call("Get","org.freedesktop.Accounts.User","IconFile");
    QString iconstr=var.value().toString();
    return iconstr;
}

QString UkuiMenuInterface::getUserName()
{
    qint64 uid=static_cast<qint64>(getuid());
//    QString name=QDir::homePath();
    QDBusInterface iface("org.freedesktop.Accounts",
                         "/org/freedesktop/Accounts",
                         "org.freedesktop.Accounts",
                         QDBusConnection::systemBus());
    QDBusReply<QDBusObjectPath> objPath=iface.call("FindUserById",uid);
    QDBusInterface useriface("org.freedesktop.Accounts",
                             objPath.value().path(),
                             "org.freedesktop.DBus.Properties",
                             QDBusConnection::systemBus());
    QDBusReply<QVariant> var=useriface.call("Get","org.freedesktop.Accounts.User","UserName");
    QString name=var.value().toString();
    return name;
}
