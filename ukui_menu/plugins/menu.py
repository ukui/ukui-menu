#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Copyright (C) 2007-2014 Clement Lefebvre <root@linuxmint.com>
# Copyright (C) 2015 Martin Wimpress <code@ubuntu-mate.org>
# Copyright (C) 2016,Tianjin KYLIN Information Technology Co., Ltd.
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

import gi
gi.require_version("Gtk", "3.0")
gi.require_version("Bamf", "3")

from gi.repository import Gtk, Pango, Gdk, Gio, GLib, GObject, GdkPixbuf#, Wnck
from gi.repository.GdkPixbuf import Pixbuf
from gi.repository import Bamf

import os
import sys
import time
import shutil
import string
import gettext
import subprocess
import filecmp
import ctypes
from ctypes import *
from ukui_menu.easybuttons import *
from ukui_menu.execute import Execute
from ukui_menu.feedback import Feedback
import dbus
from configobj import ConfigObj
import _thread
import ukuimenu
import operator
import locale
import configparser

class UkuiConfigParser(configparser.ConfigParser):
    def optionxform(self, optionstr):
        return optionstr

# i18n
gettext.install("ukui-menu", "/usr/share/locale")

ICON_PATH = "/usr/share/ukui-menu/icons/"
ICON_SIZE = 16
ukuimenu_settings = Gio.Settings.new("org.ukui.ukui-menu")
FAVNUM = 5            #常用软件显示个数


def get_user_icon():
    current_user = GLib.get_user_name()
    try:
        bus = Gio.bus_get_sync(Gio.BusType.SYSTEM, None)
        result = bus.call_sync('org.freedesktop.Accounts',
                               '/org/freedesktop/Accounts',
                               'org.freedesktop.Accounts',
                               'FindUserByName',
                               GLib.Variant ('(s)', (current_user,)),
                               GLib.VariantType.new ('(o)'),
                               Gio.DBusCallFlags.NONE,
                               -1,
                               None)
        (path,) = result.unpack()

        result = bus.call_sync('org.freedesktop.Accounts',
                               path,
                               'org.freedesktop.DBus.Properties',
                               'GetAll',
                               GLib.Variant ('(s)', ('org.freedesktop.Accounts.User',)),
                               GLib.VariantType.new ('(a{sv})'),
                               Gio.DBusCallFlags.NONE,
                               -1,
                               None)
        (props,) = result.unpack()
        usericon = props['IconFile']
        return usericon
    except Exception as e:
        print (e)
        return GLib.get_home_dir() + "/.face"

usericonPath = get_user_icon()

RecManagerInstance = Gtk.RecentManager.get_default()

APPLIST = [_("WPS Writer"),
           _("WPS Presentation"),
           _("WPS Spreadsheets"),
           _("Qt Creator"),
           _("SMPlayer"),
           _("Kylin Video"),
           _("Eye of MATE Image Viewer"),
           "图像查看器",
           _("Atril Document Viewer"),
           "文档查看器",
           _("Pluma"),
           "文本编辑器"]

WPS_W = ["application/wps-office.doc",
         "application/wps-office.docx",
         "application/wps-office.wps",
         "application/wps-office.wpt",
         "application/wps-office.dot"]
WPS_P = ["application/wps-office.pptx",
        "application/wps-office.ppt",
        "application/wps-office.pot",
        "application/wps-office.pps",
        "application/wps-office.dps",
        "application/wps-office.dpt"]
WPS_S = ["application/wps-office.xlsx",
        "application/wps-office.xls",
        "application/wps-office.et",
        "application/wps-office.ett",
        "application/wps-office.xlt"]

def get_user_item_path():
    item_dir = None

    if 'XDG_DATA_HOME' in os.environ:
        item_dir = os.path.join(os.environ['XDG_DATA_HOME'], 'applications')
    else:
        item_dir = os.path.join(os.environ['HOME'], '.local', 'share', 'applications')

    if not os.path.isdir(item_dir):
        os.makedirs(item_dir)

    return item_dir

def get_system_item_paths():
    item_dirs = None

    if 'XDG_DATA_DIRS' in os.environ:
        item_dirs = os.environ['XDG_DATA_DIRS'].split(":")
    else:
        item_dirs = [os.path.join('usr', 'share')]

    return item_dirs

def get_desktop_file_path(desktop_filename):
    if not desktop_filename:
        return ''
    #path = os.path.join(get_user_item_path(), desktop_filename)
    #if os.path.exists(path):
    #    return path
    for p in get_system_item_paths():
        path = os.path.join(p, 'applications', desktop_filename)
        if os.path.exists(path):
            return path

    return ''

class Category(GObject.GObject):
    def __init__(self, all_application_list):
        GObject.GObject.__init__ (self)

        self.category_list_m = []
        self.application_list_m = []
        self.current_category = None
        self.all_application_list = all_application_list

        for app in self.all_application_list:
            if app['category'] == _("Education") or app['category'] == _("Graphics") or app['category'] == _("Internet") or app['category'] == _("Programming") or app['category'] == _("Sound & Video") or app['category'] == _("System Tools") or app['category'] == _("Universal Access") or app['category'] == _("Preferences") or app['category'] == _("Administration") or app['category'] == _("Other"):
                continue
            elif app['category'] == _("Accessories") or app['category'] == _("Office") or app['category'] == _("Games") or app['category'] == _("Android"):
                self.application_list_m.append(app)
            else:
                self.application_list_m.append(app)

    def category_list(self):
        return self.category_list_m

    def application_list(self):
        return self.application_list_m

class Menu:
    def __init__(self, MenuToLookup):
        self.tree = ukuimenu.lookup_tree(MenuToLookup)
        self.directory = self.tree.get_root_directory()

    def getMenus(self, parent = None):
        if parent == None:
            yield self.tree.root
        else:
            for menu in parent.get_contents():
                if menu.get_type() == ukuimenu.TYPE_DIRECTORY and self.__isVisible(menu):
                    yield menu

    def getItems(self, menu):
        for item in menu.get_contents():
            if item.get_type() == ukuimenu.TYPE_ENTRY and item.get_desktop_file_id()[-19:] != '-usercustom.desktop' and self.__isVisible(item):
                yield item

    def __isVisible(self, item):
        if item.get_type() == ukuimenu.TYPE_ENTRY:
            return not (item.get_is_excluded() or item.get_is_nodisplay())
        if item.get_type() == ukuimenu.TPYE_DIRECTORY and len( item.get_contents()):
            return True

