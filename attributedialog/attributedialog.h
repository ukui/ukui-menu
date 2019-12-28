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

    QVBoxLayout* mainLayout=nullptr;
    QWidget* titlebarWid=nullptr;
    QWidget* upWid=nullptr;
    QFrame* line=nullptr;
    QWidget* downWid=nullptr;
    //标题栏
    QHBoxLayout* titlebarwidLayout=nullptr;
    QLabel* appnameLabel=nullptr;
    QLabel* titlebarLabel=nullptr;
    ToolButton* closebtn=nullptr;
    QSpacerItem* leftSpacer=nullptr;
    QSpacerItem* rightSpacer=nullptr;
    //分割线上部控件
    QHBoxLayout* upwidLayout=nullptr;
    QLabel* labelIcon=nullptr;
    QLabel* labelappName=nullptr;
    //分割线下部控件
    QVBoxLayout* downwidLayout;
    QTableWidget* tableWid;

    QTextEdit* typeEdit=nullptr;
    QTextEdit* execEdit=nullptr;
    QTextEdit* commentEdit=nullptr;
    QTextEdit* typevalueEdit=nullptr;
    QTextEdit* execvalueEdit=nullptr;
    QTextEdit* commentvalueEdit=nullptr;

    //关闭按钮
    QWidget* closeWid;
    QHBoxLayout* closewidLayout;
    QSpacerItem* btnleftSpacer;
    QPushButton* btn;

protected:
    void init_widget();
    void set_controls_style(QTextEdit* edit,QTextEdit* valueEdit,QString str,int height);
};

#endif // ATTRIBUTEDIALOG_H
