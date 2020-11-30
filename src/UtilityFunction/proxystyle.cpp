#include "proxystyle.h"
#include <QApplication>
#include <QDebug>

ProxyStyle::ProxyStyle()
{
    if(QGSettings::isSchemaInstalled(QString("org.ukui.style").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.style").toLocal8Bit());
        connect(gsetting,&QGSettings::changed,this,[=]{
            for(auto widget: QApplication::allWidgets())
            {
                widget->update();
            }
        });
    }
}

QPalette ProxyStyle::standardPalette() const
{
    auto m_palette=QProxyStyle::standardPalette();
    QColor m_highlightbg;
    if(QGSettings::isSchemaInstalled(QString("org.ukui.style").toLocal8Bit()))
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.style").toLocal8Bit());
        if(gsetting->keys().contains(QString("styleName")))
        {
            if(gsetting->get("style-name").toString()=="ukui-light")
            {
                m_highlightbg=QColor(0, 0, 0);
                m_highlightbg.setAlphaF(0.15);
            }
            else
            {
                m_highlightbg=QColor(255,255,255);
                m_highlightbg.setAlphaF(0.14);
            }
        }
        m_palette.setBrush(QPalette::Highlight,m_highlightbg);
        return m_palette;
    }
    return m_palette;
}

void ProxyStyle::polish(QPalette &pal)
{
    pal=standardPalette();
//    QProxyStyle::polish(pal);
}
