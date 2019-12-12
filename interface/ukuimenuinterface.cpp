#include "ukuimenuinterface.h"
#include <glib.h>
#include <QDir>
#include <QDebug>
#include <QCollator>
#include <QLocale>
#include <QStringList>
#include "chineseletterhelper.h"

UkuiMenuInterface::UkuiMenuInterface()
{
}

UkuiMenuInterface::~UkuiMenuInterface()
{
//    delete[] pAppInfo;
}

//文件递归查询
void UkuiMenuInterface::recursive_search_file(const QString& _filePath)
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
            recursive_search_file(fileInfo.filePath());
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
QStringList UkuiMenuInterface::get_desktop_file_path()
{
    filePathList.clear();
    recursive_search_file("/usr/share/applications/");

    filePathList.removeAll("/usr/share/applications/peony-folder-handler.desktop");
    filePathList.removeAll("/usr/share/applications/peony.desktop");
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
    filePathList.removeAll("/usr/share/applications/peony-home.desktop");
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

    return filePathList;
}

QVector<QStringList> UkuiMenuInterface::appInfoVector=QVector<QStringList>();

//创建应用信息容器
QVector<QStringList> UkuiMenuInterface::create_appinfo_vector()
{
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

    QStringList desktopfpList=get_desktop_file_path();

    for(int i=0;i<desktopfpList.count();i++)
    {
        QStringList appInfoList;
        QString desktopfp=desktopfpList.at(i);
        QString icon=get_app_icon(desktopfpList.at(i));
        QString name=get_app_name(desktopfpList.at(i));
        QString exec=get_app_exec(desktopfpList.at(i));
        QString comment=get_app_comment(desktopfpList.at(i));

        appInfoList<<desktopfp<<icon<<name<<exec<<comment;
        bool is_owned=false;
        for(int j=0;j<vector.size();j++)
        {
            if(matching_app_categories(desktopfpList.at(i),vector.at(j)))//有对应分类
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
QString UkuiMenuInterface::get_app_name(QString desktopfp)
{
    GError** error=nullptr;
    GKeyFileFlags flags=G_KEY_FILE_NONE;
    GKeyFile* keyfile=g_key_file_new ();

    QByteArray fpbyte=desktopfp.toLocal8Bit();
    char* filepath=fpbyte.data();
    g_key_file_load_from_file(keyfile,filepath,flags,error);

    QString namestr;
    QLocale cn;
    QString language=cn.languageToString(cn.language());
    if(QString::compare(language,"Chinese")==0)
    {
        char* name;
        name=g_key_file_get_locale_string(keyfile,"Desktop Entry","Name[zh_CN]", nullptr, nullptr);
        if(QString::fromLocal8Bit(name).isEmpty())
            name=g_key_file_get_locale_string(keyfile,"Desktop Entry","Name", nullptr, nullptr);
        namestr=QString::fromLocal8Bit(name);
    }
    else {
        char* name=g_key_file_get_locale_string(keyfile,"Desktop Entry","Name", nullptr, nullptr);
        namestr=QString::fromLocal8Bit(name);
    }

    g_key_file_free(keyfile);
    return namestr;
}

//获取英应用英文名
QString UkuiMenuInterface::get_app_english_name(QString desktopfp)
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
QString UkuiMenuInterface::get_app_categories(QString desktopfp)
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
QString UkuiMenuInterface::get_app_icon(QString desktopfp)
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
QString UkuiMenuInterface::get_app_exec(QString desktopfp)
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
QString UkuiMenuInterface::get_app_comment(QString desktopfp)
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
QString UkuiMenuInterface::get_app_type(QString desktopfp)
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
QString UkuiMenuInterface::get_desktop_path_by_app_name(QString appname)
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
QString UkuiMenuInterface::get_desktop_path_by_app_english_name(QString appname)
{
    int i=0;
    QString desktopfilepath;
    QStringList desktopfpList=get_desktop_file_path();
    for(i=0;i<desktopfpList.count();i++)
    {
        QString name=get_app_english_name(desktopfpList.at(i));
        if(QString::compare(name,appname)==0)
        {
            desktopfilepath=desktopfpList.at(i);
        }
    }
    return desktopfilepath;
}

QVector<QStringList> UkuiMenuInterface::get_alphabetic_classification()
{
    QVector<QStringList> data;
    QStringList appnameList;
    appnameList.clear();
    QStringList list[27];
    int index=0;
    while(index<appInfoVector.size())
    {
        QString appname=appInfoVector.at(index).at(2);
        QString appnamepy=ChineseLetterHelper::GetPinyins(appname);
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

QVector<QStringList> UkuiMenuInterface::get_functional_classification()
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
                list[3].append(appInfoVector.at(index).at(2));
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

    QStringList recentList=get_recent_app_list();//最近添加
    for(int i=0;i<10;i++)
        list[0].append(recentList.at(i));

    QVector<QStringList> data;

    QLocale local;
    QString language=local.languageToString(local.language());
    if(QString::compare(language,"Chinese")==0)
        local=QLocale(QLocale::Chinese);
    else
        local=QLocale(QLocale::English);
    QCollator collator(local);
    for(int i=0;i<11;i++)
    {
        std::sort(list[i].begin(),list[i].end(),collator);
        data.append(list[i]);
    }

    return data;

}

bool UkuiMenuInterface::matching_app_categories(QString desktopfp, QStringList categorylist)
{
    QString category=get_app_categories(desktopfp);
    int index;
    for(index=0;index<categorylist.count();index++)
    {
        if(category.contains(categorylist.at(index),Qt::CaseInsensitive))
            return true;
    }
    if(index==categorylist.count())
        return false;
}

//应用排序
QStringList UkuiMenuInterface::sort_app_name()
{
    QStringList desktopfpList=get_desktop_file_path();
    QStringList appnameList;
    appnameList.clear();
    for(int i=0;i<desktopfpList.count();i++)
    {
        QString appname=get_app_name(desktopfpList.at(i));
        appnameList.append(appname);
    }

    //从appnameList中将应用名首字符为非字母的移除
    QStringList otherappname;//不是26个字母开头
    QStringList numberappname;//以数字开头
    otherappname.clear();
    numberappname.clear();
    for(int i=0;i<appnameList.count();i++)
    {
        QString appname=ChineseLetterHelper::GetPinyins(appnameList.at(i));
        QChar c=appname.at(0);
        if( c<48 || (c>57 && c<65) || c>90 )
        {
            otherappname.append(appnameList.at(i));
            appnameList.removeAt(i);
            i--;

        }

        if(c>=48 && c<=57)
        {
            numberappname.append(appnameList.at(i));
            appnameList.removeAt(i);
            i--;
        }


    }


    //判断语言环境
    QLocale cn;
    QString language=cn.languageToString(cn.language());
    if(QString::compare(language,"Chinese")==0)
    {
//        //中文环境
//        //执行sort后，appnameList前半部分为已按字母排序好的中文，后半部分为英文
//        QLocale cn(QLocale::Chinese);
//        QCollator collator(cn);
//        std::sort(appnameList.begin(), appnameList.end(), collator);

//        //找到appnameList中第一个首字母为英文的应用名，记住位置为index
//        int index=0;
//        for(int i=0;i<appnameList.count();i++)
//        {
//            QString appnamestr=appnameList.at(i);
//            QChar c=appnamestr.at(0);
//            if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') )
//            {
//                index=i;
//                break;
//            }
//        }

//        //将应用名首字母为非英文的插入到已排序好的中文序列中
//        while(index<appnameList.count())
//        {
//            int i=0;
//            for(i=0;i<index;i++)
//            {
//                QString indexstr=appnameList.at(index);
//                QString letter=ChineseLetterHelper::GetPinyins(appnameList.at(i));
//                QString indexletter=ChineseLetterHelper::GetPinyins(indexstr);
//                if(QString::compare(letter,indexletter)>0)
//                {
//                    appnameList.removeAt(index);
//                    appnameList.insert(i,indexstr);
//                    index++;
//                    break;
//                }
//            }
//            if(i==index)index++;

//        }
//选择排序法
//        int index=0;
//        while(index<appnameList.count())
//        {
//            int min=index;
//            QString indexstr=ChineseLetterHelper::GetPinyins(appnameList.at(index));
//            int i;
//            for(i=index;i<appnameList.count();i++)
//            {
//                QString appname=ChineseLetterHelper::GetPinyins(appnameList.at(i));
//                if(QString ::compare(appname,indexstr)<0)
//                {
//                    indexstr=appname;
//                    min=i;
//                }
//            }


//            QString tmp=appnameList.at(min);
//            appnameList.removeAt(min);
//            appnameList.insert(index,tmp);
//            index++;
//        }

//起泡排序法
        for(int i=0;i<appnameList.count()-1;i++)
            for(int j=0;j<appnameList.count()-1-i;j++)
            {
                QString appname_1=ChineseLetterHelper::GetPinyins(appnameList.at(j));
                QString appname_2=ChineseLetterHelper::GetPinyins(appnameList.at(j+1));
                if(QString::compare(appname_1,appname_2)>0)
                {
                    QString tmp=appnameList.at(j);
                    appnameList.replace(j,appnameList.at(j+1));
                    appnameList.replace(j+1,tmp);

                }
            }

    }
    else{
        //英文环境
        QLocale cn(QLocale::English);
        QCollator collator(cn);
        std::sort(appnameList.begin(), appnameList.end(), collator);
    }

    for(int i=0;i<otherappname.count();i++)
    {
        appnameList.append(otherappname.at(i));
    }

    for(int i=0;i<numberappname.count();i++)
    {
        appnameList.append(numberappname.at(i));
    }

//    qDebug()<<appnameList.count();
    return appnameList;
}

QStringList UkuiMenuInterface::get_app_diff_first_letter_pos()
{
    QStringList letterposlist;
    letterposlist.clear();
    QStringList appsortlist=sort_app_name();
//    for(int i=0;i<appsortlist.count();i++)
//    {
//        QString apppy=get_app_name_pinyin(appsortlist.at(i));
//        QChar ch=apppy.at(0);
//        if(letter=='&')
//        {
//            if(ch<48 || (ch>57 && ch<65) || ch>90)//非数字非英文字符
//                applist.append(appsortlist.at(i));

//        }
//        else if(letter=='#')
//        {
//            if(ch>=48 && ch<=57)//数字字符
//                applist.append(appsortlist.at(i));
//        }
//        else{
//            if(ch==letter)
//                applist.append(appsortlist.at(i));
//        }

//    }

    QString letterstr=appsortlist.at(0);
    QChar letter=letterstr.at(0);
    letterposlist.append(QString::number(0));
    for(int i=0;i<appsortlist.count();i++)
    {
        QString apppy=get_app_name_pinyin(appsortlist.at(i));
        QChar ch=apppy.at(0);
        if(ch<48 || (ch>57 && ch<65) || ch>90)//非数字非英文字符
        {
            if(letter!='&')
            {
                letter='&';
                letterposlist.append(QString::number(i));
            }
            else continue;

        }
        else if(ch>=48 && ch<=57)//数字字符
        {
            letter='#';
            letterposlist.append(QString::number(i));
            break;
        }
        else{
            if(ch!=letter)
            {
                letter=ch;
                letterposlist.append(QString::number(i));
            }
            else continue;
        }
    }
    return letterposlist;
}

//获取应用拼音
QString UkuiMenuInterface::get_app_name_pinyin(QString appname)
{
    return ChineseLetterHelper::GetPinyins(appname);
}

//获取指定类型应用列表
QStringList UkuiMenuInterface::get_specified_category_app_list(QString categorystr)
{
    QByteArray categorybyte=categorystr.toLocal8Bit();
    char* category=categorybyte.data();

    QStringList desktopfpList=get_desktop_file_path();
    QStringList appnameList;
    appnameList.clear();
    for(int index=0;index<desktopfpList.count();index++)
    {
        QString appcategorystr=get_app_categories(desktopfpList.at(index));
        if(!appcategorystr.isEmpty())
        {
            QByteArray appcategorybyte=appcategorystr.toLocal8Bit();
            char* appcategory=appcategorybyte.data();
            int len=static_cast<int>(strlen(appcategory)-6);
            for(int i=0;i<len;i++)
            {
                if(strncmp(appcategory+i,category,strlen(category))==0)
                {
                    QString appname=get_app_name(desktopfpList.at(index));
                    if(QString::compare(appname,"访问提示")==0)
                        qDebug()<<desktopfpList.at(index);
                    appnameList.append(appname);
                    break;
                }
            }

        }
    }
    return appnameList;

}

QStringList UkuiMenuInterface::get_recent_app_list()
{
    QStringList appnamelist;
    appnamelist.clear();
    system("ls -lt /usr/share/applications | awk '{print $9'} > /tmp/ukui-start-menu.txt");
    FILE* fp;
    QString pathstr="/tmp/ukui-start-menu.txt";
    QByteArray pathbyte=pathstr.toLocal8Bit();
    char* path=pathbyte.data();
    fp=fopen(path,"r");
    if(fp!=nullptr)
    {
        while(!feof(fp))
        {
            char strline[1024];
            fgets(strline,1024,fp);
            char* desktopname=strline;
            int len=static_cast<int>(strlen(desktopname));
            if(len>8)
            {
                for(int i=0;i<len;i++)
                {
                    if(strncmp((desktopname+i),".desktop",8)==0)
                    {
                        for(int j=0;j<len;j++)
                        {
                            if(*(desktopname+j)=='\n')
                            {
                                *(desktopname+j)='\0';
                                break;
                            }
                        }
                        QString desktopstr="/usr/share/applications/"+QString::fromLocal8Bit(desktopname);
                        if(QString::compare(desktopstr,"/usr/share/applications/peony-folder-handler.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/peony.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/gnome-software-local-file.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/org.gnome.Software.Editor.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/apport-gtk.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/software-properties-livepatch.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/snap-handle-link.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/python3.7.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/rhythmbox-device.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/smplayer_enqueue.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/python2.7.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/mate-color-select.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/shotwell-viewer.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/burner-nautilus.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/gnome-system-monitor-kde.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/hplj1020.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/ukui-network-scheme.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/ukui-panel.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/unity-activity-log-manager-panel.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/blueman-adapters.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/fcitx-config-gtk3.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/im-config.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/fcitx-skin-installer.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/gcr-prompter.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/gcr-viewer.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/geoclue-demo-agent.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/gnome-disk-image-mounter.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/gnome-disk-image-writer.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/libreoffice-xsltfilter.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/peony-autorun-software.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/remmina-file.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/remmina-gnome.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/ukwm.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/nm-applet.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/peony-home.desktop")==0)
                            break;
                        if(QString::compare(desktopstr,"/usr/share/applications/mate-user-guide.desktop")==0)
                            break;
                        QString appname=get_app_name(desktopstr);
                        appnamelist.append(appname);
//                        appnamelist.append(desktopstr);
                        break;
                    }
                }
            }
            else{
                continue;
            }

        }
    }

    return appnamelist;

}

//获取网络应用列表
QStringList UkuiMenuInterface::get_network_app_list()
{
    QStringList networklist;
    networklist.clear();
//    networklist=get_specified_category_app_list("Network");
    networklist=get_specified_category_app_list("Network");
//    qDebug()<<networklist.count();
    return networklist;
}

//获取社交沟通应用列表
QStringList UkuiMenuInterface::get_social_app_list()
{
    QStringList sociallist;
    sociallist.clear();
    sociallist=get_specified_category_app_list("Messaging");
//    qDebug()<<sociallist.count();
    return sociallist;

}

//获取影音播放应用列表
QStringList UkuiMenuInterface::get_av_app_list()
{
    QStringList avlist;
    QStringList avlist_1;
    avlist.clear();
    avlist_1.clear();
    avlist=get_specified_category_app_list("Audio");
    avlist_1=get_specified_category_app_list("Video");
    int j;
    for(int i=0;i<avlist_1.count();i++)
    {
        for(j=0;j<avlist.count();j++)
        {
            QString avstr_1=avlist_1.at(i);
            QString avstr=avlist.at(j);
            if(QString::compare(avstr,avstr_1)==0)
                break;
        }
        if(j==avlist.count())
            avlist.append(avlist_1.at(i));
    }
    return avlist;
}

//获取开发编程应用列表
QStringList UkuiMenuInterface::get_develop_app_list()
{
    QStringList videolist;
    videolist.clear();
    videolist=get_specified_category_app_list("Development");
//    qDebug()<<videolist.count();
    return videolist;
}

//获取图形图像应用列表
QStringList UkuiMenuInterface::get_graphics_app_list()
{
    QStringList graphiclist;
    graphiclist.clear();
    graphiclist=get_specified_category_app_list("Graphics");
//    qDebug()<<graphiclist.count();
    return graphiclist;
}

//获取游戏娱乐应用列表
QStringList UkuiMenuInterface::get_game_app_list()
{
    QStringList gamelist;
    gamelist.clear();
    gamelist=get_specified_category_app_list("Game");
//    qDebug()<<gamelist.count();
    return gamelist;
}

//获取办公学习应用列表
QStringList UkuiMenuInterface::get_office_app_list()
{
    QStringList office=get_specified_category_app_list("Office");
    QStringList calculator=get_specified_category_app_list("Calculator");
    QStringList wpssheet=get_specified_category_app_list("Spreadsheet");
    QStringList wpsresentation=get_specified_category_app_list("Presentation");
    QStringList wpsword=get_specified_category_app_list("WordProcessor");

    int j;
    for(int i=0;i<calculator.count();i++)
    {
        for(j=0;j<office.count();j++)
        {
            if(QString::compare(office.at(j),calculator.at(i))==0)
                break;
        }
        if(j==office.count())
            office.append(calculator.at(i));
    }

    for(int i=0;i<wpssheet.count();i++)
    {
        for(j=0;j<office.count();j++)
        {
            if(QString::compare(office.at(j),wpssheet.at(i))==0)
                break;
        }
        if(j==office.count())
            office.append(wpssheet.at(i));
    }
    for(int i=0;i<wpsresentation.count();i++)
    {
        for(j=0;j<office.count();j++)
        {
            if(QString::compare(office.at(j),wpsresentation.at(i))==0)
                break;
        }
        if(j==office.count())
            office.append(wpsresentation.at(i));
    }
    for(int i=0;i<wpsword.count();i++)
    {
        for(j=0;j<office.count();j++)
        {
            if(QString::compare(office.at(j),wpsword.at(i))==0)
                break;
        }
        if(j==office.count())
            office.append(wpsword.at(i));
    }
//    qDebug()<<officelist.count();
    return office;
}

//阅读翻译
QStringList UkuiMenuInterface::get_education_app_list()
{
    QStringList educationlist;
    educationlist.clear();
    educationlist=get_specified_category_app_list("Education");
    return educationlist;
}

//获取系统管理应用列表
QStringList UkuiMenuInterface::get_systemadmin_app_list()
{
    QStringList systemlist=get_specified_category_app_list("System");
    QStringList settingslist=get_specified_category_app_list("Settings");
    QStringList securitylist=get_specified_category_app_list("Security");

    remove_repetition_appname(settingslist,systemlist);
    remove_repetition_appname(securitylist,systemlist);

    int j=0;
    for(int i=0;i<settingslist.count();i++)
    {
        for(j=0;j<systemlist.count();j++)
        {
            if(QString::compare(settingslist.at(i),systemlist.at(j))==0)
                break;
        }
        if(j==systemlist.count())
            systemlist.append(settingslist.at(i));


    }

    for(int i=0;i<securitylist.count();i++)
    {
        for(j=0;j<systemlist.count();j++)
        {
            if(QString::compare(securitylist.at(i),systemlist.at(j))==0)
                break;
        }
        if(j==systemlist.count())
            systemlist.append(securitylist.at(i));

    }

//    qDebug()<<systemlist.count();
    return systemlist;
}

//获取其他应用列表
QStringList UkuiMenuInterface::get_other_app_list()
{
    QStringList otherlist;
    otherlist.clear();

    QStringList desktopfpList=get_desktop_file_path();

    for(int i=0;i<desktopfpList.count();i++)
    {
        QString appname=get_app_name(desktopfpList.at(i));
        otherlist.append(appname);
    }
    QStringList networkList=get_network_app_list();
    QStringList socialList=get_social_app_list();
    QStringList avList=get_av_app_list();
    QStringList developList=get_develop_app_list();
    QStringList graphList=get_graphics_app_list();
    QStringList gameList=get_game_app_list();
    QStringList officeList=get_office_app_list();
    QStringList systemadminList=get_systemadmin_app_list();

    remove_repetition_appname(otherlist,networkList);
    remove_repetition_appname(otherlist,socialList);
    remove_repetition_appname(otherlist,avList);
    remove_repetition_appname(otherlist,developList);
    remove_repetition_appname(otherlist,graphList);
    remove_repetition_appname(otherlist,gameList);
    remove_repetition_appname(otherlist,officeList);
    remove_repetition_appname(otherlist,systemadminList);

//    qDebug()<<otherlist.count();

    return otherlist;

}

//移除重复项
void UkuiMenuInterface::remove_repetition_appname(QStringList &list, QStringList complist)
{
    for(int i=0;i<complist.count();i++)
        for(int j=0;j<list.count();j++)
        {
            if(QString::compare(complist.at(i),list.at(j))==0)
            {
                list.removeAt(j);
                j--;
            }
        }
}

//获取用户图像
QString UkuiMenuInterface::get_user_icon()
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

QString UkuiMenuInterface::get_user_name()
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
    QDBusReply<QVariant> var=useriface.call("Get","org.freedesktop.Accounts.User","RealName");
    QString name=var.value().toString();
    return name;
}
