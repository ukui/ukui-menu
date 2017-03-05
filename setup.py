#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Copyright (C) 2015 by Mike Gabriel <mike.gabriel@das-netzwerkteam.de>
# Copyright: 2016,Tianjin KYLIN Information Technology Co., Ltd.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

import os
import sys

from glob import glob

from setuptools import setup

import DistUtilsExtra.command.build_extra
import DistUtilsExtra.command.build_i18n
import DistUtilsExtra.command.clean_i18n

# to update i18n .mo files (and merge .pot file into .po files) run on Linux:
# ,,python setup.py build_i18n -m''

# silence pyflakes, __VERSION__ is properly assigned below...
__VERSION__ = '0.0.0.0'
for line in file('lib/ukui-menu.py').readlines():
    if (line.startswith('__VERSION__')):
        exec(line.strip())
        break

PROGRAM_VERSION = __VERSION__

def datafilelist(installbase, sourcebase):
    datafileList = []
    for root, subFolders, files in os.walk(sourcebase):
        fileList = []
        for f in files:
            fileList.append(os.path.join(root, f))
        datafileList.append((root.replace(sourcebase, installbase), fileList))
    return datafileList

data_files = [
    ('{prefix}/share/man/man1'.format(prefix=sys.prefix), glob('data/*.1')),
    ('{prefix}/share/glib-2.0/schemas'.format(prefix=sys.prefix), glob('data/plugins/org.mate.ukui-menu.plugins.*.gschema.xml'),
    ),
    ('{prefix}/share/mate-panel/applets'.format(prefix=sys.prefix), ['data/org.mate.panel.UkuiMenuApplet.mate-panel-applet']),
    ('{prefix}/share/dbus-1/services'.format(prefix=sys.prefix), ['data/org.mate.panel.applet.UkuiMenuAppletFactory.service']),
    ('{prefix}/share/ukui-menu'.format(prefix=sys.prefix), ['data/ukui-menu.glade',
                                                            'data/popup.xml',
                                                            'data/ukuimenu.css',
                                                            'data/applications.list',
                                                           ],
    ),
    ('{prefix}/share/ukui-menu/plugins'.format(prefix=sys.prefix), ['data/plugins/ukuimenu.glade',
                                                                    'data/plugins/property.glade',
                                                                    'ukui_menu/plugins/menu.py',
                                                                   ],
    ),
    ('{prefix}/share/ukui-menu/icons'.format(prefix=sys.prefix), ['data/icons/ukui-logo.svg',
                                                                  'data/icons/allapp-active.png',
                                                                  'data/icons/allapp.png',
                                                                  'data/icons/arrow-left.png',
                                                                  'data/icons/arrow-right-line.png',
                                                                  'data/icons/arrow-right.png',
                                                                  'data/icons/computer-active.png',
                                                                  'data/icons/computer.png',
                                                                  'data/icons/controlcenter-active.png',
                                                                  'data/icons/controlcenter.png',
                                                                  'data/icons/favapp-active.png',
                                                                  'data/icons/favapp.png',
                                                                  'data/icons/sep.png',
                                                                  'data/icons/shutdown.svg',
                                                                  'data/icons/so-close.png',
                                                                  'data/icons/so.png',
                                                                  'data/icons/start-click.png',
                                                                  'data/icons/start-hover.png',
                                                                  'data/icons/start.png',
                                                                  'data/icons/stock_person.png',
                                                                 ],
    ),
    ('{prefix}/lib/ukui-menu'.format(prefix=sys.prefix), glob('lib/*')),
]
data_files.extend(datafilelist('{prefix}/share/locale'.format(prefix=sys.prefix), 'build/mo'))

cmdclass ={
            "build" : DistUtilsExtra.command.build_extra.build_extra,
            "build_i18n" :  DistUtilsExtra.command.build_i18n.build_i18n,
            "clean": DistUtilsExtra.command.clean_i18n.clean_i18n,
}

setup(
    name = "ukui-menu",
    version = PROGRAM_VERSION,
    description = "An advanced menu for UKUI. Supports filtering, favorites, autosession, and many other features.",
    license = 'GPLv2+',
    author = 'Huan Peng',
    url = 'https://github.com/ukui/ukui-menu',
    packages = [ 'ukui_menu', 'ukui_menu.plugins', ],
    package_dir = {
        '': '.',
    },
    data_files = data_files,
    install_requires = [ 'setuptools', ],
    scripts = ['ukui-menu'],
    cmdclass = cmdclass,
)
