#ifndef FULLMAINWINDOW_H
#define FULLMAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include "src/CommonUseWidget/fullcommonusewidget.h"
#include "src/FunctionWidget/fullfunctionwidget.h"
#include "src/LetterWidget/fullletterwidget.h"

class FullMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FullMainWindow(QWidget *parent = nullptr);
    ~FullMainWindow();
public:
Q_SIGNALS:
    void showNormalWindow();
private Q_SLOTS:
    void on_minPushButton_clicked();

    void on_fullSelectToolButton_clicked();

    void on_fullSelectMenuButton_triggered(QAction *arg1);

protected:
    void paintEvent(QPaintEvent *event);
    bool event ( QEvent * event);

private:
    QAction *m_allAction = nullptr;
    QAction *m_letterAction = nullptr;
    QAction *m_funcAction = nullptr;
    QMenu *m_menu = nullptr;

    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *topHorizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *fullSelectToolButton;
    QToolButton *fullSelectMenuButton;
    QPushButton *minPushButton;
    QStackedWidget *fullStackedWidget;
    FullCommonUseWidget *fullCommonPage;
    FullLetterWidget *fullLetterPage;
    FullFunctionWidget *fullFunctionPage;
};

#endif // FULLMAINWINDOW_H
