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

#ifndef ATTRIBUTEDIALOG_H
#define ATTRIBUTEDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QSpacerItem>
#include <QToolButton>
#include <QPushButton>
#include <QTableWidget>
#include <QScrollBar>
#include <QPixmap>
#include <QTextEdit>
#include <ukuimenuinterface.h>
#include <QHeaderView>
#include <QMenu>
#include <QWidgetAction>
#include "windowmove.h"
#include "toolbutton.h"
#include "color.h"

namespace Ui {
class AttributeDialog;
}

class AttributeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AttributeDialog(QWidget *parent = nullptr);
    ~AttributeDialog();
    void set_attribute_value(QString desktopfp);

private:
    Ui::AttributeDialog *ui;

    UkuiMenuInterface* pUkuiMenuInterface=nullptr;
    WindowMove* wm=nullptr;

    QHBoxLayout* mainLayout=nullptr;
    QFrame* frame=nullptr;
    QVBoxLayout* frameLayout=nullptr;

    QWidget* titlebarWid=nullptr;
    QWidget* upWid=nullptr;
    QWidget* downWid=nullptr;
    QWidget* downmainWid;
    //标题栏
    QHBoxLayout* titlebarwidLayout=nullptr;
    ToolButton* closebtn=nullptr;
    QSpacerItem* leftSpacer=nullptr;
    //上部控件
    QHBoxLayout* upwidLayout=nullptr;
    QLabel* labelIcon=nullptr;
    QLabel* labelappName=nullptr;
    //下部控件
    QGridLayout* gridLayout=nullptr;
    QVBoxLayout* downwidLayout=nullptr;

//    QTextEdit* typeEdit=nullptr;
//    QTextEdit* execEdit=nullptr;
//    QTextEdit* commentEdit=nullptr;
//    QTextEdit* typevalueEdit=nullptr;
//    QTextEdit* execvalueEdit=nullptr;
//    QTextEdit* commentvalueEdit=nullptr;

    QLabel* typeLabel=nullptr;
    QLabel* execLabel=nullptr;
    QLabel* commentLabel=nullptr;
    QLineEdit* typevalueEdit=nullptr;
    QLineEdit* execvalueEdit=nullptr;
    QLineEdit* commentvalueEdit=nullptr;

    //关闭按钮
    QWidget* closeWid;
    QHBoxLayout* closewidLayout;
    QSpacerItem* btnleftSpacer;
    QPushButton* btn;

    //右键菜单
    QMenu* menu=nullptr;
    QWidgetAction* cutAction=nullptr;
    QWidget* cutWid=nullptr;
    QWidgetAction* copyAction=nullptr;
    QWidget* copyWid=nullptr;
    QWidgetAction* pasteAction=nullptr;
    QWidget* pasteWid=nullptr;
    QWidgetAction* deleteAction=nullptr;
    QWidget* deleteWid=nullptr;
    QWidgetAction* selectallAction=nullptr;
    QWidget* selectallWid=nullptr;

protected:
    void init_widget();
    void set_controls_style(QLabel* edit,QLineEdit* valueEdit,QString str,int height);
    void init_widget_action(QWidget* wid, QString textstr,bool enabled);

private slots:
    void show_menu();
    void cutaction_trigger_slot();
    void copyaction_trigger_slot();
    void pasteaction_trigger_slot();
    void deleteaction_trigger_slot();
    void selectallaction_trigger_slot();
};

#endif // ATTRIBUTEDIALOG_H
