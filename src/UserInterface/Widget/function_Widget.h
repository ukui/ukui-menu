#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLocale>
#include <QDateTime>
#include <QGSettings>
#include <QSettings>
#include "AbstractInterface.h"
#include <QGraphicsDropShadowEffect>
#include "plugin_widget.h"
#include "currenttime_interface.h"

class FunctionWidget : public QWidget
{
    Q_OBJECT
public:
    FunctionWidget(QWidget *parent);
    ~FunctionWidget();
    //QWidget* focusPlug=nullptr;
    pluginwidget *focusPlug = nullptr;
    void setDownOpacityEffect(const qreal &num);

protected:
    void initUi();

private:
    CurrentTimeInterface *Time = nullptr;
    QGSettings *themeSetting = nullptr;
    QGSettings *timeSetting = nullptr;
    QString themeName;
    QObject *plugin = nullptr;
    AbstractInterface *app;
    QGraphicsDropShadowEffect *effect;
    QPalette palette;
    QTimer *myTimer = nullptr;

    //上方(时间+搜索)
    QWidget *upWidget = nullptr;
    QWidget *leftUpWidget = nullptr;
    QWidget *upLeftWidget = nullptr;
    QWidget *downWidget = nullptr;


    QVBoxLayout *upLayout;
    QHBoxLayout *leftUpLayout;
    QVBoxLayout *downLayout;
    QVBoxLayout *focusModeLayout;

    QHBoxLayout *upLeftLayout;
    QVBoxLayout *upRightLayout;

    QLabel *timeLabel;
    QLabel *weekLabel;
    QLabel *dateLabel;
    QPushButton *searchEditBtn;

    QDBusInterface *usrInterface = nullptr;

    //右侧
    QWidget *upRightWidget = nullptr;

    qreal opacity; //透明系数

private Q_SLOTS:

    void obtainSearchResult();

public Q_SLOTS:

    void changeSearchBoxBackground(QString styleName);
public:
Q_SIGNALS:
    void hideTabletWindow();



};

#endif // TIMEWIDGET_H