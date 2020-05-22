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

#ifndef FUNCTIONCLASSIFYBUTTON_H
#define FUNCTIONCLASSIFYBUTTON_H
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QSvgRenderer>
#include <QPainter>
#include <QHBoxLayout>
#include "src/Style/style.h"

class FunctionClassifyButton : public QPushButton
{
    Q_OBJECT
public:
    /**
     * @param iconstr正常图片
     */
    FunctionClassifyButton(QWidget *parent,
               int width,
               int height,
               int iconSize,
               QString category,
               QString text,
               bool fullscreen,
               bool enabled);

private:
    enum State {
        Enabled,
        Disabled,
        Normal,
        Checked
    };

    int m_width;
    int m_height;
    int m_iconSize;
    QString m_category;
    QString m_text;
    bool m_fullscreen;
    bool m_enabled;
    QLabel* m_iconLabel=nullptr;
    QLabel* m_textLabel=nullptr;
    State m_state=Checked;

protected:
    void enterEvent(QEvent* e);
    void leaveEvent(QEvent* e);
    void updateIconState(const State state);
    void updateTextState(const State state);

Q_SIGNALS:
    void buttonClicked(QString category);

private Q_SLOTS:
    void reactToToggle(bool checked);
    void buttonClickedSlot();
};

#endif // FUNCTIONCLASSIFYBUTTON_H
