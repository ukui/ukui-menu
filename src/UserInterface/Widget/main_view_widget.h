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

#ifndef MAINVIEWWIDGET_H
#define MAINVIEWWIDGET_H

#include <QWidget>
#include <QSpacerItem>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QScrollBar>
#include <QFileSystemWatcher>
#include <QSettings>
#include "ukuimenuinterface.h"
#include "searchappthread.h"
#include "directory_changed_thread.h"
#include "convert_winid_to_desktop.h"
#include "style.h"
#include "src/UtilityFunction/utility.h"
#include <QComboBox>
#include <QCheckBox>

class MainViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainViewWidget(QWidget *parent = nullptr);
    ~MainViewWidget();

protected:

    void initUi();

    void paintEvent(QPaintEvent *event);

public Q_SLOTS:

Q_SIGNALS:

    void directoryChangedSignal();

    void sendDirectoryPath();
};

#endif // MAINVIEWWIDGET_H
