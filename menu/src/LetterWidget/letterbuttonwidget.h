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

#ifndef LETTERBUTTONWIDGET_H
#define LETTERBUTTONWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "src/UtilityFunction/functionclassifybutton.h"

namespace Ui {
class LetterButtonWidget;
}

class LetterButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LetterButtonWidget(QWidget *parent = nullptr);
    ~LetterButtonWidget();

private:
    Ui::LetterButtonWidget *ui;
    QGridLayout* gridLayout=nullptr;

protected:
    void init_widget();

signals:
    /**
     * 向LetterWidget界面发送字母分类按钮被点击信号
     * @param btnname存放按钮名称
     */
    void send_letterbtn_signal(QString btnname);

private slots:
    void letterbtn_clicked_slot();//字母分类按钮槽函数
    void recv_letterbtn_list(QStringList list);//接收LetterWidget字母按钮列表
};

#endif // LETTERBUTTONWIDGET_H
