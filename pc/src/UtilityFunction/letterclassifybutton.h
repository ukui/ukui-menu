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

#ifndef LETTERCLASSIFYBUTTON_H
#define LETTERCLASSIFYBUTTON_H
#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include "src/Style/style.h"

class LetterClassifyButton : public QToolButton
{
    Q_OBJECT
public:
    LetterClassifyButton(QWidget *parent,
                         QString hoverbg,
                         QString pressedbg,
                         QString letter
                         );

    bool is_pressed=false;

private:
    QString hoverbg;
    QString pressedbg;

protected:
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

Q_SIGNALS:
    void buttonClicked(QAbstractButton* btn);

private Q_SLOTS:
    void reactToToggle(bool checked);
};

#endif // LETTERCLASSIFYBUTTON_H
