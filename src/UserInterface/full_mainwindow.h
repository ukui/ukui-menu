#ifndef FULLMAINWINDOW_H
#define FULLMAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QPainterPath>
#include "full_commonuse_widget.h"
#include "full_function_widget.h"
#include "full_letter_widget.h"
#include "searchappthread.h"
#include "full_searchresult_widget.h"

class FullMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FullMainWindow(QWidget *parent = nullptr);
    ~FullMainWindow();
public:
    void updateView();
    void resetEditline();
    void repaintWidget();

Q_SIGNALS:

    void showNormalWindow();
    void sendSearchKeyword(QString arg);
    void sendSetFocusToCom();
    void sendSetFocusToFun();
    void sendSetFocusToLet();
    void sendSetFocusToResult();
    void sendUpdateOtherView();

private Q_SLOTS:

    void on_minPushButton_clicked();
    void on_fullSelectToolButton_clicked();
    void on_fullSelectMenuButton_triggered(QAction *arg1);
    void searchAppSlot(QString arg);
    void recvSearchResult(QVector<QStringList> arg);
    void setFocusToButton();
    void on_fullSelectMenuButton_clicked();


protected:
    void paintEvent(QPaintEvent *event);
    bool event(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QAction *m_allAction = nullptr;
    QAction *m_letterAction = nullptr;
    QAction *m_funcAction = nullptr;
    QMenu *m_menu = nullptr;

    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *topHorizontalLayout;
    QHBoxLayout *bottomHorizonLayout;
    QSpacerItem *horizontalSpacer;
    QLineEdit *m_lineEdit;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *fullSelectToolButton;
    QToolButton *fullSelectMenuButton;
    QPushButton *minPushButton;
    QStackedWidget *m_fullStackedWidget;

    FullCommonUseWidget *m_fullCommonPage;
    FullLetterWidget *m_fullLetterPage;
    FullFunctionWidget *m_fullFunctionPage;
    FullSearchResultWidget *m_fullResultPage;

    SearchAppThread *m_searchAppThread = nullptr;
    int m_state = 0;
    QWidget *m_queryWid = nullptr;
    QLabel *m_queryIcon = nullptr;
    QLabel *m_queryText = nullptr;
    bool isSearching = false;
    QString m_buttonStyle;
};

#endif // FULLMAINWINDOW_H
