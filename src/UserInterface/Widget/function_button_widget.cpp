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

#include "function_button_widget.h"
#include "style.h"
#include <QSvgRenderer>
#include <QPainter>
#include <QDebug>

FunctionButtonWidget::FunctionButtonWidget(QWidget *parent) :
    QWidget(parent)
{
    m_categoryList.clear();
    m_categoryList.append(QObject::tr("Office"));
    m_categoryList.append(QObject::tr("Development"));
    m_categoryList.append(QObject::tr("Image"));
    m_categoryList.append(QObject::tr("Video"));
    m_categoryList.append(QObject::tr("Internet"));
    m_categoryList.append(QObject::tr("Game"));
    m_categoryList.append(QObject::tr("Education"));
    m_categoryList.append(QObject::tr("Social"));
    m_categoryList.append(QObject::tr("System"));
    m_categoryList.append(QObject::tr("Safe"));
    m_categoryList.append(QObject::tr("Others"));
    initUi();
}

FunctionButtonWidget::~FunctionButtonWidget()
{
}

void FunctionButtonWidget::initUi()
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_StyledBackground, true);
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(40, 80, 40, 80);
    gridLayout->setSpacing(5);
    this->setLayout(gridLayout);

    for (int row = 0; row < 6; row++)
        for (int col = 0; col < 2; col++) {
            FunctionClassifyButton *iconbtn = new FunctionClassifyButton(Style::LeftBtnWidth,
                    Style::LeftBtnHeight,
                    Style::LeftIconSize,
                    m_categoryList.at(row * 2 + col),
                    false,
                    false);
            gridLayout->addWidget(iconbtn, row, col);
            m_buttonList.append(iconbtn);
            connect(iconbtn, &FunctionClassifyButton::buttonClicked, this, &FunctionButtonWidget::functionBtnClickedSlot);

            if (row * 2 + col == 10) {
                break;
            }
        }

    if (QGSettings::isSchemaInstalled(QString("org.ukui.style").toLocal8Bit())) {
        QGSettings *gsetting = new QGSettings(QString("org.ukui.style").toLocal8Bit());
        connect(gsetting, &QGSettings::changed, this, [ = ] {
            Q_FOREACH (QAbstractButton *btn, m_buttonList)
            {
                FunctionClassifyButton *fbtn = qobject_cast<FunctionClassifyButton *>(btn);
                fbtn->updateIconState();
            }
        });
    }
}

/**
 * 功能分类按钮槽函数
 */
void FunctionButtonWidget::functionBtnClickedSlot()
{
    FunctionClassifyButton *btn = dynamic_cast<FunctionClassifyButton *>(sender());
    QWidget *wid = btn->layout()->itemAt(0)->widget();
    QLabel *label = qobject_cast<QLabel *>(wid);
    Q_EMIT sendFunctionBtnSignal(label->text());
}

void FunctionButtonWidget::hideEvent(QHideEvent *event)
{
    Q_EMIT sendResetFunctionPage();
    return QWidget::hideEvent(event);
}

/**
 * 接收FunctionWidget界面分类按钮列表
 */
void FunctionButtonWidget::recvClassificationBtnList()
{
    QGridLayout *gridLayout = qobject_cast<QGridLayout *>(this->layout());

    for (int row = 0; row < 6; row++)
        for (int col = 0; col < 2; col++) {
            QLayoutItem *item = gridLayout->itemAtPosition(row, col);
            FunctionClassifyButton *btn = qobject_cast<FunctionClassifyButton *>(item->widget());

            if (UkuiMenuInterface::functionalVector.at(row * 2 + col).isEmpty()) {
                btn->m_enabled = false;
            } else {
                btn->m_enabled = true;
            }

            btn->setEnabled(btn->m_enabled);
            btn->updateBtnState();

            if (row * 2 + col == 10) {
                break;
            }
        }
}
