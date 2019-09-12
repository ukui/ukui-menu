#include "kylin-start-menu-interface.h"
#include <glib.h>
#include <QDir>
#include <QDebug>
#include <QCollator>
#include <QLocale>
#include <QStringList>
#include "chineseletterhelper.h"

QStringList KylinStartMenuInterface::filePathList;

KylinStartMenuInterface::KylinStartMenuInterface()
{

}

//文件递归查询
void KylinStartMenuInterface::recursive_search_file(const QString& _filePath)
{
    QDir dir(_filePath);
    if (!dir.exists()) {
        return;
    }

    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
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
            QString filePath=fileInfo.filePath();
            filePathList.append(filePath);
        }
        i++;

    } while(i < list.size());

}

//获取系统deskyop文件路径
QStringList KylinStartMenuInterface::get_desktop_file_path()
{
    filePathList.clear();
    recursive_search_file("/usr/share/applications/");

    for(int i=0;i<filePathList.count();i++)
    {
        QString category=get_app_categories(filePathList.at(i));
        if(QString::compare(category,"Screensaver;")==0)
        {
            filePathList.removeAt(i);
            i--;
            continue;
        }

        QString appname=get_app_name(filePathList.at(i));
        if(appname.isEmpty())
        {
            filePathList.removeAt(i);
            i--;
            continue;
        }
    }

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

    return filePathList;
}

//获取应用名称
QString KylinStartMenuInterface::get_app_name(QString desktopfp)
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

//获取应用分类
QString KylinStartMenuInterface::get_app_categories(QString desktopfp)
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
QString KylinStartMenuInterface::get_app_icon(QString desktopfp)
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
QString KylinStartMenuInterface::get_app_exec(QString desktopfp)
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
QString KylinStartMenuInterface::get_app_comment(QString desktopfp)
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
QString KylinStartMenuInterface::get_app_type(QString desktopfp)
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
QString KylinStartMenuInterface::get_desktop_path_by_app_name(QString appname)
{
    int i=0;
    QString desktopfilepath;
    QStringList desktopfpList=get_desktop_file_path();
    for(i=0;i<desktopfpList.count();i++)
    {
        QString name=get_app_name(desktopfpList.at(i));
        if(QString::compare(name,appname)==0)
        {
            desktopfilepath=desktopfpList.at(i);
        }
    }
    return desktopfilepath;
}

//应用排序
QStringList KylinStartMenuInterface::sort_app_name()
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
    QStringList otherappname;
    otherappname.clear();
    for(int i=0;i<appnameList.count();i++)
    {
        QString appname=ChineseLetterHelper::GetPinyins(appnameList.at(i));
        QChar c=appname.at(0);
        if( c<65 || c>90 )
        {
            otherappname.append(appnameList.at(i));
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

//    qDebug()<<appnameList.count();
    return appnameList;
}

//获取应用拼音
QString KylinStartMenuInterface::get_app_name_pinyin(QString appname)
{
    return ChineseLetterHelper::GetPinyins(appname);
}

//获取指定类型应用列表
QStringList KylinStartMenuInterface::get_specified_category_app_list(QString categorystr)
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

QStringList KylinStartMenuInterface::get_recent_app_list()
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
QStringList KylinStartMenuInterface::get_network_app_list()
{
    QStringList networklist;
    networklist.clear();
//    networklist=get_specified_category_app_list("Network");
    networklist=get_specified_category_app_list("Network");
//    qDebug()<<networklist.count();
    return networklist;
}

//获取社交沟通应用列表
QStringList KylinStartMenuInterface::get_social_app_list()
{
    QStringList sociallist;
    sociallist.clear();
    sociallist=get_specified_category_app_list("Messaging");
//    qDebug()<<sociallist.count();
    return sociallist;

}

//获取影音播放应用列表
QStringList KylinStartMenuInterface::get_av_app_list()
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
QStringList KylinStartMenuInterface::get_develop_app_list()
{
    QStringList videolist;
    videolist.clear();
    videolist=get_specified_category_app_list("Development");
//    qDebug()<<videolist.count();
    return videolist;
}

//获取图形图像应用列表
QStringList KylinStartMenuInterface::get_graphics_app_list()
{
    QStringList graphiclist;
    graphiclist.clear();
    graphiclist=get_specified_category_app_list("Graphics");
//    qDebug()<<graphiclist.count();
    return graphiclist;
}

//获取游戏娱乐应用列表
QStringList KylinStartMenuInterface::get_game_app_list()
{
    QStringList gamelist;
    gamelist.clear();
    gamelist=get_specified_category_app_list("Game");
//    qDebug()<<gamelist.count();
    return gamelist;
}

//获取办公学习应用列表
QStringList KylinStartMenuInterface::get_office_app_list()
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

//获取系统管理应用列表
QStringList KylinStartMenuInterface::get_systemadmin_app_list()
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
QStringList KylinStartMenuInterface::get_other_app_list()
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
void KylinStartMenuInterface::remove_repetition_appname(QStringList &list, QStringList complist)
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
