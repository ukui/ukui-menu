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
#include "src/UtilityFunction/letterclassifybutton.h"

class LetterButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LetterButtonWidget(QWidget *parent = nullptr);
    ~LetterButtonWidget();

protected:
    /**
     * @brief Initializes UI
     */
    void initUi();
    bool event(QEvent *event);

Q_SIGNALS:
    /**
     * @brief Send button click signal to letterwidget interface
     * @param btnname: Category button name
     */
    void sendLetterBtnSignal(QString btnname);
    void sendWidgetRepaintSignal(bool isrepaint);

public Q_SLOTS:
    /**
     * @brief Respond to button click
     */
    void letterBtnClickedSlot();
    /**
     * @brief Receive letters classification list
     * @param list: Letters classification list
     */
    void recvLetterBtnList(QStringList list);
};

#endif // LETTERBUTTONWIDGET_H
