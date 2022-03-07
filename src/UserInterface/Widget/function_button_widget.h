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

#ifndef FUNCTIONBUTTONWIDGET_H
#define FUNCTIONBUTTONWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "function_classify_button.h"
#include "ukuimenuinterface.h"

class FunctionButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionButtonWidget(QWidget *parent = nullptr);
    ~FunctionButtonWidget();

private:
    QStringList m_categoryList;
    QList<QAbstractButton *> m_buttonList;

protected:
    /**
     * @brief Initializes UI
     */
    void initUi();

    void hideEvent(QHideEvent *event);

Q_SIGNALS:
    /**
     * @brief Send button click signal to functionwidget interface
     * @param btnname: Category button name
     */
    void sendFunctionBtnSignal(QString btnname);
    void sendResetFunctionPage();

public Q_SLOTS:
    /**
     * @brief Respond to button click
     */
    void functionBtnClickedSlot();
    /**
     * @brief Receive function classification list
     */
    void recvClassificationBtnList();
};

#endif // FUNCTIONBUTTONWIDGET_H