class pluginclass( object ):
    TARGET_TYPE_TEXT = 80
    toButton = (Gtk.TargetEntry.new("text/uri-list", 0, TARGET_TYPE_TEXT), Gtk.TargetEntry.new("text/uri-list", 0, TARGET_TYPE_TEXT))
    TARGET_TYPE_FAV = 81
    toFav = (Gtk.TargetEntry.new("FAVORITES", Gtk.TargetFlags.SAME_APP, 81), Gtk.TargetEntry.new("text/plain", 0, 100 ), Gtk.TargetEntry.new("text/uri-list", 0, 101))
    fromFav = (Gtk.TargetEntry.new("FAVORITES", Gtk.TargetFlags.SAME_APP, 81), Gtk.TargetEntry.new("FAVORITES", Gtk.TargetFlags.SAME_APP, 81))

    def __init__(self, ukuiMenuWin, showCategoryMenu):
        self.ukuiMenuWin = ukuiMenuWin
        self.showCategoryMenu = showCategoryMenu
        self.de = "ukui"

        self.showRecentFile = ukuimenu_settings.get_boolean("show-recent-file")

        self.builder = Gtk.Builder()
        #The Glade file for the plugin
        self.builder.add_from_file (os.path.join('/', 'usr', 'share', 'ukui-menu',  'plugins', 'ukuimenu.glade'))
        ukuimenu_settings.connect("changed::user-icon-changed", self.changeimage)
        ukuimenu_settings.connect("changed::show-recent-file", self.RecentFileChanged)
        ukuimenu_settings.connect("changed::recent-app-num", self.updateHistoryBox)
        self.windowHeight = 505
        self.addedHeight = 0
        self.ButtonHoverWidgetHistory = None

        self.window = self.builder.get_object( "window" )
        self.expanderApp = self.builder.get_object("expanderApp")
        self.labelExpanderApp = self.builder.get_object("labelExpanderApp")
        self.expanderSystem = self.builder.get_object("expanderSystem")
        self.labelExpanderSystem = self.builder.get_object("labelExpanderSystem")
        self.categoriesApp = self.builder.get_object("categoriesApp")
        self.categoriesSystem = self.builder.get_object("categoriesSystem")
        self.expanderRecent = self.builder.get_object("expanderRecent")
        self.recentBox = self.builder.get_object("recentBox")
        self.expanderRecent.hide()
        self.labelExpander = self.builder.get_object("labelExpander")
        self.labelExpander.set_text(_("Recent"))
        self.content_holder = self.builder.get_object("eventbox")
        self.content_holder.connect( "key-press-event", self.keyPress )
        self.content_holder.set_size_request(575, self.windowHeight)

        self.layout_left = self.builder.get_object("layout_left")
        self.viewport1 = self.builder.get_object("viewport1")
        self.viewport1.set_name("Viewport2")
        self.viewport2 = self.builder.get_object("viewport2")
        self.viewport2.set_name("Viewport2")
        self.vp_right_container = self.builder.get_object("vp_right_container")
        self.vp_right_container.set_name("Viewport1")

        self.viewport_left = self.builder.get_object("viewport_left")
        self.viewport_left.set_name("Viewport1")
        self.viewport_right = self.builder.get_object("viewport_right")
        self.viewport_right.set_name("Viewport1")
        self.vp_left_container = self.builder.get_object("vp_left_container")
        self.vp_left_container.set_name("Viewport1")
        self.scrolledwindow_fav = self.builder.get_object("scrolledwindow_fav")
        self.scrolledwindow_fav.set_name("Viewport1")
        self.scrolledwindow_rec = self.builder.get_object("scrolledwindow_recent")
        self.scrolledwindow_rec.set_name("Viewport1")
        self.applicationsScrolledWindow = self.builder.get_object("applicationsScrolledWindow")
        self.applicationsScrolledWindow.set_name("Viewport1")
        self.scrolledwindow1 = self.builder.get_object("scrolledwindow1")
        self.scrolledwindow1.set_name("Viewport1")
        self.scrolledwindow2 = self.builder.get_object("scrolledwindow2")
        self.scrolledwindow2.set_name("Viewport1")
        self.viewport_fav = self.builder.get_object("viewport_fav")
        self.viewport_fav.set_name("Viewport1")
        self.viewport_allapp = self.builder.get_object("viewport_allapp")
        self.viewport_allapp.set_name("Viewport1")
        self.viewport8 = self.builder.get_object("viewport8")
        self.viewport8.set_name("Viewport1")
        self.viewport9 = self.builder.get_object("viewport9")
        self.viewport9.set_name("Viewport1")
        self.viewport12 = self.builder.get_object("viewport12")
        self.viewport12.set_name("Viewport1")
        self.viewport16 = self.builder.get_object("viewport16")
        self.viewport16.set_name("Viewport1")

        self.button_user = self.builder.get_object("button_user")
        self.button_user.set_name("ButtonUser")
        self.button_user.connect("button-press-event", self.on_button_user_clicked)

        self.button_showall = self.builder.get_object("button_showall")
        self.button_showall.set_label(_("All Apps"))
        self.button_showall.set_name("ButtonApp")
        self.button_showall.connect("clicked", self.on_button_showall_clicked)

        self.button_back = self.builder.get_object("button_back")
        self.button_back.set_label(_("Favorites"))
        self.button_back.set_name("ButtonApp")
        self.button_back.connect("clicked", self.on_button_back_clicked)

        self.image_user = self.builder.get_object("image_user")
        if not os.path.exists(usericonPath):
            iconpath = "/usr/share/ukui-menu/icons/stock_person.png"
            os.system("cp %s %s" % (iconpath, usericonPath))
        else:
            iconpath = usericonPath
        try:
            pixbuf = Pixbuf.new_from_file(iconpath)
        except Exception as e:
            iconpath = "/usr/share/ukui-menu/icons/stock_person.png"
            os.system("cp %s %s" % (iconpath, usericonPath))
            pixbuf = Pixbuf.new_from_file(iconpath)
            print (e)
        pixbuf = pixbuf.scale_simple(69, 69, 2)  #2 := BILINEAR
        self.image_user.set_from_pixbuf(pixbuf)

        self.image1 = self.builder.get_object("image1")
        iconfile = ICON_PATH + "sep.png"
        pixbuf = Pixbuf.new_from_file(iconfile)
        pixbuf = pixbuf.scale_simple(210, 1, 2)  #2 := BILINEAR
        self.image1.set_from_pixbuf(pixbuf)
        self.image2 = self.builder.get_object("image2")
        iconfile = ICON_PATH + "sep.png"
        pixbuf = Pixbuf.new_from_file(iconfile)
        pixbuf = pixbuf.scale_simple(210, 1, 2)  #2 := BILINEAR
        self.image2.set_from_pixbuf(pixbuf)
        self.image3 = self.builder.get_object("image3")
        iconfile = ICON_PATH + "sep.png"
        pixbuf = Pixbuf.new_from_file(iconfile)
        pixbuf = pixbuf.scale_simple(80, 1, 2)  #2 := BILINEAR
        self.image3.set_from_pixbuf(pixbuf)
        self.image5 = self.builder.get_object("image5")
        iconfile = ICON_PATH + "sep.png"
        pixbuf = Pixbuf.new_from_file(iconfile)
        pixbuf = pixbuf.scale_simple(210, 1, 2)  #2 := BILINEAR
        self.image5.set_from_pixbuf(pixbuf)

        self.button_shutdown = self.builder.get_object("button_shutdown")
        self.button_shutdown.set_name("ButtonPower")
        self.viewport18 = self.builder.get_object("viewport18")
        self.viewport18.set_name("ViewportPower")
        self.button_shutdown.set_label(_("Shutdown"))
        self.change_icon(self.button_shutdown, ICON_PATH + "shutdown.svg")
        self.button_shutdown.connect("button-press-event", self.shutdown)
        self.button_shutdown.connect("enter", self.button_shutdown_enter)
        self.button_shutdown.connect("leave", self.button_shutdown_leave)

        self.button_shutdown1 = self.builder.get_object("button_shutdown1")
        self.button_shutdown1.set_name("ButtonPower")
        self.viewport23 = self.builder.get_object("viewport23")
        self.viewport23.set_name("ViewportPower")
        self.button_shutdown1.connect("button-press-event", self.menu_right_click)
        self.button_shutdown1.connect("enter", self.button_shutdown1_enter)
        self.button_shutdown1.connect("leave", self.button_shutdown1_leave)

        self.image_showall = self.builder.get_object("image_showall")
        iconfile = ICON_PATH + "arrow-right.png"
        pixbuf = Pixbuf.new_from_file(iconfile)
        pixbuf = pixbuf.scale_simple(5, 7, 2)  #2 := BILINEAR
        self.image_showall.set_from_pixbuf(pixbuf)

        self.image_shutdown1 = self.builder.get_object("image_shutdown1")
        iconfile = ICON_PATH + "arrow-right.png"
        pixbuf = Pixbuf.new_from_file(iconfile)
        pixbuf = pixbuf.scale_simple(5, 7, 2)  #2 := BILINEAR
        self.image_shutdown1.set_from_pixbuf(pixbuf)

        self.image_back = self.builder.get_object("image_back")
        iconfile = ICON_PATH + "arrow-left.png"
        pixbuf = Pixbuf.new_from_file(iconfile)
        pixbuf = pixbuf.scale_simple(5, 7, 2)  #2 := BILINEAR
        self.image_back.set_from_pixbuf(pixbuf)

        #recentbox
        self.layout_applications_left = self.builder.get_object("layout_applications_left")
        self.layout_applications_right = self.builder.get_object("layout_applications_right")
        self.FileList=[]
        self.favAppList = []
        #dirty ugly hack, to get favorites drag origin position
        self.drag_origin = None

        self.favoritesPath = os.path.join(GLib.get_home_dir(), ".config","ukui-menu","applications.list")
        self.favoritesBox = self.builder.get_object("favoritesBox")
        self.favoritesBox.set_row_spacings(3)
        self.favoritesBox.connect( "drag-data-received", self.ReceiveCallback )
        self.favoritesBox.drag_dest_set ( Gtk.DestDefaults.MOTION | Gtk.DestDefaults.HIGHLIGHT | Gtk.DestDefaults.DROP,  self.toButton, Gdk.DragAction.COPY )

        self.categoriesBox = self.builder.get_object("categoriesbox")
        self.categoriesBox.set_spacing(2)
        self.applicationsBox = self.builder.get_object("applicationsBox")
        self.applicationsBox.set_spacing(2)
        self.applicationsScrolledWindow = self.builder.get_object( "applicationsScrolledWindow")

        self.favappbutton = self.builder.get_object("button_favapp")
        self.favappbutton.set_label(_("Favorites"))
        self.favappbutton.set_name("Button")
        self.change_icon(self.favappbutton, ICON_PATH + "favapp.png")
        self.favappbutton.connect("enter", self.favappbutton_enter)
        self.favappbutton.connect("leave", self.favappbutton_leave)
        self.favappbutton.connect("clicked", lambda c1: self.changeTab(0))

        self.viewportfav = self.builder.get_object("viewportfav")
        self.viewportallapp = self.builder.get_object("viewportallapp")

        self.allappbutton = self.builder.get_object("button_allapp")
        self.allappbutton.set_label(_("All Apps"))
        self.allappbutton.set_name("Button")
        self.change_icon(self.allappbutton, ICON_PATH + "allapp.png")
        self.allappbutton.connect("enter", self.allappbutton_enter)
        self.allappbutton.connect("leave", self.allappbutton_leave)
        self.allappbutton.connect("clicked", lambda c2: self.changeTab(1))

        self.viewport_entry = self.builder.get_object("viewport_entry")
        self.viewport_entry.set_name("Viewport1")
        self.searchEntry =self.builder.get_object( "search_entry" )
        self.searchEntry.set_name("Entry")
        self.searchEntry.set_text(_("Search local program"))
        self.searchEntry.modify_text(Gtk.StateType.NORMAL, Gdk.color_parse('#999'))
        iconpath = ICON_PATH + "so.png"
        pixbuf = Pixbuf.new_from_file(iconpath)
        pixbuf = pixbuf.scale_simple(16, 16, 2)  #2 := BILINEAR
        self.searchEntry.set_icon_from_pixbuf(1, pixbuf)
        self.searchEntry.set_icon_activatable(1, True)
        self.searchEntry.connect("changed", self.searchChanged)
        self.searchEntry.connect("button-press-event", self.entryPopup)
        self.searchEntry.connect("focus-out-event", self.focusOut)
        self.searchEntry.connect("focus-in-event", self.focusIn)
        self.searchEntry.connect("icon-press", self.searchIconPressed)

        self.searchResultBox = self.builder.get_object( "searchResultBox" )
        self.searchResultBox.set_name("AppBox")
        self.searchResultBox.set_spacing(3)
        self.recentAppBox = self.builder.get_object( "RecentAppBox" )
        self.recentAppBox.set_spacing(3)

        self.bamfok = False
        GLib.timeout_add( 30000, self.getBamfStatus )

        self.label_user = self.builder.get_object("label_user")
        user_name = GLib.get_user_name()
        self.label_user.set_text(user_name)
        #left-menu
        self.button_computer = self.builder.get_object( "button_computer" )
        self.button_computer.set_name("Button")
        self.button_computer.set_label(_("Home"))
        self.change_icon(self.button_computer, ICON_PATH + "computer.png")
        self.button_computer.connect("button-press-event",self.on_computer_clicked)
        self.button_computer.connect("enter", self.button_computer_enter)
        self.button_computer.connect("leave", self.button_computer_leave)

        self.button_controlcenter = self.builder.get_object( "button_controlcenter" )
        self.button_controlcenter.set_name("Button")
        self.button_controlcenter.set_label(_("Settings"))
        self.change_icon(self.button_controlcenter, ICON_PATH + "controlcenter.png")
        self.button_controlcenter.connect("button-press-event", self.on_controlcenter_clicked)
        self.button_controlcenter.connect("enter", self.button_controlcenter_enter)
        self.button_controlcenter.connect("leave", self.button_controlcenter_leave)

        self.button_feedback = self.builder.get_object("button_feedback")
        self.button_feedback.set_name("Button")
        self.button_feedback.set_label(_("Feedback"))
        self.change_icon(self.button_feedback, ICON_PATH + "feedback.png")
        self.button_feedback.connect("button-press-event", self.on_feedback_clicked)
        self.button_feedback.connect("enter", self.button_feedback_enter)
        self.button_feedback.connect("leave", self.button_feedback_leave)


        self.label3 = self.builder.get_object("label3")
        self.label3.set_text(_("No items matched"))

        self.iconSize = 24
        self.favCols = 1

        self.buildingButtonList = False
        self.stopBuildingButtonList = False

        self.rebuildLock = False
        self.activeFilter = (1, "", self.searchEntry)
        self.current_suggestion = None
        self.applicationList = []
        self.uncategoriedList = []
        self.all_app_list = []

        self.all_application_list = []
        for mainitems in [ "ukui-applications.menu", "ukui-settings.menu" ]:
            mymenu = Menu( mainitems )
            mymenu.tree.add_monitor( self.menuChanged, None )

        self.lastActiveTab = 0

    def getBamfStatus(self):
        if self.bamfok:
            return

        self.bamfok = True
        try:
            self.matcher = Bamf.Matcher.get_default()
            self.matcher.connect('view-opened', self.viewOpened, None)
        except Exception as e:
            #print (e)
            return
        self.bamfok = False

    def clearAllRecent(self, *args, **kargs):
        if os.path.exists(GLib.get_home_dir() + "/.recentAppLog"):
            os.remove(GLib.get_home_dir() + "/.recentAppLog")
        self.updateHistoryBox()

    def change_icon(self, widget, iconfile):
        pixbuf = Pixbuf.new_from_file(iconfile)
        pixbuf = pixbuf.scale_simple(ICON_SIZE, ICON_SIZE, 2)  #2 := BILINEAR
        image = Gtk.Image()
        image.set_from_pixbuf(pixbuf)
        widget.set_image(image)

    def favappbutton_enter(self, *args, **kargs):
        self.change_icon(self.favappbutton, ICON_PATH + "favapp-active.png")
    def favappbutton_leave(self, *args, **kargs):
        self.change_icon(self.favappbutton, ICON_PATH + "favapp.png")

    def button_shutdown_enter(self, *args, **kargs):
        self.viewport18.set_name("ViewportPowerEnter")
    def button_shutdown_leave(self, *args, **kargs):
        self.viewport18.set_name("ViewportPower")

    def button_shutdown1_enter(self, *args, **kargs):
        self.viewport23.set_name("ViewportPowerEnter")
    def button_shutdown1_leave(self, *args, **kargs):
        self.viewport23.set_name("ViewportPower")

    def allappbutton_enter(self, *args, **kargs):
        self.change_icon(self.allappbutton, ICON_PATH + "allapp-active.png")
    def allappbutton_leave(self, *args, **kargs):
        self.change_icon(self.allappbutton, ICON_PATH + "allapp.png")

    def button_computer_enter(self, *args, **kargs):
        self.change_icon(self.button_computer, ICON_PATH + "computer-active.png")
    def button_computer_leave(self, *args, **kargs):
        self.change_icon(self.button_computer, ICON_PATH + "computer.png")
    def on_computer_clicked(self, widget ,event):
        realPath = GLib.get_home_dir()
        current_desktop = os.getenv("XDG_CURRENT_DESKTOP")
        if current_desktop == "GNOME":
            os.system("nautilus %s" % realPath)
        elif current_desktop == "MATE":
            os.system("caja %s" % realPath)
        else:
            os.system("peony %s" % realPath)

    def button_controlcenter_enter(self, *args, **kargs):
        self.change_icon(self.button_controlcenter, ICON_PATH + "controlcenter-active.png")
    def button_controlcenter_leave(self, *args, **kargs):
        self.change_icon(self.button_controlcenter, ICON_PATH + "controlcenter.png")
    def on_controlcenter_clicked(self, widget, event):
        current_desktop = os.getenv("XDG_CURRENT_DESKTOP")
        if current_desktop == "GNOME":
            os.system("gnome-control-center &")
        elif current_desktop == "MATE":
            os.system("mate-control-center &")
        else:
            os.system("ukui-control-center &")

    def button_feedback_enter(self, *args, **kargs):
        self.change_icon(self.button_feedback, ICON_PATH + "feedback-active.png")
    def button_feedback_leave(self, *args, **kargs):
        self.change_icon(self.button_feedback, ICON_PATH + "feedback.png")
    def on_feedback_clicked(self, widget, event):
        feedback =  Feedback()
        feedback.run()

    def ShowRecentFile(self, widget):
        widget.rightbox.hide()
        if widget.appName not in APPLIST:
            return
        RecentInfo = RecManagerInstance.get_items()
        #wps
        if widget.appName == _("WPS Writer"):
            for item in RecentInfo:
                if item.get_mime_type() in WPS_W:
                    widget.rightbox.show_all()
        if widget.appName == _("WPS Presentation"):
            for item in RecentInfo:
                if item.get_mime_type() in WPS_P:
                    widget.rightbox.show_all()
        if widget.appName == _("WPS Spreadsheets"):
            for item in RecentInfo:
                if item.get_mime_type() in WPS_S:
                    widget.rightbox.show_all()
        #gtk app
        if widget.appName == _("Pluma") or widget.appName == "文本编辑器":
            for item in RecentInfo:
                if item.has_application("Pluma"):
                    widget.rightbox.show_all()
        if widget.appName == _("Eye of MATE Image Viewer") or widget.appName == "图像查看器":
            for item in RecentInfo:
                if item.has_application("Eye of MATE Image Viewer"):
                    widget.rightbox.show_all()
        if widget.appName == _("Atril Document Viewer") or widget.appName == "文档查看器":
            for item in RecentInfo:
                if item.has_application("Atril Document Viewer"):
                    widget.rightbox.show_all()
        #qt app
        if widget.appName == _("Qt Creator"):
            cf = UkuiConfigParser()
            path = os.path.join(GLib.get_user_config_dir(), 'QtProject/QtCreator.ini')
            if os.path.exists(path):
                cf.read(path)
                recentFiles = cf.get("RecentFiles", "Files")
                if recentFiles is not "" and recentFiles != "@Invalid()":
                    widget.rightbox.show_all()
        if widget.appName == _("Kylin Video"):
            cf = UkuiConfigParser()
            path = os.path.join(GLib.get_user_config_dir(), 'kylin-video/kylin-video.ini')
            if os.path.exists(path):
                cf.read(path)
                recentFiles = cf.get("history", "recents")
                if recentFiles is not "":
                    widget.rightbox.show_all()

    def AddFavBtn(self):
        self.favorites = []
        try:
            self.checkUkuiMenuFolder()
            if not os.path.isfile(self.favoritesPath):
                shutil.copyfile("/usr/share/ukui-menu/applications.list", self.favoritesPath)

            applicationsFile = open(self.favoritesPath, "r")
            applicationsList = applicationsFile.readlines()
            applicationsFile.close()
            for child in self.favoritesBox:
                child.destroy()
            position = 0
            for app in applicationsList:
                app = app.strip()
                if app[0:9] == "location:":
                    favButton = self.favoritesBuildLauncher( app[9:] )
                if favButton:
                    favButton.set_name("ButtonApp")
                    favButton.position = position
                    self.favorites.append( favButton )
                    self.favoritesPositionOnGrid( favButton )
                    favButton.connect( "drag-data-received", self.onFavButtonDragReorder )
                    favButton.drag_dest_set( Gtk.DestDefaults.MOTION | Gtk.DestDefaults.HIGHLIGHT | Gtk.DestDefaults.DROP, self.fromFav, Gdk.DragAction.COPY )
                    favButton.connect( "drag-data-get", self.onFavButtonDragReorderGet )
                    favButton.drag_source_set( Gdk.ModifierType.BUTTON1_MASK, self.toFav, Gdk.DragAction.COPY )
                    position += 1

            self.favoritesSave()
        except Exception as e:
            print (e)
            
        Gdk.flush()

    def favoritesSave(self):
        try:
            self.checkUkuiMenuFolder()
            appListFile = open(self.favoritesPath, "w")

            for favorite in self.favorites:
                if favorite.type == "location":
                    appListFile.write( "location:" + favorite.desktopFile + "\n" )
                else:
                    appListFile.write( favorite.type + "\n" )
 
            appListFile.close()
        except Exception as e:
            print (e)

    def isLocationInFavorites(self, location):
        for fav in self.favorites:
            if fav.type == "location" and fav.desktopFile == location:
                return True

        return False

    def checkUkuiMenuFolder(self):
        if os.path.exists( os.path.join( os.path.expanduser("~"), ".config", "ukui-menu", "applications" ) ):
            return True
        try:
            os.makedirs( os.path.join( os.path.expanduser("~"), ".config", "ukui-menu", "applications" ) )
            return True
        except:
            pass

        return False

    def favoritesPositionOnGrid(self, favorite):
        row = 0
        col = 0
        for fav in self.favorites:
            if fav.position == favorite.position:
                break
            row +=1
        self.favoritesBox.attach(favorite, col, col + 1, row, row + 1) #table自适应填充，随fav多少高度跟随改变

    def onFavButtonDragReorderGet(self, widget, context, selection, targetType, eventTime ):
        if targetType == self.TARGET_TYPE_FAV:
            self.drag_origin = widget.position
            selection.set( selection.get_target(), 8, str(widget.position).encode())

    def onFavButtonDragReorder(self, widget, context, x, y, selection, targetType, time):
        if targetType == self.TARGET_TYPE_FAV:
            #self.favoritesReorder( int(selection.data), widget.position )
            self.favoritesReorder( self.drag_origin, widget.position )

    def favoritesReorder( self, oldposition, newposition ):
        if oldposition == newposition:
            return
        tmp = self.favorites[ oldposition ]
        if newposition > oldposition:
            if ( self.favorites[ newposition - 1 ].type == "space" or self.favorites[ newposition - 1 ].type == "separator" ) and self.favCols > 1:
                newposition = newposition - 1
            for i in range( oldposition, newposition ):
                self.favorites[ i ] = self.favorites[ i + 1 ]
                self.favorites[ i ].position = i
        elif newposition < oldposition:
            for i in range( 0,  oldposition - newposition ):
                self.favorites[ oldposition - i ] = self.favorites[ oldposition - i - 1 ]
                self.favorites[ oldposition - i ] .position = oldposition - i
        self.favorites[ newposition ] = tmp
        self.favorites[ newposition ].position = newposition

        for fav in self.favorites:
            self.favoritesBox.remove( fav )
            self.favoritesPositionOnGrid( fav )

        self.favoritesSave()
        self.favoritesBox.resize( self.favoritesGetNumRows(), self.favCols )

    def openApp(self, widget, FileData):
        Data, FileName = FileData
        uri, AppName, appname = Data
        self.mateMenuWin.hide()

        if appname == _("WPS Writer"):
            os.system("wps &")
        if appname == _("WPS Presentation"):
            os.system("wpp &")
        if appname == _("WPS Spreadsheets"):
            os.system("et &")
        if appname == _("Pluma") or appname == "文本编辑器":
            os.system("pluma &")

    def callback(self, widget, FileData):
        Data, FileName = FileData
        uri, AppName, appname = Data
        self.ukuiMenuWin.hide()

        if os.path.exists(FileName):
            pass
        else:
            msg = _("\nFile not exists, it was moved or deleted!\n\nIt will be automatically removed from the history list!\n")
            md = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING, Gtk.ButtonsType.OK, msg)
            md.run()
            md.destroy()
            self.onRecentFileRemove(None, Data)
            return
        if " " in FileName:
            FileName = FileName.replace(" ", "\ ")
        if "&" in FileName:
            FileName = FileName.replace("&", "\&")

        if appname == _("WPS Writer"):
            os.system("wps %s &" % FileName)
        if appname == _("WPS Presentation"):
            os.system("wpp %s &" % FileName)
        if appname == _("WPS Spreadsheets"):
            os.system("et %s &" % FileName)
        if appname == _("Qt Creator"):
            os.system("qtcreator %s &" % FileName)
        if appname == _("Kylin Video"):
            os.system("kylin-video %s &" % FileName)
        if appname == _("Eye of MATE Image Viewer") or appname == "图像查看器":
            os.system("eom %s &" % FileName)
        if appname == _("Atril Document Viewer") or appname == "文档查看器":
            os.system("atril %s &" % FileName)
        if appname == _("Pluma") or appname == "文本编辑器":
            os.system("pluma %s &" % FileName)

    def favoritesBuildLauncher(self, location):
        try:
            ButtonIcon = None
            #location = string.join(location.split("%20"))
            for fav in self.favorites:
                if fav.type == "location" and fav.desktopFile == location:
                    return None
            favButton = FavApplicationLauncher( location, 32, False)
            if favButton.appExec:
                favButton.show()
                favButton.connect( "clicked", lambda w: self.ukuiMenuWin.hide() )
                favButton.connect( "button-press-event", self.favPopup )
                #favButton.set_tooltip_text( favButton.getTooltip() )
                favButton.type = "location"
                if self.showRecentFile:
                    self.ShowRecentFile(favButton)
                    favButton.connect( "enter", self.showHistory )
                    favButton.connect( "leave", self.hideHistory )
                return favButton
        except Exception as e:
            print ("s")

    def entryPopup( self, widget, ev ):
        if ev.button == 3:
            self.ukuiMenuWin.stopHiding()

    def focusOut( self, widget, ev ):
        self.searchEntry.set_name("Entry")
        self.searchEntry.set_text(_("Search local program"))
        iconpath = ICON_PATH + "so.png"
        pixbuf = Pixbuf.new_from_file(iconpath)
        pixbuf = pixbuf.scale_simple(16, 16, 2)  #2 := BILINEAR
        self.searchEntry.set_icon_from_pixbuf(1, pixbuf)

    def focusIn( self, widget, ev ):
        text = self.searchEntry.get_text()
        if text == _("Search local program"):
            self.searchEntry.set_text("")

    def favPopup( self, widget, ev ):
        button = MenuApplicationLauncher(widget.desktopFile, 32, "", True, highlight=(False))
        button.set_name("ButtonAppHover")
        addedDesktop = False
        try:
            if (os.path.exists(GLib.get_user_special_dir(GLib.USER_DIRECTORY_DESKTOP) + "/" + os.path.basename(widget.desktopFile))):
                addedDesktop = True
        except Exception as detail:
            print (detail)

        text = ""
        if ev.button == 3:
            if widget.type == "location":
                if (os.path.exists(GLib.get_home_dir() + "/.applet")):
                    f = open(GLib.get_home_dir() + "/.applet")
                    text = f.read()
                    f.close()

                mTree = Gtk.Menu()
                mTree.set_name("myGtkLabel")
                mTree.set_events(Gdk.EventMask.POINTER_MOTION_MASK | Gdk.EventMask.POINTER_MOTION_HINT_MASK |
                                 Gdk.EventMask.BUTTON_PRESS_MASK | Gdk.EventMask.BUTTON_RELEASE_MASK)

                launchMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Open(_O)"))
                image = Gtk.Image.new_from_icon_name("gtk-execute", Gtk.IconSize.MENU)
                launchMenuItem.set_image(image)

                separator1 = Gtk.SeparatorMenuItem()

                desktopMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Add to desktop(_A)"))
                image = Gtk.Image.new_from_icon_name("gtk-add", Gtk.IconSize.MENU)
                desktopMenuItem.set_image(image)
                if addedDesktop:
                    desktopMenuItem.set_sensitive(False)

                panelMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Lock to panel(_L)"))
                #image = Gtk.Image.new_from_icon_name("gtk-lock", Gtk.IconSize.MENU)
                #panelMenuItem.set_image(image)

                separator3 = Gtk.SeparatorMenuItem()

                removeFromFavMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Unlock from startup menu(_U)"))
                #image = Gtk.Image.new_from_icon_name("gtk-unlock", Gtk.IconSize.MENU)
                #removeFromFavMenuItem.set_image(image)
                if len(self.favorites) < 2:
                    removeFromFavMenuItem.set_sensitive(False)

                separator4 = Gtk.SeparatorMenuItem()

                propsMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Property(_P)"))
                image = Gtk.Image.new_from_icon_name("gtk-properties", Gtk.IconSize.MENU)
                propsMenuItem.set_image(image)

                desktopMenuItem.connect("activate", self.add_to_desktop, widget)
                panelMenuItem.connect("activate", self.add_to_panel, widget)
                launchMenuItem.connect( "activate", self.onLaunchApp, widget)
                removeFromFavMenuItem.connect( "activate", self.onFavoritesRemove, widget )
                propsMenuItem.connect( "activate", self.onPropsApp, widget)

                mTree.append(launchMenuItem)
                mTree.append(separator1)
                if self.de == "ukui":
                    mTree.append(desktopMenuItem)
                    mTree.append(panelMenuItem)
                    if button.appName in text:
                        panelMenuItem.set_sensitive(False)
                    mTree.append(separator3)
                mTree.append(removeFromFavMenuItem)
                mTree.append(separator4)
                mTree.append(propsMenuItem)

                mTree.show_all()

                mTree.connect("deactivate", self.set_item_state, widget)

                self.ukuiMenuWin.stopHiding()
                widget.remove(widget.Align1)
                widget.Align1.remove(widget.HBox1)
                widget.viewport.show()
                widget.viewport.add(widget.HBox1)
                widget.add(widget.viewport)
                widget.set_name("ButtonAppHover")
                mTree.attach_to_widget(widget)
                mTree.popup(None, None, None, None, ev.button, ev.time)

    def get_realPath(self, path, configDir):
        realPath = GLib.get_home_dir() + path
        try:
            config = ConfigObj(GLib.get_home_dir() + "/.config/user-dirs.dirs")
            chinesePath = config[configDir]
            chinesePath = subprocess.getoutput("echo "+ chinesePath)
            if os.path.exists(chinesePath):
                realPath = chinesePath
        except Exception as detail:
            print (detail)
        return realPath

    def on_button_user_clicked (self, widget, event):
        self.ukuiMenuWin.hide()
        if os.path.exists("/usr/bin/ukui-control-center"):
            os.system("ukui-control-center -u &")
        elif os.path.exists("/usr/bin/mate-about-me"):
            os.system('mate-about-me &')

    def changeimage(self, settings, key, args = None):
        usericonPath = get_user_icon()
        try:
            pixbuf = Pixbuf.new_from_file(usericonPath)
        except Exception as e:
            iconpath = "/usr/lib/ubuntu-mate/mate-menu/stock_person.png"
            os.system("cp %s %s" % (iconpath, usericonPath))
            pixbuf = Pixbuf.new_from_file(iconpath)
            print (e)
        pixbuf = pixbuf.scale_simple(69, 69, 2)  #2 := BILINEAR
        self.image_user.set_from_pixbuf(pixbuf)

    def updateRecentFile(self):
        self.AddFavBtn()
        self.updateHistoryBox()

    def updateRecent(self):
        self.showRecentFile = ukuimenu_settings.get_boolean("show-recent-file")
        if not self.showRecentFile:
            return

        GLib.timeout_add( 100, self.updateRecentFile )

    def RecentFileChanged(self, settings = None, key = None, args = None):
        self.showRecentFile = ukuimenu_settings.get_boolean("show-recent-file")
        self.AddFavBtn()
        self.updateHistoryBox()

    def shutdownmenuPopup(self, widget):
        if True:      #1-左右，2-中，3-右
            mTree = Gtk.Menu()
            mTree.set_name("myGtkLabel")

            shutdown = Gtk.ImageMenuItem.new_with_mnemonic(_("Shutdown (_P)"))
            timingshutdown = Gtk.ImageMenuItem.new_with_mnemonic(_("Countdown Shutdown (_T)"))
            separatorMenuItem1 = Gtk.SeparatorMenuItem()
            separatorMenuItem1.set_visible(True)
            reboot = Gtk.ImageMenuItem.new_with_mnemonic(_("Reboot (_R)"))
            separatorMenuItem2 = Gtk.SeparatorMenuItem()
            separatorMenuItem2.set_visible(True)
            logout = Gtk.ImageMenuItem.new_with_mnemonic(_("Logout (_O)"))
            changeuser = Gtk.ImageMenuItem.new_with_mnemonic(_("Switch Users (_S)"))
            lock = Gtk.ImageMenuItem.new_with_mnemonic(_("Lock Screen (_L)"))
            mTree.append(lock)
            mTree.append(changeuser)
            mTree.append(logout)
            mTree.append(separatorMenuItem2)
            mTree.append(reboot)
            mTree.append(separatorMenuItem1)
            mTree.append(timingshutdown)
            mTree.append(shutdown)
            mTree.show_all()
            lock.connect("activate", self.lock, widget)
            changeuser.connect("activate", self.switchuser, widget)
            logout.connect("activate", self.logout, widget)
            reboot.connect("activate", self.reboot, widget)
            timingshutdown.connect("activate", self.showtimingshutdown, widget)
            shutdown.connect("activate", self.shutdown1, widget)
            self.ukuiMenuWin.stopHiding()
            mTree.connect("deactivate", self.set_state, widget)
            return mTree

    def set_state(self, menu, widget):
        self.viewport23.set_sensitive(True)

    def menu_right_click(self, widget, event):
        mTree = self.shutdownmenuPopup(widget)
        mTree.attach_to_widget(widget)
        mTree.popup(None, None, None, None, event.button, event.time)
        self.hovermenupopup = True
        return True # event has been handled

    def switchuser(self, menu, widget):
        system_bus = dbus.SystemBus()
        xdg_seat_path = os.environ['XDG_SEAT_PATH']
        obj = system_bus.get_object('org.freedesktop.DisplayManager', xdg_seat_path)
        interface = dbus.Interface(obj,'org.freedesktop.DisplayManager.Seat')
        interface.SwitchToGreeter()
        self.ukuiMenuWin.hide()
        Gdk.flush()

    def logout(self, menu, widget):
        os.system("ukui-session-save --logout-dialog &")
        self.ukuiMenuWin.hide()
        Gdk.flush()

    def reboot(self, menu, widget):
        session_bus = dbus.SessionBus()
        obj = session_bus.get_object('org.gnome.SessionManager','/org/gnome/SessionManager')
        interface = dbus.Interface(obj,'org.gnome.SessionManager')
        interface.RequestReboot()
        self.ukuiMenuWin.hide()
        Gdk.flush()

    def shutdown(self, menu, widget):
        os.system("ukui-session-save --shutdown-dialog &")
        self.ukuiMenuWin.hide()
        Gdk.flush()

    def showtimingshutdown(self, menu, widget):
        os.system("/usr/lib/ukui-menu/timing-shutdown.py &")

    def shutdown1(self, menu, widget):
        session_bus = dbus.SessionBus()
        obj = session_bus.get_object('org.gnome.SessionManager','/org/gnome/SessionManager')
        interface = dbus.Interface(obj,'org.gnome.SessionManager')
        interface.RequestShutdown()
        self.ukuiMenuWin.hide()
        Gdk.flush()

    def lock(self, menu, widget):
        if os.path.exists("/usr/bin/ukui-screensaver-command"):
            os.system("ukui-screensaver-command -l")
        else:
            os.system( "mate-screensaver-command -l" )
        self.ukuiMenuWin.hide()
        Gdk.flush()

    def onShowMenu(self):
        screenHeight = Gdk.Screen.height()
        heightPath = os.path.join(GLib.get_home_dir(), ".windowHeight")
        if (self.currentHisCount + len(self.favAppList) - 1) < 10:
            self.windowHeight = 505
            self.addedHeight = 0
            if os.path.exists(heightPath):
                os.remove(heightPath)
        else:
            self.addedHeight = 41*((self.currentHisCount + len(self.favAppList) - 1) - 10)
            self.windowHeight = 505 + self.addedHeight
            f = open(heightPath, 'w')
            f.write(str(self.windowHeight))
            f.close()
        if self.windowHeight > screenHeight - 60:
            self.addedHeight = screenHeight - 60 - 505
            self.windowHeight = screenHeight - 60
        self.content_holder.set_size_request(340, self.windowHeight)
        self.updateRecent()
        self.updateHeight()
        Gdk.flush()

    def updateHeight(self):
        self.layout_left.move(self.viewport_entry, 103, 465+self.addedHeight)
        self.layout_left.move(self.viewport18, 4, 465+self.addedHeight)
        self.layout_left.move(self.viewport23, 74, 465+self.addedHeight)
        self.viewport1.set_size_request(236, 455+self.addedHeight)
        self.vp_left_container.set_size_request(227, 450+self.addedHeight)
        self.viewport2.set_size_request(236, 455+self.addedHeight)
        self.vp_right_container.set_size_request(227, 450+self.addedHeight)

    def do_plugin(self):
        self.Todos()

    def Todos(self):
        self.searchEntry.connect( "changed", self.Filter )
        self.searchEntry.connect( "activate", self.Search )

        self.buildButtonList()

    def buildButtonList(self):
        if self.buildingButtonList:
            self.stopBuildingButtonList = True
            GLib.timeout_add( 100, self.buildButtonList )
            return
        self.stopBuildingButtonList = False
        self.updateBoxes(False)

    def changeShowCategoryIcons(self, settings, key, args):
        categoryIconSize = self.iconSize

        for child in self.categoriesBox:
            child.setIconSize( categoryIconSize )

    def menuChanged(self, x, y):
        # wait some miliseconds because there a multiple events send at the same time and we don't want to rebuild the menu for each
        if self.menuChangedTimer:
            GLib.source_remove( self.menuChangedTimer )

        self.menuChangedTimer = GLib.timeout_add( 100, self.updateBoxes, True )

    def updateBoxes(self, menu_has_changed):
        self.categorybutton_list = []
        self.menuHasChanged = menu_has_changed

        self.loadMenuFiles()
        self.all_application_list = self.build_application_list()
        self.RecentFileChanged()

        if self.rebuildLock:
            return

        for child in self.categoriesBox.get_children():
            if child == self.expanderApp or child == self.expanderSystem:
                continue
            self.categoriesBox.remove( child )

        self.rebuildLock = True
        self.menuChangedTimer = None

        category_icon_size = self.iconSize
        sorted_category_list = []
        self.list = self.buildCategoryList()

        for item in self.list:
            try:
                if self.showCategoryMenu:
                    item["filter"] = item["name"]
                    item["button"] = CategoryButton( item["icon"], category_icon_size, [item["name"]], item["filter"])
                else:
                    if item["name"] == _("Education") or item["name"] == _("Graphics") or item["name"] == _("Internet") or item["name"] == _("Programming") or item["name"] == _("Sound & Video") or item["name"] == _("System Tools") or item["name"] == _("Universal Access") or item["name"] == _("Preferences") or item["name"] == _("Administration") or item["name"] == _("Other"):
                        continue
                    elif item["name"] == _("Accessories") or item["name"] == _("Office") or item["name"] == _("Games") or item["name"] == _("Android"):
                        item["button"] = CategoryButton( "folder", category_icon_size, [item["name"]], item["name"] )
                    else:
                        item["button"] = CategoryButton( "folder", category_icon_size, [item["name"]], item["name"] )
                item["button"].connect( "button-press-event", self.FilterAndClear, item["filter"] )
                item["button"].connect( "focus-in-event", self.scrollItemIntoView )
                item["button"].connect( "enter", self.showGoNext )
                item["button"].connect( "leave", self.hideGoNext )
                item["button"].show()
                item["button"].set_name("ButtonApp")
                self.categorybutton_list.append(item["button"])

                sorted_category_list.append( ( item["name"], item["button"] ) )
            except Exception as e:
                print (e)

        if not self.showCategoryMenu:
            sorted_category_list.sort()

        for child in self.categoriesApp.get_children():
            self.categoriesApp.remove( child )
        for child in self.categoriesSystem.get_children():
            self.categoriesSystem.remove( child )

        currentApp = False
        currentSystem = False
        for item in sorted_category_list:
            try:
                if self.showCategoryMenu:
                    self.categoriesBox.set_spacing(1)
                    if item[0] == _("Applications"):
                        self.labelExpanderApp.set_label(_("Applications"))
                        currentApp = True
                        currentSystem = False
                        continue
                    if item[0] == _("System"):
                        self.labelExpanderSystem.set_label(_("System"))
                        currentSystem = True
                        currentApp = False
                        continue
                    if currentApp:
                        self.categoriesApp.pack_start(item[1], False, False, 0)
                    if currentSystem:
                        self.categoriesSystem.pack_start(item[1], False, False, 0)
                else:
                    self.expanderApp.hide()
                    self.expanderSystem.hide()
                    self.categoriesBox.set_spacing(2)
                    self.categoriesBox.pack_start( item[1], False, False, 0 )
            except Exception as e:
                print (e)

        if self.showCategoryMenu:
            new_application_list = self.all_application_list
        else:
            cat = Category(self.all_application_list)
            new_application_list = cat.application_list()
        added_application_list = []
        removed_application_list = []
        #处理分类中有程序安装或删除时的显示更新
        if not self.applicationList:
            added_application_list = new_application_list
        else:
            for item in new_application_list:
                found = False
                for item2 in self.applicationList:
                    if item['entry'].get_desktop_file_path() is None or item2['entry'].get_desktop_file_path() is None:
                        continue
                    if item['entry'].get_desktop_file_path().decode('utf-8') == item2['entry'].get_desktop_file_path().decode('utf-8'):
                        found = True
                        break
                if not found:
                    added_application_list.append(item)

            key = 0
            for item in self.applicationList:
                found = False
                for item2 in new_application_list:
                    if item['entry'].get_desktop_file_path() is None or item2['entry'].get_desktop_file_path() is None:
                        continue
                    if item['entry'].get_desktop_file_path().decode('utf-8') == item2['entry'].get_desktop_file_path().decode('utf-8'):
                        found = True
                        break
                if not found:
                    removed_application_list.append(key)
                else:
                    key += 1

        for key in removed_application_list:
            self.applicationList[key]['button'].destroy()
            del self.applicationList[key]

        sorted_application_list = []
        for child in self.applicationsBox.get_children():
            self.applicationsBox.remove( child )

        for item in new_application_list:
            found = False
            for item1 in added_application_list:
                if item == item1:
                    button = self.build_application_launcher(item, menu_has_changed)
                    if button:
                        item["button"] = button
                        item["button"].connect( "focus-in-event", self.scrollItemIntoView )                                    #feng
                        if item["button"].appNoDisplay == True:
                            continue
                        sorted_application_list.append( ( item["button"].appName, item["button"] ) )
                        self.applicationList.append( item )
                        found = True
                        continue
            if not found:
                button = self.build_application_launcher(item, False)
                if button:
                    item["button"] = button
                    item["button"].connect( "focus-in-event", self.scrollItemIntoView )                                    #feng
                    if item["button"].appNoDisplay == True:
                        continue
                    sorted_application_list.append( ( item["button"].appName, item["button"] ) )
                    self.applicationList.append( item )

        if not self.showCategoryMenu:
            sorted_application_list.sort()
        launcherNames = [] # Keep track of launcher names so we don't add them twice in the list.
        for item in sorted_application_list:
        #for item in new_application_list:
            launcherName = item[0]
            button = item[1]
            self.applicationsBox.pack_start( button, False, False, 0 )
            button.show()
            if launcherName in launcherNames:
                button.hide()
            else:
                launcherNames.append(launcherName)
        self.expanderRecent.hide()

        self.uncategorized_list = []
        for item in self.all_application_list:
            found = False
            for item2 in self.applicationList:
                if item['entry'].get_desktop_file_path() is None or item2['entry'].get_desktop_file_path() is None:
                    continue
                if item['entry'].get_desktop_file_path().decode('utf-8') == item2['entry'].get_desktop_file_path().decode('utf-8') or os.path.basename(item['entry'].get_desktop_file_path().decode('utf-8')) == os.path.basename(item2['entry'].get_desktop_file_path().decode('utf-8')):
                    found = True
                    break
            if not found:
                self.uncategorized_list.append(item)

        new_uncategoried_list = self.uncategorized_list
        added_uncategoried_list = []
        removed_uncategoried_list = []
        #处理未分类中有程序的删除或安装时的更新显示
        if not self.uncategoriedList:
            added_uncategoried_list = new_uncategoried_list
        else:
            for item in new_uncategoried_list:
                found = False
                for item2 in self.uncategoriedList:
                    if item['entry'].get_desktop_file_path() is None or item2['entry'].get_desktop_file_path() is None:
                        continue
                    if item['entry'].get_desktop_file_path().decode('utf-8') == item2['entry'].get_desktop_file_path().decode('utf-8'):
                        found = True
                        break
                if not found:
                    added_uncategoried_list.append(item)

            key = 0
            for item in self.uncategoriedList:
                found = False
                for item2 in new_uncategoried_list:
                    if item['entry'].get_desktop_file_path() is None or item2['entry'].get_desktop_file_path() is None:
                        continue
                    if item['entry'].get_desktop_file_path().decode('utf-8') == item2['entry'].get_desktop_file_path().decode('utf-8'):
                        found = True
                        break
                if not found:
                    removed_uncategoried_list.append(key)
                else:
                    key += 1

        for key in removed_uncategoried_list:
            self.uncategoriedList[key]['button'].destroy()
            del self.uncategoriedList[key]

        sorted_uncategoried_list = []
        for item in self.uncategoriedList:
            sorted_uncategoried_list.append( ( item["button"].appName, item["button"] ) )

        for item in added_uncategoried_list:
            button = self.build_application_launcher(item, menu_has_changed)
            if button:
                item["button"] = button
                item["button"].connect( "focus-in-event", self.scrollItemIntoView )                                    #feng
                if item["button"].appNoDisplay == True:
                    continue
                sorted_uncategoried_list.append( ( item["button"].appName, item["button"] ) )
                self.uncategoriedList.append( item )

        if not self.showCategoryMenu:
            sorted_uncategoried_list.sort()
        launcherNames = [] # Keep track of launcher names so we don't add them twice in the list.
        for item in sorted_uncategoried_list:
            launcherName = item[0]
            button = item[1]
            self.categoriesBox.pack_start( button, False, False, 0 )
            button.show()
            if launcherName in launcherNames:
                button.hide()
            else:
                launcherNames.append(launcherName)

        self.rebuildLock = False

    def build_application_launcher(self, application, menu_has_changed):
        if application['entry'].get_desktop_file_path() is None:
            return None
        button = MenuApplicationLauncher( application['entry'].get_desktop_file_path().decode('utf-8'),
                                          self.iconSize, application["category"],
                                          True, 28,
                                          highlight = (True and menu_has_changed) )
        button.set_name("ButtonApp")
        if button.appName == "Logout" or button.appName == "Shutdown" or button.appName == "Reboot":
            return None
        if button.appExec:
            button.set_tooltip_text( button.getTooltip() )
            button.connect( "clicked", lambda w: self.ukuiMenuWin.hide() )
            button.connect( "button-press-event", self.menuPopup )
            button.desktop_file_path = application['entry'].get_desktop_file_path().decode('utf-8')
            if button.appName == "Eclipse":
                (a, b) = locale.getdefaultlocale()
                if a == "zh_CN":
                    button.set_tooltip_text( " Eclipse\n 描述：Eclipse集成开发环境" )
            if button.appName == "Qt Creator":
                (a, b) = locale.getdefaultlocale()
                if a == "zh_CN":
                    button.set_tooltip_text( " Qt Creator\n 描述：Qt Creator集成开发环境" )
            return button
        else:
            button.destroy()
            return None

    def build_application_list(self):
        application_list = []

        def find_applications_recursively(app_list, directory, catName):
            for item in directory.get_contents():
                if item.get_type() == ukuimenu.TYPE_ENTRY:
                    if entry.get_name() is None:
                        continue
                    app_list.append( { 'entry': item,
                                       'category': entry.get_name().decode('utf-8') } )
                elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
                    find_applications_recursively(app_list, item, catName)

        for menu in self.menuFiles:
            root = menu.directory
            for entry in root.get_contents():
                if entry.get_type() == ukuimenu.TYPE_DIRECTORY and len(entry.get_contents()):
                    for item in entry.get_contents():
                        if item.get_type() == ukuimenu.TYPE_DIRECTORY:
                            if entry.get_name() is None:
                                continue
                            find_applications_recursively(application_list, item, entry.get_name().decode('utf-8'))
                        elif item.get_type() == ukuimenu.TYPE_ENTRY:
                            if entry.get_name() is None:
                                continue
                            application_list.append({ 'entry': item,
                                                      'category': entry.get_name().decode('utf-8') })
        return application_list

    def loadMenuFiles(self):
        self.menuFiles = []
        for mainitems in [ "ukui-applications.menu", "ukui-settings.menu" ]:
            mymenu = Menu( mainitems )
            self.menuFiles.append( mymenu )
            mymenu.tree.add_monitor( self.menuChanged, None )

    def buildCategoryList(self):
        newCategoryList = [ ]

        num = 1

        for menu in self.menuFiles:
            if self.showCategoryMenu:
                name = menu.tree.get_root_directory().get_name()
                icon = menu.tree.get_root_directory().get_icon()
                if name is None:
                    continue
                else:
                    name = name.decode('utf-8')
                if icon is None:
                    icon = "folder"
                else: 
                    icon = icon.decode('utf-8')
                if len(menu.directory.get_contents()):
                    newCategoryList.append( { "name": name, "icon": icon, "tooltip": name, "filter": name, "index": num } )
                    if name == _("Applications"):
                        self.expanderApp.show()
                    if name == _("System"):
                        self.expanderSystem.show()
                else:
                    if name == _("Applications"):
                        self.expanderApp.hide()
                    if name == _("System"):
                        self.expanderSystem.hide()

            for child in menu.directory.get_contents():
                if child.get_type() == ukuimenu.TYPE_DIRECTORY:
                    name = child.get_name()
                    if name is None:
                        continue
                    else:
                        name = name.decode('utf-8')
                    icon = child.get_icon()
                    if icon is None:
                        icon = "folder"
                    else:
                        icon = icon.decode('utf-8')
                    newCategoryList.append( { "name": name,  "icon": icon, "tooltip": name, "filter": name, "index": num } )
            num += 1
        return newCategoryList

    def destroy( self ):
        self.applicationsBox.destroy()
        self.categoriesBox.destroy()
        self.searchResultBox.destroy()
        self.searchEntry.destroy()

    def FilterAndClear( self, widget, event, category = None ):
        for widget1 in self.categorybutton_list:
            if widget1 == widget:
                continue
            else:
                widget1.aState = False
                widget1.rightbox.hide()
        if widget.aState:
            self.layout_applications_right.hide()
            self.content_holder.set_size_request(340, self.windowHeight)
            widget.aState = False
            widget.set_name("ButtonApp")
        else:
            self.layout_applications_right.show()
            self.content_holder.set_size_request(575, self.windowHeight)
            widget.aState = True
            widget.grab_focus()
            for widget1 in self.categorybutton_list:
                if widget1 != widget:
                    widget1.aState = False
                    widget1.rightbox.hide()
            widget.set_name("ButtonAppHover")
            if self.ButtonHoverWidgetHistory and self.ButtonHoverWidgetHistory != widget:
                self.ButtonHoverWidgetHistory.set_name("ButtonApp")
            self.ButtonHoverWidgetHistory = widget
        self.Filter( widget, category)

    def Filter(self, widget, category = None):
        self.filterTimer = None
        sorted_all_application_list = []

        if len(self.all_app_list) > 0:
            if self.menuHasChanged:
                for item in self.all_application_list:
                    button = self.build_application_launcher(item, False)
                    if button:
                        item["button"] = button
                        item["button"].connect( "focus-in-event", self.scrollItemIntoView )
                        sorted_all_application_list.append( ( item['button'].appName, item['button'] ) )
                if not self.showCategoryMenu:
                    sorted_all_application_list.sort()
                self.all_app_list = sorted_all_application_list
        else:
            for item in self.all_application_list:
                button = self.build_application_launcher(item, False)
                if button:
                    item["button"] = button
                    item["button"].connect( "focus-in-event", self.scrollItemIntoView )
                    sorted_all_application_list.append( ( item['button'].appName, item['button'] ) )
            if not self.showCategoryMenu:
                sorted_all_application_list.sort()
            self.all_app_list = sorted_all_application_list


        if  widget == self.searchEntry:
            searchText = self.searchEntry.get_text()
            if searchText == _("Search local program"):
                return
            self.layout_applications_right.hide()
            if False:
                widget.set_text( "" )
            else:
                text = widget.get_text()
                if self.lastActiveTab != 2:
                    self.changeTab( 2, clear = False )
                text = widget.get_text()
                if text == "":
                    self.changeTab(1)
                showns = False # Are any app shown?
                self.shownList = []
                for child in self.searchResultBox:
                    self.searchResultBox.remove(child)
                num = 1
                for num in range(1, 4):
                    for i in self.all_app_list:
                        shown = i[1].filterText( text, num )
                        if (shown):
                            dupe = False
                            for item in self.shownList:
                                if i[1].desktopFile == item.desktopFile:
                                    dupe = True
                            if not dupe:
                                self.shownList.append(i[1])
                                self.searchResultBox.pack_start(i[1], False, False, 0)
                                i[1].show()
                if len(self.shownList) == 0:
                    self.searchResultBox.pack_start(self.label3, True, True, 0)
                    self.label3.show()
                self.activeFilter = (0, text, widget)
        else:
            #print "CATFILTER"
            self.activeFilter = (1, category, widget)
            if category == "":
                listedDesktopFiles = []
                for i in self.applicationsBox.get_children():
                    if not i.desktop_file_path in listedDesktopFiles:
                        listedDesktopFiles.append( i.desktop_file_path )
                        i.show_all()
                    else:
                        i.hide()
            else:
                for i in self.applicationsBox.get_children():
                    if i == self.expanderRecent:
                        self.expanderRecent.hide()
                        continue
                    i.filterCategory( category )

            if not self.showCategoryMenu:
                self.expanderApp.hide()
                self.expanderSystem.hide()
                for i in self.categoriesBox.get_children():
                    if i == self.expanderApp or i == self.expanderSystem:
                        continue
                    i.released()
                    i.set_relief( Gtk.ReliefStyle.NONE )
                
        self.applicationsScrolledWindow.get_vadjustment().set_value( 0 )

    def on_button_showall_clicked(self, widget):
        self.changeTab(1)

    def on_button_back_clicked(self, widget):
        self.changeTab(0)

    def menuPopup(self, widget, event):
        button = MenuApplicationLauncher(widget.desktopFile, 32, "", True, highlight=(False))
        addedDesktop = False
        try:
            if (os.path.exists(GLib.get_user_special_dir(GLib.USER_DIRECTORY_DESKTOP) + "/" + os.path.basename(widget.desktopFile))):
                addedDesktop = True
        except Exception as detail:
            print (detail)

        text = ""
        if event.button == 3:
            if (os.path.exists(GLib.get_home_dir() + "/.applet")):
                f = open(GLib.get_home_dir() + "/.applet")
                text = f.read()
                f.close()

            mTree = Gtk.Menu()
            mTree.set_name("myGtkLabel")
            launchMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Open(_O)"))
            image = Gtk.Image.new_from_icon_name("gtk-execute", Gtk.IconSize.MENU)
            launchMenuItem.set_image(image)

            separator1 = Gtk.SeparatorMenuItem()

            desktopMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Add to desktop(_A)"))
            image = Gtk.Image.new_from_icon_name("gtk-add", Gtk.IconSize.MENU)
            desktopMenuItem.set_image(image)
            if addedDesktop:
                desktopMenuItem.set_sensitive(False)

            panelMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Lock to panel(_L)"))
            #image = Gtk.Image.new_from_icon_name("gtk-lock", Gtk.IconSize.MENU)
            #panelMenuItem.set_image(image)

            separator2 = Gtk.SeparatorMenuItem()

            favoriteMenuItem = Gtk.CheckMenuItem.new_with_mnemonic(_("Add to startup menu"))
            #image = Gtk.Image.new_from_icon_name("gtk-lock", Gtk.IconSize.MENU)
            #favoriteMenuItem.set_image(image)

            separator3 = Gtk.SeparatorMenuItem()

            uninstallMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Uninstall(_U)"))
            image = Gtk.Image.new_from_icon_name("gtk-delete", Gtk.IconSize.MENU)
            uninstallMenuItem.set_image(image)

            deleteMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Remove from list(_R)"))
            image = Gtk.Image.new_from_icon_name("gtk-remove", Gtk.IconSize.MENU)
            deleteMenuItem.set_image(image)

            alldelete = Gtk.ImageMenuItem(_("Remove all"))
            #image = Gtk.Image.new_from_icon_name("gtk-lock", Gtk.IconSize.MENU)
            #alldeleteMenuItem.set_image(image)

            separator4 = Gtk.SeparatorMenuItem()

            propsMenuItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Property(_P)"))
            image = Gtk.Image.new_from_icon_name("gtk-properties", Gtk.IconSize.MENU)
            propsMenuItem.set_image(image)

            mTree.append(launchMenuItem)
            mTree.append(separator1)
            if self.de == "ukui":
                mTree.append(desktopMenuItem)
                mTree.append(panelMenuItem)
                if button.appName in text:
                    panelMenuItem.set_sensitive(False)
                mTree.append(separator2)

            mTree.append(favoriteMenuItem)
            if widget.isRecentApp:
                mTree.append(deleteMenuItem)
                mTree.append(separator4)
                mTree.append(alldelete)
                deleteMenuItem.connect("activate", self.delete_from_menu, widget)
                alldelete.connect("activate", self.clearAllRecent)
            else:
                if os.path.exists("/usr/bin/ubuntu-kylin-software-center"):
                    mTree.append(separator3)
                    mTree.append(uninstallMenuItem)
                    uninstallMenuItem.connect ( "activate", self.onUninstallApp, widget )
                    mTree.append(separator4)

            mTree.append(propsMenuItem)
            mTree.show_all()
            launchMenuItem.connect( "activate", self.onLaunchApp, widget )
            desktopMenuItem.connect("activate", self.add_to_desktop, widget)
            panelMenuItem.connect("activate", self.add_to_panel, widget)
            if self.isLocationInFavorites( widget.desktopFile ):
                if len(self.favorites) < 2:
                    favoriteMenuItem.set_sensitive(False)
                favoriteMenuItem.set_active( True )
                favoriteMenuItem.connect( "toggled", self.onRemoveFromFavorites, widget )
            else:
                favoriteMenuItem.set_active( False )
                favoriteMenuItem.connect( "toggled", self.onAddToFavorites, widget )
            propsMenuItem.connect( "activate", self.onPropsApp, widget)
            mTree.connect("deactivate", self.set_item_state, widget)

            widget.set_name("ButtonAppHover")
            self.ukuiMenuWin.stopHiding()
            widget.remove(widget.Align1)
            widget.Align1.remove(widget.HBox1)
            widget.viewport.show()
            widget.viewport.add(widget.HBox1)
            widget.add(widget.viewport)
            mTree.attach_to_widget(widget)
            mTree.popup(None, None, None, None, event.button, event.time)

    def set_item_state(self, menu, widget):
        button = MenuApplicationLauncher(widget.desktopFile, 32, "", True, highlight=(False))
        widget.emit("leave")
        widget.set_name("ButtonApp")
        widget.viewport.remove(widget.HBox1)
        widget.remove(widget.viewport)
        widget.Align1.add(widget.HBox1)
        widget.add(widget.Align1)

    def add_to_desktop(self, widget, desktopEntry):
        try:
            os.system("cp \"%s\" \"%s/\"" % (desktopEntry.desktopFile, GLib.get_user_special_dir(GLib.USER_DIRECTORY_DESKTOP)))
            os.system("chmod a+rx %s/*.desktop" % GLib.get_user_special_dir(GLib.USER_DIRECTORY_DESKTOP))
        except Exception as detail:
            print (detail)

    def add_to_panel(self, widget, desktopEntry):
        self.get_panel()
        i = 0
        panel_schema = Gio.Settings.new("org.ukui.panel")
        applet_list = panel_schema.get_strv("object-id-list")

        while True:
            test_obj = "object_%d" % (i)
            if test_obj in applet_list:
                i += 1
            else:
                break

        path = "/org/ukui/panel/objects/%s/" % (test_obj)
        new_schema = Gio.Settings.new_with_path("org.ukui.panel.object", path)
        new_schema.set_string("launcher-location", desktopEntry.desktopFile)
        new_schema.set_string("object-type", "launcher")
        new_schema.set_string("toplevel-id", self.panel)
        new_schema.set_int("position", 50)
        applet_list.append(test_obj)
        panel_schema.set_strv("object-id-list", applet_list)

    def get_panel(self):
        panelsettings = Gio.Settings.new("org.ukui.panel")
        applet_list = panelsettings.get_strv("object-id-list")
        for applet in applet_list:
            object_schema = Gio.Settings.new_with_path("org.ukui.panel.object", "/org/ukui/panel/objects/%s/" % (applet))
            keys = object_schema.list_keys()
            if "applet-iid" in keys:
                iid = object_schema.get_string("applet-iid")
                if iid is not None and iid.find("UkuiMenu") != -1:
                    self.panel = object_schema.get_string("toplevel-id")
                    self.panel_position = object_schema.get_int("position") + 1

    def onLaunchApp( self, menu, widget ):
        widget.execute()
        self.ukuiMenuWin.hide()

    def onFavoritesRemove( self, menu, widget ):
        self.favoritesRemove( widget.position )

    def favoritesRemove( self, position ):
        tmp = self.favorites[ position ]
        self.favorites.remove( self.favorites[ position ] )
        tmp.destroy()
        for i in range( position, len( self.favorites ) ):
            self.favorites[ i ].position = i
            self.favoritesBox.remove( self.favorites[ i ] )
            self.favoritesPositionOnGrid( self.favorites[ i ] )
        self.favoritesSave()
        self.favoritesBox.resize( self.favoritesGetNumRows(), self.favCols )
        self.updateHistoryBox()

    def onAddToFavorites(self, menu, widget):
        self.favoritesAdd( self.favoritesBuildLauncher( widget.desktopFile ) )
        self.updateHistoryBox()

    def onRemoveFromFavorites(self, menu, widget):
        self.favoritesRemoveLocation(widget.desktopFile)
        self.updateHistoryBox()

    def favoritesRemoveLocation(self, location):
        for fav in self.favorites:
            if fav.type == "location" and fav.desktopFile == location:
                self.favoritesRemove(fav.position)
        self.updateHistoryBox()

    def favoritesAdd(self, favButton, position = -1):
        if favButton:
            favButton.set_name("ButtonApp")
            favButton.position = len(self.favorites)
            self.favorites.append( favButton )
            for fav in self.favorites:
                self.favoritesBox.remove(fav)
                self.favoritesPositionOnGrid(fav)

            favButton.connect("drag-data-received", self.onFavButtonDragReorder)
            favButton.drag_dest_set(Gtk.DestDefaults.MOTION | Gtk.DestDefaults.HIGHLIGHT | Gtk.DestDefaults.DROP, self.fromFav, Gdk.DragAction.COPY)
            favButton.connect("drag-data-get", self.onFavButtonDragReorderGet)
            favButton.drag_source_set(Gdk.ModifierType.BUTTON1_MASK, self.toFav, Gdk.DragAction.COPY)

            if position >= 0:
                self.favoritesReorder(favButton.position, position)

            self.favoritesSave()
            self.favoritesBox.resize(self.favoritesGetNumRows(), self.favCols)
        self.updateHistoryBox()

    def favoritesGetNumRows(self):
        rows = 0
        col = 0
        for fav in self.favorites:
            if  (fav.type == "separator" or fav.type == "space") and col != 0:
                rows += 1
                col = 0
            col += 1
            if  fav.type == "separator" or fav.type == "space":
                rows += 1
                col = 0
            if col >= self.favCols:
                rows += 1
                col = 0
        return rows

    def onPropsApp(self, menu, widget):
        button = MenuApplicationLauncher(widget.desktopFile, 32, "", True, highlight=(False))
        self.builder1 = Gtk.Builder()
        self.builder1.add_from_file( os.path.join( '/', 'usr', 'share', 'ukui-menu',  'plugins', 'property.glade' ) )
        dialog = self.builder1.get_object("dialog1")
        #dialog.set_name("myGtkLabel")
        self.entry_type = self.builder1.get_object("entry_type")
        self.entry_name = self.builder1.get_object("entry_name")
        self.entry_command = self.builder1.get_object("entry_command")
        self.entry_comment = self.builder1.get_object("entry_comment")
        label_type = self.builder1.get_object("label_type")
        label_type.set_text(_("Type:"))
        label_name = self.builder1.get_object("label_name")
        label_name.set_text(_("Name:"))
        label_command = self.builder1.get_object("label_command")
        label_command.set_text(_("Command:"))
        label_comment = self.builder1.get_object("label_comment")
        label_comment.set_text(_("Note:"))
        self.image_app = self.builder1.get_object("image2")
        icon = button.appIconName
        if os.path.isfile(icon):
            pixbuf = Pixbuf.new_from_file(icon)
            pixbuf = pixbuf.scale_simple(60, 60, 2)
            self.image_app.set_from_pixbuf(pixbuf)
        else:
            self.image_app.set_from_icon_name(icon, Gtk.IconSize.DND)
            self.image_app.set_pixel_size(60)

        self.button_close = self.builder1.get_object("button_close")
        self.button_close.connect("clicked", self.close_dia, dialog)

        if button.appType == "Application":
            self.entry_type.set_text(_("Application"))
        if button.appType == "Application in Terminal":
            self.entry_type.set_text(_("Application in Terminal"))
        self.entry_name.set_text(button.appName)
        self.entry_command.set_text(button.appExec)
        self.entry_comment.set_text(button.appComment)
        if button.appName == "Eclipse":
            (a, b) = locale.getdefaultlocale()
            if a == "zh_CN":
                self.entry_comment.set_text("Eclipse集成开发环境")
        if button.appName == "Qt Creator":
            (a, b) = locale.getdefaultlocale()
            if a == "zh_CN":
                self.entry_comment.set_text("Qt Creator集成开发环境")

        dialog.set_title(button.appName + _("Property"))
        dialog.show()

        self.ukuiMenuWin.hide()
        Gdk.flush()

    def close_dia(self, widget, dialog):
        dialog.hide()

    def delete_from_menu(self, menu, widget):
        recentApp = {}
        try:
             delappname = widget.appName.lower()
             if os.path.exists(GLib.get_home_dir() + "/.recentAppLog"):
                 f = open(GLib.get_home_dir() + "/.recentAppLog", 'r')
                 lines = f.readlines()
                 f.close()
                 for line in lines:
                     word = line.strip().split(':')
                     if word[0] == delappname:
                         continue
                     recentApp[word[0]] = int(word[len(word) - 1])
                 f = open(GLib.get_home_dir() + "/.recentAppLog", 'w')
             else:
                 f = open(GLib.get_home_dir() + "/.recentAppLog", 'w')

             srApp = sorted(iter(recentApp.items()), key = operator.itemgetter(1), reverse=True)
             for i in range(len(srApp)):
                 f.write(srApp[i][0] + ":" + str(srApp[i][1]) + "\n")
             f.close()
             self.updateHistoryBox()
        except Exception as detail:
            print (detail)

    def changeTab( self, tabNum, clear = True ):
        notebook = self.builder.get_object( "notebook_left" )
        if tabNum == 0:
            self.favappbutton.grab_focus()
            self.button_user.grab_focus()
            if self.ButtonHoverWidgetHistory:
                self.ButtonHoverWidgetHistory.set_name("ButtonApp")
            self.viewportfav.set_name("ViewportButton")
            self.viewportallapp.set_name("ViewportNull")
            self.viewport_entry.set_name("Viewport")
            self.searchEntry.set_name("Entry")
            self.favappbutton.set_sensitive(False)
            self.allappbutton.set_sensitive(True)
            self.content_holder.set_size_request(340, self.windowHeight)
            self.layout_applications_right.hide()
            for widget1 in self.categorybutton_list:
                widget1.aState = False
                widget1.rightbox.hide()
            notebook.set_current_page( 0 )
        elif tabNum == 1:
            self.viewportfav.set_name("ViewportNull")
            self.viewportallapp.set_name("ViewportButton")
            self.viewport_entry.set_name("ViewportEntry")
            self.searchEntry.set_name("EntryActive")
            self.favappbutton.set_sensitive(True)
            self.allappbutton.set_sensitive(False)
            self.searchEntry.grab_focus()
            notebook.set_current_page( 1 )
        elif tabNum == 2:
            self.content_holder.set_size_request(340, self.windowHeight)
            #self.updateHeight()
            self.layout_applications_right.hide()
            for widget1 in self.categorybutton_list:
                widget1.aState = False
                widget1.rightbox.hide()
            notebook.set_current_page( 2 )
        self.lastActiveTab = tabNum

    def Search( self, widget ):
        text = self.searchEntry.get_text().strip()
        if text != "":
            self.shownList[0].grab_focus()

    def onUninstallApp( self, menu, widget ):
        widget.uninstall()
        self.ukuiMenuWin.hide()

    def onRemoveFromStartup( self, menu, widget ):
        widget.removeFromStartup()

    def ReceiveCallback( self, widget, context, x, y, selection, targetType, time ):
        if targetType == self.TARGET_TYPE_TEXT:
            for uri in selection.get_uris():
                self.favoritesAdd( self.favoritesBuildLauncher( uri ) )

    def showGoNext( self, widget ):
        widget.rightbox.show_all()

    def hideGoNext( self, widget ):
        if not widget.aState:
            widget.rightbox.hide()

    def onRecentFileRemove(self, menu, Data):
        uri, AppName, appname = Data
        hasapp = False

        if AppName == "Kylin Video":
            fileString = ""
            cf = UkuiConfigParser()
            path = os.path.join(GLib.get_user_config_dir(), 'kylin-video/kylin-video.ini')
            if os.path.exists(path):
                cf.read(path)
                recentFiles = cf.get("history", "recents")
                if recentFiles.endswith(","):
                    recentFiles = recentFiles[:-1]
                recentFiles = recentFiles.split(", ")
                for recfile in recentFiles:
                    if recfile == uri:
                        continue
                    fileString = fileString + recfile + ", "
                fileString = fileString[0:len(fileString)-2]
                cf.set("history", "recents", fileString)
                if fileString == "":
                    hasapp = False
                else:
                    hasapp = True
                fd = open(path, 'w')
                cf.write(fd)
                fd.close()
        elif AppName == "Qt Creator":
            uri = uri.encode("unicode_escape").decode('utf-8')
            uri = uri.replace('\\u', '\\x')
            fileString = ""
            cf = UkuiConfigParser()
            path = os.path.join(GLib.get_user_config_dir(), 'QtProject/QtCreator.ini')
            if os.path.exists(path):
                cf.read(path)
                recentFiles = cf.get("RecentFiles", "Files")
                if recentFiles == "@Invalid()":
                    hasapp = False
                    return
                if recentFiles.endswith(","):
                    recentFiles = recentFiles[:-1]
                recentFiles = recentFiles.split(", ")
                for recfile in recentFiles:
                    if recfile == uri:
                        continue
                    fileString = fileString + recfile + ", "
                fileString = fileString[0:len(fileString)-2]
                cf.set("RecentFiles", "Files", fileString)
                if fileString == "":
                    hasapp = False
                else:
                    hasapp = True
                fd = open(path, 'w')
                cf.write(fd)
                fd.close()
        else:
            RecManagerInstance.remove_item(uri)
            RecentInfo = RecManagerInstance.get_items()
            for item in RecentInfo:
                if AppName == "WPS Writer":
                    if item.get_mime_type() in WPS_W:
                        hasapp = True
                        break
                elif AppName == "WPS Presentation":
                    if item.get_mime_type() in WPS_P:
                        hasapp = True
                        break
                elif AppName == "WPS Spreadsheets":
                    if item.get_mime_type() in WPS_S:
                        hasapp = True
                        break
                elif item.has_application(AppName):
                    hasapp = True
                    break

        self.updateRecent()

        if hasapp:
            self.startsToShow(appname)
        else:
            self.expanderRecent.hide()
            self.layout_applications_right.hide()
            self.content_holder.set_size_request(340, self.windowHeight)

    def openDir(self, menu, FileData):
        Data, FileName = FileData
        dirname = os.path.dirname(FileName)
        if os.path.exists(FileName):
            pass
        else:
            msg = _("\nFile not exists, it was moved or deleted!\n\nIt will be automatically removed from the history list!\n")
            md = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING, Gtk.ButtonsType.OK, msg)
            md.run()
            md.destroy()
            self.onRecentFileRemove(menu, Data)
            return
        os.system("peony %s" % dirname)

    def copyFile(self, menu, FileData):
        Data, FileName = FileData
        if os.path.exists(FileName):
            pass
        else:
            msg = _("\nFile not exists, it was moved or deleted!\n\nIt will be automatically removed from the history list!\n")
            md = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING, Gtk.ButtonsType.OK, msg)
            md.run()
            md.destroy()
            self.onRecentFileRemove(menu, Data)
            return
        dstFileName = GLib.get_user_special_dir(GLib.USER_DIRECTORY_DESKTOP) + "/" + os.path.basename(FileName) 

        if os.path.exists(dstFileName):
            msg = _("\nThe file already exists, are you sure to cover it?\n\n")
            md = Gtk.MessageDialog(None, 0, Gtk.MessageType.QUESTION, Gtk.ButtonsType.YES_NO, msg)
            response = md.run()
            if response == Gtk.ResponseType.YES:
                if " " in FileName:
                    FileName = FileName.replace(" ", "\ ")
                if "&" in FileName:
                    FileName = FileName.replace("&", "\&")
                os.system("cp %s %s/" % (FileName, GLib.get_user_special_dir(GLib.USER_DIRECTORY_DESKTOP)))
            elif response == Gtk.ResponseType.NO:
                pass
            md.destroy()
            return
        if " " in FileName:
            FileName = FileName.replace(" ", "\ ")
        if "&" in FileName:
            FileName = FileName.replace("&", "\&")
        os.system("cp %s %s/" % (FileName, GLib.get_user_special_dir(GLib.USER_DIRECTORY_DESKTOP)))

    def propertyFile(self, menu, FileData):
        Data, FileName = FileData
        if os.path.exists(FileName):
            pass
        else:
            msg = _("\nFile not exists, it was moved or deleted!\n\nIt will be automatically removed from the history list!\n")
            md = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING, Gtk.ButtonsType.OK, msg)
            md.run()
            md.destroy()
            self.onRecentFileRemove(menu, Data)
            return
        if " " in FileName:
            FileName = FileName.replace(" ", "\ ")
        if "&" in FileName:
            FileName = FileName.replace("&", "\&")
        os.system("peony -a %s &" % FileName)

    def recentFilePopup( self, widget, ev, FileData ):
        Data, FileName = FileData
        uri, AppName, appname = Data
        if ev.button == 3:
            mTree = Gtk.Menu()
            mTree.set_name("myGtkLabel")
            mTree.set_events(Gdk.EventMask.POINTER_MOTION_MASK | Gdk.EventMask.POINTER_MOTION_HINT_MASK |
                             Gdk.EventMask.BUTTON_PRESS_MASK | Gdk.EventMask.BUTTON_RELEASE_MASK)
            openRecentFile = Gtk.ImageMenuItem.new_with_mnemonic(_("Open(_O)"))
            image = Gtk.Image.new_from_icon_name("gtk-open", Gtk.IconSize.MENU)
            openRecentFile.set_image(image)

            openDirRecentFile = Gtk.ImageMenuItem.new_with_mnemonic(_("Open directory(_D)"))
            image = Gtk.Image.new_from_icon_name("gtk-directory", Gtk.IconSize.MENU)
            openDirRecentFile.set_image(image)

            separator1 = Gtk.SeparatorMenuItem()

            newDoc = Gtk.ImageMenuItem.new_with_mnemonic(_("New(_N)"))
            image = Gtk.Image.new_from_icon_name("gtk-new", Gtk.IconSize.MENU)
            newDoc.set_image(image)

            copyRecentFile = Gtk.ImageMenuItem.new_with_mnemonic(_("Copy to desktop(_C)"))
            image = Gtk.Image.new_from_icon_name("gtk-copy", Gtk.IconSize.MENU)
            copyRecentFile.set_image(image)

            removeRecentFile = Gtk.ImageMenuItem.new_with_mnemonic(_("Remove from list(_R)"))
            image = Gtk.Image.new_from_icon_name("gtk-remove", Gtk.IconSize.MENU)
            removeRecentFile.set_image(image)

            separator2 = Gtk.SeparatorMenuItem()

            propertyRecentFile = Gtk.ImageMenuItem.new_with_mnemonic(_("Property(_P)"))
            image = Gtk.Image.new_from_icon_name("gtk-properties", Gtk.IconSize.MENU)
            propertyRecentFile.set_image(image)

            openRecentFile.connect( "activate", self.callback, FileData )
            openDirRecentFile.connect( "activate", self.openDir, FileData )
            newDoc.connect( "activate", self.openApp, FileData )
            copyRecentFile.connect( "activate", self.copyFile, FileData )
            removeRecentFile.connect( "activate", self.onRecentFileRemove, Data )
            propertyRecentFile.connect( "activate", self.propertyFile, FileData )

            mTree.append(openRecentFile)
            mTree.append(openDirRecentFile)
            mTree.append(separator1) 
            if appname ==  _("WPS Writer") or appname == _("WPS Presentation") or appname == _("WPS Spreadsheets") or appname == _("Pluma") or appname == "文本编辑器":
                mTree.append(newDoc)
            mTree.append(copyRecentFile)
            mTree.append(removeRecentFile)
            mTree.append(separator2)
            mTree.append(propertyRecentFile)

            mTree.show_all()
            widget.set_name("ButtonAppHover")

            mTree.connect("deactivate", lambda w: widget.set_name("ButtonApp"))

            self.ukuiMenuWin.stopHiding()
            mTree.attach_to_widget(widget)
            mTree.popup(None, None, None, None, ev.button, ev.time)

    def AddRecentBtn( self, Name, RecentImage, Data ):
        DispName=os.path.basename( Name )
        uri, AppName, appname = Data
        FileData = (Data, Name)

        RecFButton = Gtk.Button( "", "ok", True )
        RecFButton.remove( RecFButton.get_children()[0] )
        RecFButton.set_size_request( 200, -1 )
        RecFButton.set_relief( Gtk.ReliefStyle.NONE )
        RecFButton.connect( "clicked", self.callback, FileData )
        RecFButton.connect( "button-press-event", self.recentFilePopup, FileData )
        RecFButton.set_name("ButtonApp")
        RecFButton.show()

        Box1 = Gtk.Box( orientation=Gtk.Orientation.HORIZONTAL, spacing=5 )

        if AppName == "Kylin Video" or AppName == "Qt Creator":
            if RecentImage:
                RecentImage.set_size_request( 20, -1 )
                Box1.add( RecentImage )
        else:
            ButtonIcon = Gtk.Image()
            ButtonIcon.set_size_request( 20, -1 )
            ButtonIcon.set_from_pixbuf(RecentImage)
            Box1.add( ButtonIcon )

        Label1 = Gtk.Label( DispName )
        Label1.set_ellipsize( Pango.EllipsizeMode.END )
        Box1.add( Label1 )

        RecFButton.add( Box1 )
        RecFButton.set_tooltip_text(Name)
        RecFButton.show_all()

        self.recentBox.pack_start( RecFButton, False, True, 0 )

    def getRecentImage(self, filename):
        if not os.path.exists(filename):
            return None
        gfile = Gio.File.new_for_path(filename)
        gicon = gfile.query_info("standard::icon", Gio.FileQueryInfoFlags.NONE, None).get_icon()
        names = Gio.ThemedIcon.get_names(gicon)
        recentImage = Gtk.Image.new_from_icon_name(names[1], Gtk.IconSize.MENU)
        return recentImage

    def startsToShow( self, appname ):
        self.canShow = True
        FileString=[]
        IconString=[]
        UriString=[]
        AppName = None
        if self.ButtonHoverWidgetHistory:
            self.ButtonHoverWidgetHistory.set_name("ButtonApp")
        RecentInfo = RecManagerInstance.get_items()
        cf = UkuiConfigParser()
        for child in self.applicationsBox.get_children():
            if child == self.expanderRecent:
                self.applicationsBox.remove(self.expanderRecent)
            child.hide()

        #qt app
        if appname == _("SMPlayer"):
            pass;
        if appname == _("Kylin Video"):
            path = os.path.join(GLib.get_user_config_dir(), 'kylin-video/kylin-video.ini')
            if os.path.exists(path):
                cf.read(path)
                recentFiles = cf.get("history", "recents")
                if recentFiles == "@Invalid()":
                    return
                recentFiles = recentFiles.split(", ")
                for recfile in recentFiles:
                    if recfile.endswith(","):
                        recfile = recfile[:-1]
                    FileString.append(recfile)
                    IconString.append(self.getRecentImage(recfile))
                    UriString.append(recfile)
                    AppName = "Kylin Video"
        if appname == _("Qt Creator"):
            path = os.path.join(GLib.get_user_config_dir(), 'QtProject/QtCreator.ini')
            if os.path.exists(path):
                cf.read(path)
                recentFiles = cf.get("RecentFiles", "Files")
                if recentFiles == "@Invalid()":
                    return
                recentFiles = recentFiles.replace('\\x', '\\u')
                recentFiles = recentFiles.encode("utf-8").decode("unicode_escape")
                recentFiles = recentFiles.split(", ")
                for recfile in recentFiles:
                    FileString.append(recfile)
                    IconString.append(self.getRecentImage(recfile))
                    UriString.append(recfile)
                    AppName = "Qt Creator"
        #wps
        if appname == _("WPS Writer"):
            for item in RecentInfo:
                if item.get_mime_type() in WPS_W:
                    FileString.append(item.get_uri_display())
                    IconString.append(item.get_icon(Gtk.IconSize.MENU))
                    UriString.append(item.get_uri())
                    AppName = "WPS Writer"
        if appname == _("WPS Presentation"):
            for item in RecentInfo:
                if item.get_mime_type() in WPS_P:
                    FileString.append(item.get_uri_display())
                    IconString.append(item.get_icon(Gtk.IconSize.MENU))
                    UriString.append(item.get_uri())
                    AppName = "WPS Presentation"
        if appname == _("WPS Spreadsheets"):
            for item in RecentInfo:
                if item.get_mime_type() in WPS_S:
                    FileString.append(item.get_uri_display())
                    IconString.append(item.get_icon(Gtk.IconSize.MENU))
                    UriString.append(item.get_uri())
                    AppName = "WPS Spreadsheets"
        #gtk app
        if appname == _("Eye of MATE Image Viewer"):
            for item in RecentInfo:
                if item.has_application("Eye of MATE Image Viewer"):
                    FileString.append(item.get_uri_display())
                    IconString.append(item.get_icon(Gtk.IconSize.MENU))
                    UriString.append(item.get_uri())
                    AppName = "Eye of MATE Image Viewer"
        if appname == _("Atril Document Viewer"):
            for item in RecentInfo:
                if item.has_application("Atril Document Viewer"):
                    FileString.append(item.get_uri_display())
                    IconString.append(item.get_icon(Gtk.IconSize.MENU))
                    UriString.append(item.get_uri())
                    AppName = "Atril Document Viewer"
        if appname == _("Pluma"):
            for item in RecentInfo:
                if item.has_application("Pluma"):
                    FileString.append(item.get_uri_display())
                    IconString.append(item.get_icon(Gtk.IconSize.MENU))
                    UriString.append(item.get_uri())
                    AppName = "Pluma"

        for child in self.recentBox.get_children():
            self.recentBox.remove( child )

        loc = 0
        if not FileString:
            self.expanderRecent.hide()
            self.layout_applications_right.hide()
            self.content_holder.set_size_request(340, self.windowHeight)
            self.showHis = False
            return

        if len(FileString[0]) < 2:
            self.expanderRecent.hide()
            self.layout_applications_right.hide()
            self.content_holder.set_size_request(340, self.windowHeight)
            self.showHis = False
            return
        
        recent_file_num = ukuimenu_settings.get_int("recent-file-num")

        for Name in FileString:
            if loc > recent_file_num - 1:
                break
            if Name != None:
                self.AddRecentBtn( Name, IconString[loc], (UriString[loc], AppName, appname) )
            loc = loc + 1

        self.ButtonHoverWidget.set_name("ButtonAppHover")
        self.ButtonHoverWidgetHistory = self.ButtonHoverWidget
        self.expanderRecent.show()
        self.applicationsBox.pack_start( self.expanderRecent, False, False, 0)
        self.layout_applications_right.show()
        self.content_holder.set_size_request(575, self.windowHeight)
        self.labelExpander.set_text(_("Recent(%s)") % str(loc))
        self.recentShow = None

    def showHistory( self, widget ):
        self.showHis = False
        self.recentHide = None
        self.recentShow = None
        self.ButtonHoverWidget = widget
        for item in APPLIST:
            if widget.appName == item:
                self.showHis = True

        if not self.showHis:
            self.recentHide = GLib.timeout_add( 500, self.startsToHide )
            return

        widget.set_state(Gtk.StateType.PRELIGHT)
        self.recentShow = GLib.timeout_add( 500, self.startsToShow, widget.appName )

    def startsToHide( self ):
        if self.ButtonHoverWidgetHistory:
            self.ButtonHoverWidgetHistory.set_name("ButtonApp")
        if self.showHis:
            return
        self.expanderRecent.hide()
        self.layout_applications_right.hide()
        self.content_holder.set_size_request(340, self.windowHeight)
        self.recentHide = None
        self.ButtonHoverWidgetHistory = None

    def hideHistory( self, widget ):
        if self.showHis:
            if self.recentShow:
                GLib.source_remove( self.recentShow )
        if not self.showHis:
            if self.recentHide:
                GLib.source_remove( self.recentHide )

    def scrollItemIntoView( self, widget, event = None ):
        viewport = widget.get_parent()
        while not isinstance( viewport, Gtk.Viewport ):
            if not viewport.get_parent():
                return
            viewport = viewport.get_parent()
        aloc = widget.get_allocation()
        viewport.get_vadjustment().clamp_page(aloc.y, aloc.y + aloc.height)

    def searchIconPressed(self, entry, icon_pos, event):
        if self.searchEntry.get_text() != "" and self.searchEntry.get_text() != _("Search local program"):
            self.searchEntry.set_text("")

    def searchChanged(self, widget):
        if self.searchEntry.get_text() != "":
            iconpath = ICON_PATH + "so-close.png"
            pixbuf = Pixbuf.new_from_file(iconpath)
            pixbuf = pixbuf.scale_simple(16, 16, 2)  #2 := BILINEAR
            self.searchEntry.set_icon_from_pixbuf(1, pixbuf)
        else:
            iconpath = ICON_PATH + "so.png"
            pixbuf = Pixbuf.new_from_file(iconpath)
            pixbuf = pixbuf.scale_simple(16, 16, 2)  #2 := BILINEAR
            self.searchEntry.set_icon_from_pixbuf(1, pixbuf)

    def viewOpened(self, matcher, view, user_data):
        recentApp = {}
        tempName = ""
        found = False
        try:
            viewName = Bamf.View.get_name(view)
            if viewName == "ukui-menu.py":
                return
            if viewName == "QQ International" or viewName == "TXMenuWindow":
                appName = "qq国际版"
                found = True
            if viewName == "Mozilla Firefox":
                appName = "firefox 网络浏览器"
                found = True

            if not found:
                obj = dbus.SessionBus().get_object('org.ayatana.bamf', '/org/ayatana/bamf/matcher')
                interface = dbus.Interface(obj, 'org.ayatana.bamf.matcher')
                application = interface.ActiveApplication()
                obj = dbus.SessionBus().get_object('org.ayatana.bamf', application)
                interface = dbus.Interface(obj, 'org.ayatana.bamf.application')
                desktopname = interface.DesktopFile()
                tempPath = os.path.basename(desktopname)
                desktopPath = get_desktop_file_path(tempPath)
                appInfo = Gio.DesktopAppInfo.new_from_filename(desktopname)
                tempName = Gio.DesktopAppInfo.get_display_name(appInfo)
        except Exception as e:
        #    print (e)
            return

        if tempName.endswith('.py'):
            return
        # special applications
        if "Chromium" in tempName:
            appName = _("Chromium Browser")
            found = True
        elif "Google Chrome" in tempName:
            appName = "google chrome"
            found = True
        elif tempName.lower() == "fcitx config tool":
            appName = _("Fcitx Config")
            found = True

        if not found:
            for appname in self.all_application_list:
                if appname["entry"].get_name() is None:
                    continue
                if desktopPath:
                    button = MenuApplicationLauncher(desktopPath, 32, "", True, highlight=(False))
                    button.set_name("ButtonApp")
                    if button.appName.lower() == appname["entry"].get_name().decode('utf-8').lower():
                        appName = appname["entry"].get_name().decode('utf-8').lower()
                        found = True
                        break
                    if desktopPath == appname["entry"].get_desktop_file_path().decode('utf-8'):
                        appName = appname["entry"].get_name().decode('utf-8').lower()
                        found = True
                        break
                    if tempName.lower() == appname["entry"].get_name().decode('utf-8').lower():
                        appName = appname["entry"].get_name().decode('utf-8').lower()
                        found = True
                        break
                if viewName:
                    if viewName.lower() == appname["entry"].get_name().decode('utf-8').lower():
                        appName = appname["entry"].get_name().decode('utf-8').lower()
                        found = True
                        break
        if not found:
            return
        #read and write recentAppLog file
        if os.path.exists(GLib.get_home_dir() + "/.recentAppLog"):
            f = open(GLib.get_home_dir() + "/.recentAppLog", 'r')
            lines = f.readlines()
            f.close()
            for line in lines:
                word = line.strip().split(':')
                recentApp[word[0]] = int(word[len(word) - 1])
            f = open(GLib.get_home_dir() + "/.recentAppLog", 'w')
        else:
            f = open(GLib.get_home_dir() + "/.recentAppLog", 'w')

        recentApp[appName] = recentApp.get(appName,0) + 1
        srApp = sorted(iter(recentApp.items()), key = operator.itemgetter(1), reverse=True)
        for i in range(len(srApp)):
            f.write(srApp[i][0] + ":" + str(srApp[i][1]) + "\n")
        f.close()
        self.updateHistoryBox()

    def updateHistoryBox( self, settings = None, key = None, args = None ):
        for child in self.recentAppBox:
            child.destroy()
        historyDisplayNum = ukuimenu_settings.get_int("recent-app-num")
        self.currentHisCount = 1

        try:
            applicationsFile = open(self.favoritesPath, "r")
            applicationsList = applicationsFile.readlines()
            applicationsFile.close()
            self.favAppList = applicationsList
            if os.path.exists(GLib.get_home_dir() + "/.recentAppLog"):
                f = open(GLib.get_home_dir() + "/.recentAppLog", 'r')
            else:
                return
            lines = f.readlines()
            f.close()
            for line in lines:
                word = line.strip().split(':')
                found = False
                for appname in self.all_application_list:
                    if appname["entry"].get_desktop_file_path() is None:
                        continue
                    findDesktop = False
                    for tempdesktop in self.favAppList:
                        temp = tempdesktop.split(":")[1]
                        if appname["entry"].get_desktop_file_path().decode('utf-8') in temp or os.path.basename(appname["entry"].get_desktop_file_path().decode('utf-8')) in temp or os.path.basename(temp) in appname["entry"].get_desktop_file_path().decode('utf-8'):
                            findDesktop = True
                    if word[0].lower() == appname["entry"].get_name().decode('utf-8').lower() and findDesktop == False: 
                        button = MenuApplicationLauncher( appname["entry"].get_desktop_file_path().decode('utf-8'), 32, "", True, highlight=(False) )
                        if button.appExec:
                            #button.set_tooltip_text( button.getTooltip() )
                            button.connect( "clicked", lambda w: self.ukuiMenuWin.hide() )
                            button.isRecentApp = True
                            button.connect( "button-press-event", self.menuPopup )

                            if self.showRecentFile:
                                self.ShowRecentFile(button)
                                button.connect( "enter", self.showHistory )
                                button.connect( "leave", self.hideHistory )

                        found = True
                        break
                if found and (self.currentHisCount <= historyDisplayNum):
                    self.recentAppBox.pack_start( button, False, True, 0 )
                    button.set_name("ButtonApp")
                    button.show()
                    Gdk.flush()
                    if self.currentHisCount == historyDisplayNum:
                        break
                    else:
                        self.currentHisCount += 1

        except Exception as e:
            print (e)
        
        Gdk.flush()

    def keyPress( self, widget, event ):
        if event.string.strip() != "" or event.keyval == Gdk.KEY_BackSpace:
            self.searchEntry.grab_focus()
            self.searchEntry.set_position(-1)
            self.searchEntry.event( event )
            return True

        if event.keyval == Gdk.KEY_Down and self.searchEntry.is_focus():
            notebook = self.builder.get_object( "notebook_left" )
            page = notebook.get_current_page()
            if page == 0:
                self.favoritesBox.get_children()[0].grab_focus()    #Todo: focus the favoritesBox but not the recentAppBox now. 
            elif page == 1:
                self.categoriesBox.get_children()[0].grab_focus()
            else:
                self.searchResultBox.get_children()[0].grab_focus()

        return False
