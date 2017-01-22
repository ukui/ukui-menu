#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# Copyright (C) 2007-2014 Clement Lefebvre <root@linuxmint.com>
# Copyright (C) 2015 Martin Wimpress <code@ubuntu-mate.org>
# Copyright (C) 2016 National University of Defense Technology(NUDT) & Ukui Ltd
# Copyright: 2016,Tianjin UKUI Information Technology Co., Ltd. 
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
import threading
import commands
import subprocess
import filecmp
import ctypes
from ctypes import *
from ukui_menu.easybuttons import *
from ukui_menu.execute import Execute
from ukui_menu.easygsettings import EasyGSettings
from ukui_menu.easyfiles import *
from user import home
from ukui_menu.filemonitor import monitor as filemonitor
from xml.etree import ElementTree as ET
import copy
from pyinotify import WatchManager, Notifier, ProcessEvent, IN_DELETE_SELF,IN_ACCESS
import dbus
from configobj import ConfigObj
import thread
import ukuimenu
import operator
import platform

# i18n
gettext.install("ukui-menu", "/usr/share/locale") 

ICON_PATH = "/usr/share/ukui-menu/icons/"
ICON_SIZE = 16
ukuimenu_settings=Gio.Settings.new("org.ukui.ukui-menu.plugins.menu")
FAVNUM = 5            #常用软件显示个数

server = False
try:
    (status, output) = commands.getstatusoutput("uname -a | grep server")
    if status == 0:
        server = True
    else:
        server = False
except Exception, e:
        print e

serverx86 = False
try:
    (statusx86, outputx86) = commands.getstatusoutput("dpkg -l | grep ukui-server-guide")
    if statusx86 == 0:
        serverx86 = True
    else:
        serverx86 = False
except Exception, e:
        print e

def get_user_icon():
    current_user = GLib.get_user_name ()
    try:
        bus = Gio.bus_get_sync (Gio.BusType.SYSTEM, None)
        result = bus.call_sync ('org.freedesktop.Accounts',
                                '/org/freedesktop/Accounts',
                                'org.freedesktop.Accounts',
                                'FindUserByName',
                                GLib.Variant ('(s)', (current_user,)),
                                GLib.VariantType.new ('(o)'),
                                Gio.DBusCallFlags.NONE,
                                -1,
                                None)
        (path,) = result.unpack ()

        result = bus.call_sync ('org.freedesktop.Accounts',
                                path,
                                'org.freedesktop.DBus.Properties',
                                'GetAll',
                                GLib.Variant ('(s)', ('org.freedesktop.Accounts.User',)),
                                GLib.VariantType.new ('(a{sv})'),
                                Gio.DBusCallFlags.NONE,
                                -1,
                                None)
        (props,) = result.unpack ()
        usericon = props['IconFile']
        return usericon
    except Exception, e:
        print e
        return GLib.get_home_dir() + "/.face"

usericonPath = get_user_icon()

class EventHandle(ProcessEvent):
    def process_IN_DELETE_SELF(self,event):
        ifchangegsettings = ukuimenu_settings.get_boolean("ifchange")
        if ifchangegsettings:
            os.system("gsettings set org.ubuntu-ukui.ukuimenu.plugins.menu ifchange false")
        else:
            os.system("gsettings set org.ubuntu-ukui.ukuimenu.plugins.menu ifchange true")

def FSMonitor(self):
    wm = WatchManager()
    mask = IN_DELETE_SELF | IN_ACCESS
    notifier = Notifier(wm,EventHandle())
    while True:
        try:
            notifier.process_events()
            wm.add_watch(usericonPath,mask,rec=True)
            if notifier.check_events():
                notifier.read_events()
        except KeyboardInterrupt:
            notifier.stop()
            break

def get_user_item_path():
    item_dir = None

    if os.environ.has_key('XDG_DATA_HOME'):
        item_dir = os.path.join(os.environ['XDG_DATA_HOME'], 'applications')
    else:
        item_dir = os.path.join(os.environ['HOME'], '.local', 'share', 'applications')

    if not os.path.isdir(item_dir):
        os.makedirs(item_dir)

    return item_dir

def get_system_item_paths():
    item_dirs = None

    if os.environ.has_key('XDG_DATA_DIRS'):
        item_dirs = os.environ['XDG_DATA_DIRS'].split(":")
    else:
        item_dirs = [os.path.join('usr', 'share')]

    return item_dirs

def get_desktop_file_path(desktop_filename):
    if not desktop_filename:
        return ''
    path = os.path.join(get_user_item_path(), desktop_filename)
    if os.path.exists(path):
        return path
    for p in get_system_item_paths():
        path = os.path.join(p, 'applications', desktop_filename)
        if os.path.exists(path):
            return path

    return ''

def get_category_file_path():
    dir_path = os.path.dirname(os.path.abspath(sys.argv[0]))
    system_category_file_path = os.path.join(dir_path, 'category.xml')
    os.system("mkdir -p %s/.config/ukui-menu/category/" % home)
    user_category_file_path = '%s/.config/ukui-menu/category/category.xml' % home
    if not os.path.exists(user_category_file_path):
        os.system("cp %s %s" % (system_category_file_path, user_category_file_path))

    return user_category_file_path
class Category(GObject.GObject):

    def __init__(self, all_application_list):
        GObject.GObject.__init__ (self)

        self.category_list_m = []
        self.application_list_m = []
        self.current_category = None
        self.all_application_list = all_application_list

        if not server or not serverx86:
            for app in self.all_application_list:
                if app['category'] == 'Games' or app['category'] == _("Games") or app['category'] == '游戏':
                    self.application_list_m.append(app)

#        for app in self.all_application_list:
#            if app['category'] == 'Office' or app['category'] == _('Office') or app['category'] == '办公':
#                self.application_list_m.append(app)

        if not server or not serverx86:
            self.application_list_m += self.autostart_list()

        self.category_file = open(get_category_file_path(), 'r+b')
        self.tree = ET.ElementTree(file = self.category_file.name)
        self.load_tree_data()
    def move_to_category(self, application, category):
        found = False
        for cat in self.category_list_m:
            if cat['name'] == category['name'] and \
               cat['icon'] == category['icon'] and \
               cat['tooltip'] == category['tooltip'] and \
               cat['filter'] == category['filter']:
                found = True
                break
        if not found:
            return

        found = False
        for app in self.all_application_list:
            if app['desktop_file_path'] == application['desktop_file_path']:
                found = True
                break
        if not found:
            return

        application['category'] = category['name']

        # sync to the category file
        for child in self.tree.getroot():
            if child.attrib['name'] == str(category['name']).decode('UTF-8'):
                ET.SugElement(child, 'Entry',
                              {'name': application['name'].decode('UTF-8'), 'source': application['desktop_file_path'].decode('UTF-8')})
                break
        tree.write(self.category_file, 'UTF-8')
        self.category_file.flush()

    def load_tree_data(self):
        for directory in self.tree.getroot():
            name = directory.attrib['name'].encode('UTF-8')
            icon = directory.attrib['icon'].encode('UTF-8')
            self.category_list_m.append( { "name": name, "icon": icon, "tooltip": name, "filter": name } )
            for entry in directory:
                desktop_file_path = get_desktop_file_path(entry.attrib['source'].encode('UTF-8'))
                if not desktop_file_path:
                    continue
                found = False
                for app in self.all_application_list:
                    if app['desktop_file_path'] == desktop_file_path:
                        found = True
                        break
                if not found:
                    continue
                self.application_list_m.append( { 'desktop_file_path': desktop_file_path, 'category': name } )

    def category_list(self):
        return self.category_list_m

    def application_list(self):
        return self.application_list_m

    def autostart_list(self):
        autostart_dirs = []
        try:
            autostart_dirs = [os.path.join(p, 'autostart') for p in os.environ['XDG_CONFIG_DIRS'].split(':')]
            # home's autostart has the highest priority
            autostart_dirs.insert(0, '%s/.config/autostart' % home)
            autostart_dirs = [ d for d in autostart_dirs if os.path.exists(d) ]
        except KeyError, e:
            print 'No environment variable called XDG_CONFIG_DIRS'

        autostart_apps = []
        autostart_desktop_filenames = []
        for di in autostart_dirs:
            for filename in os.listdir(di):
                file_path = os.path.join(di, filename)
                if filename in autostart_desktop_filenames:
                    continue
                autostart_desktop_filenames.append(filename)
                try:
                    app = Gio.DesktopAppInfo.new_from_filename(file_path)
                    if app.get_show_in('MATE'):
                        autostart_apps.append(app)
                except TypeError,e:
                    print "error"

        for app in autostart_apps:
            # we have to use get_string here since false as a boolean may indicate the
            # key does not exist in which condition the remove op will not be performed
            if app.get_string('X-MATE-Autostart-enabled') == 'false':
                autostart_apps.remove(app)
            if app.get_string('Name') == "MATE Settings Daemon":
                autostart_apps.remove(app)

       # TODO: use translation
        return [{'desktop_file_path': app.get_filename(), 'category': '启动'} for app in autostart_apps]


class Menu:
    def __init__( self, MenuToLookup ):
        self.tree = ukuimenu.lookup_tree( MenuToLookup )
        self.directory = self.tree.get_root_directory()

    def getMenus( self, parent = None ):
        if parent == None:
            yield self.tree.root
        else:
            for menu in parent.get_contents():
                if menu.get_type() == ukuimenu.TYPE_DIRECTORY and self.__isVisible( menu ):
                    yield menu

    def getItems( self, menu ):
        for item in menu.get_contents():
            if item.get_type() == ukuimenu.TYPE_ENTRY and item.get_desktop_file_id()[-19:] != '-usercustom.desktop' and self.__isVisible( item ):
                yield item

    def __isVisible( self, item ):
        if item.get_type() == ukuimenu.TYPE_ENTRY:
            return not ( item.get_is_excluded() or item.get_is_nodisplay() )
        if item.get_type() == ukuimenu.TPYE_DIRECTORY and len( item.get_contents() ):
            return True

class pluginclass( object ):
    TARGET_TYPE_TEXT = 80
    toButton = ( Gtk.TargetEntry.new( "text/uri-list", 0, TARGET_TYPE_TEXT ), Gtk.TargetEntry.new( "text/uri-list", 0, TARGET_TYPE_TEXT ) )
    TARGET_TYPE_FAV = 81
    toFav = ( Gtk.TargetEntry.new( "FAVORITES", Gtk.TargetFlags.SAME_APP, 81 ), Gtk.TargetEntry.new( "text/plain", 0, 100 ), Gtk.TargetEntry.new( "text/uri-list", 0, 101 ) )
    fromFav = ( Gtk.TargetEntry.new( "FAVORITES", Gtk.TargetFlags.SAME_APP, 81 ), Gtk.TargetEntry.new( "FAVORITES", Gtk.TargetFlags.SAME_APP, 81 ) )

    def __init__( self, ukuiMenuWin, toggleButton ):
        self.ukuiMenuWin = ukuiMenuWin
        self.toggleButton = toggleButton
        self.de = "ukui"

        self.builder = Gtk.Builder()
        #The Glade file for the plugin
        self.builder.add_from_file (os.path.join( '/', 'usr', 'share', 'ukui-menu',  'plugins', 'ukuimenu.glade' ))
        ukuimenu_settings.connect("changed::ifchange",self.changeimage)
        #Set 'heading' property for plugin
        self.heading = ""
        self.windowHeight = 505
        self.addedHeight = 0

        self.window = self.builder.get_object( "window" )
        self.content_holder = self.builder.get_object( "eventbox" )
        self.content_holder.connect( "key-press-event", self.keyPress )
        self.content_holder.set_size_request(575, self.windowHeight)

        style_provider = Gtk.CssProvider()
        try:
            css = open( os.path.join( '/', 'usr', 'share', 'ukui-menu', 'ukuimenu.css' ), 'rb' )
            css_data = css.read()
            css.close()
            style_provider.load_from_data(css_data)
            Gtk.StyleContext.add_provider_for_screen(Gdk.Screen.get_default(), style_provider, Gtk.STYLE_PROVIDER_PRIORITY_APPLICATION)
        except Exception, e:
            print e
        
        self.layout_left = self.builder.get_object("layout_left")
        self.viewport1 = self.builder.get_object("viewport1")
        self.viewport2 = self.builder.get_object("viewport2")
        self.vp_right_container = self.builder.get_object("vp_right_container")
        self.vp_right_container.set_name("Viewport1")

        self.viewport_left = self.builder.get_object("viewport_left")
        self.viewport_left.set_name("Viewport")
        self.viewport_right = self.builder.get_object("viewport_right")
        self.viewport_right.set_name("Viewport")
        self.vp_left_container = self.builder.get_object("vp_left_container")
        self.vp_left_container.set_name("Viewport1")
        self.scrolledwindow_fav = self.builder.get_object("scrolledwindow_fav")
        self.scrolledwindow_fav.set_name("Viewport1")
        self.viewport_fav = self.builder.get_object("viewport_fav")
        self.viewport_fav.set_name("Viewport1")
        self.viewport_allapp = self.builder.get_object("viewport_allapp")
        self.viewport_allapp.set_name("Viewport1")

        self.button_user = self.builder.get_object("button_user")
        self.button_user.connect("button-press-event", self.on_button_user_clicked)

        self.button_showall = self.builder.get_object("button_showall")
        self.button_showall.set_label(_("All App"))
        self.button_showall.set_name("ButtonApp")
        self.button_showall.connect("clicked", self.on_button_showall_clicked)

        self.button_back = self.builder.get_object("button_back")
        self.button_back.set_label(_("Favorite"))
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
        except Exception, e:
            iconpath = "/usr/share/ukui-menu/icons/stock_person.png"
            os.system("cp %s %s" % (iconpath, usericonPath))
            pixbuf = Pixbuf.new_from_file(iconpath)
            print e
        pixbuf = pixbuf.scale_simple(63, 63, 2)  #2 := BILINEAR
        self.image_user.set_from_pixbuf(pixbuf)

        self.image3 = self.builder.get_object("image3")
        iconfile = ICON_PATH + "sep.png"
        pixbuf = Pixbuf.new_from_file(iconfile)
        pixbuf = pixbuf.scale_simple(80, 2, 2)  #2 := BILINEAR
        self.image3.set_from_pixbuf(pixbuf)

        self.button_shutdown = self.builder.get_object("button_shutdown")
        self.button_shutdown.set_name("ButtonPower")
        self.viewport18 = self.builder.get_object("viewport18")
        self.viewport18.set_name("ViewportPower")
        self.button_shutdown.set_label(_("Power"))
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
        #self.favoritesBox.drag_dest_set ( Gtk.DestDefaults.MOTION | Gtk.DestDefaults.HIGHLIGHT | Gtk.DestDefaults.DROP,  self.toButton, 2, Gdk.DragAction.COPY )

        self.AddFavBtn()

        self.hovermenupopup = False

        self.appHistoryList= self.buildAppHistoryList()

        self.categoriesBox = self.builder.get_object( "categoriesbox" )
        self.categoriesBox.set_spacing(2)
        self.applicationsBox = self.builder.get_object( "applicationsBox" )
        self.applicationsBox.set_spacing(2)
        self.applicationsScrolledWindow = self.builder.get_object( "applicationsScrolledWindow")

        self.favappbutton = self.builder.get_object( "button_favapp" )
        self.favappbutton.set_label(_("Favorite"))
        self.favappbutton.set_name("Button")
        self.change_icon(self.favappbutton, ICON_PATH + "favapp.png")
        self.favappbutton.connect("enter", self.favappbutton_enter)
        self.favappbutton.connect("leave", self.favappbutton_leave)
        self.favappbutton.connect("clicked", lambda c1: self.changeTab(0))

        self.viewportfav = self.builder.get_object( "viewportfav" )
        self.viewportallapp = self.builder.get_object( "viewportallapp" )

        self.allappbutton = self.builder.get_object( "button_allapp" )
        self.allappbutton.set_label(_("All App"))
        self.allappbutton.set_name("Button")
        self.change_icon(self.allappbutton, ICON_PATH + "allapp.png")
        self.allappbutton.connect("enter", self.allappbutton_enter)
        self.allappbutton.connect("leave", self.allappbutton_leave)
        self.allappbutton.connect("clicked", lambda c2: self.changeTab(1))

        self.viewport_entry = self.builder.get_object( "viewport_entry" )
        self.viewport_entry.set_name("Viewport")
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
        GLib.timeout_add( 100, self.getBamfStatus )

        self.updateHistoryBox()

        self.label_user = self.builder.get_object("label_user")
        user_name = GLib.get_user_name()
        self.label_user.set_text(user_name)
        self.label_user.set_name("myGtkLabel")
        #left-menu
        self.button_computer = self.builder.get_object( "button_computer" )
        self.button_computer.set_label(_("Home"))
        self.button_computer.set_name("Button")
        self.change_icon(self.button_computer, ICON_PATH + "computer.png")
        self.button_computer.connect("button-press-event",self.on_computer_clicked)
        self.button_computer.connect("enter", self.button_computer_enter)
        self.button_computer.connect("leave", self.button_computer_leave)

        self.button_controlcenter = self.builder.get_object( "button_controlcenter" )
        self.button_controlcenter.set_label(_("Settings"))
        self.button_controlcenter.set_name("Button")
        self.change_icon(self.button_controlcenter, ICON_PATH + "controlcenter.png")
        self.button_controlcenter.connect("button-press-event", self.on_controlcenter_clicked)
        self.button_controlcenter.connect("enter", self.button_controlcenter_enter)
        self.button_controlcenter.connect("leave", self.button_controlcenter_leave)

        self.label3 = self.builder.get_object("label3")
        self.label3.set_text(_("No items matched"))

        self.iconSize = 20
        self.favCols = 1

        self.buildingButtonList = False
        self.stopBuildingButtonList = False

        self.rebuildLock = False
        self.activeFilter = (1, "", self.searchEntry)
        self.current_suggestion = None
        self.categoryList = []
        self.applicationList = []
        self.uncategoriedList = []
        self.all_app_list = []
        self.categorybutton_list = []

        self.all_application_list = []
        for mainitems in [ "ukui-applications.menu", "ukui-settings.menu" ]:
            mymenu = Menu( mainitems )
            mymenu.tree.add_monitor( self.menuChanged, None )

    def getBamfStatus(self):
        if self.bamfok:
            return

        self.bamfok = True
        try:
            self.matcher = Bamf.Matcher.get_default()
            self.matcher.connect('view-opened', self.viewOpened, None)
        except Exception, e:
            print e
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
        os.system("peony %s" % realPath)

    def button_controlcenter_enter(self, *args, **kargs):
        self.change_icon(self.button_controlcenter, ICON_PATH + "controlcenter-active.png")
    def button_controlcenter_leave(self, *args, **kargs):
        self.change_icon(self.button_controlcenter, ICON_PATH + "controlcenter.png")
    def on_controlcenter_clicked(self, widget, event):
        if os.path.exists("/usr/bin/ukui-control-center"):
                os.system("ukui-control-center &")
        else:
                os.system("mate-control-center &")

    def on_button_user_clicked ( self, widget, event):
        self.ukuiMenuWin.hide()
        os.system('ukui-control-center -u &')

    def on_button_showall_clicked(self, widget):
        self.changeTab(1)

    def on_button_back_clicked(self, widget):
        self.changeTab(0)

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
        except Exception, e:
            print e
    
    def favoritesSave( self ):
        try:
            self.checkUkuiMenuFolder()
            appListFile = open(self.favoritesPath, "w")

            for favorite in self.favorites:
                if favorite.type == "location":
                    appListFile.write( "location:" + favorite.desktopFile + "\n" )
                else:
                    appListFile.write( favorite.type + "\n" )
 
            appListFile.close()
        except Exception, e:
            print e

    def isLocationInFavorites( self, location ):
        for fav in self.favorites:
            if fav.type == "location" and fav.desktopFile == location:
                return True

        return False

    def checkUkuiMenuFolder( self ):
        if os.path.exists( os.path.join( os.path.expanduser( "~" ), ".config", "ukui-menu", "applications" ) ):
            return True
        try:
            os.makedirs( os.path.join( os.path.expanduser( "~" ), ".config", "ukui-menu", "applications" ) )
            return True
        except:
            pass

        return False

    def favoritesPositionOnGrid( self , favorite):
        row = 0
        col = 0
        for fav in self.favorites:
            if fav.position == favorite.position:
                break
            row +=1
        self.favoritesBox.attach( favorite, col, col + 1, row, row + 1) #table自适应填充，随fav多少高度跟随改变
   
    def onFavButtonDragReorderGet( self, widget, context, selection, targetType, eventTime ):
        if targetType == self.TARGET_TYPE_FAV:
            self.drag_origin = widget.position
            selection.set( selection.get_target(), 8, str(widget.position))
    
    def onFavButtonDragReorder( self, widget, context, x, y, selection, targetType, time  ):
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

    def callback(self, widget, filename=None):
        self.ukuiMenuWin.hide()
        x = os.system("gvfs-open \""+filename+"\"")
        if x == 256:
            dia = Gtk.Dialog('File not found!',
                             None,
                             Gtk.DialogFlags.MODAL | Gtk.DialogFlags.DESTROY_WITH_PARENT,
                             ("ok", 77))
            dia.vbox.pack_start(Gtk.Label('The location or file could not be found!'), False, False, 0)
            dia.vbox.show_all()
            dia.show()
            result = dia.run()
            if result == 77:
                dia.destroy()

    def favoritesBuildLauncher(self, location):
        try:
            ButtonIcon = None
            location = string.join(location.split("%20"))
            for fav in self.favorites:
                if fav.type == "location" and fav.desktopFile == location:
                    return None
            favButton = FavApplicationLauncher( location, 32, False)
            if favButton.appExec:
                if favButton.appName == "Caja" or favButton.appExec == "ukui-terminal":
                    return None
                favButton.show()
                favButton.connect( "clicked", lambda w: self.ukuiMenuWin.hide() )
                favButton.connect( "button-press-event", self.favPopup )
                favButton.set_tooltip_text( favButton.getTooltip() )
                favButton.type = "location"
                return favButton
        except Exception, e:
            print "s"

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
        button.set_name("ButtonApp")
        addedDesktop = False
        try:
            # Determine where the Desktop folder is (could be localized)
            import sys, commands
            sys.path.append('/usr/lib/ubuntu-ukui/common')
            from configobj import ConfigObj
            config = ConfigObj(home + "/.config/user-dirs.dirs")
            desktopDir = home + "/Desktop"
            tmpdesktopDir = config['XDG_DESKTOP_DIR']
            tmpdesktopDir = commands.getoutput("echo " + tmpdesktopDir)
            if os.path.exists(tmpdesktopDir):
                desktopDir = tmpdesktopDir
            # Copy the desktop file to the desktop
            if (os.path.exists(desktopDir + "/" + os.path.basename(widget.desktopFile))):
                addedDesktop = True
        except Exception, detail:
            print detail

        text = ""
        if ev.button == 3:
            if ev.y > widget.get_allocation().height / 2:
                insertBefore = False
            else:
                insertBefore = True

            if widget.type == "location":
                if (os.path.exists(GLib.get_home_dir() + "/.applet")):
                    f = open(GLib.get_home_dir() + "/.applet")
                    text = f.read()

                mTree = Gtk.Menu()
                mTree.set_name("myGtkLabel")
                mTree.set_events(Gdk.EventMask.POINTER_MOTION_MASK | Gdk.EventMask.POINTER_MOTION_HINT_MASK |
                                 Gdk.EventMask.BUTTON_PRESS_MASK | Gdk.EventMask.BUTTON_RELEASE_MASK)
                launchMenuItem = Gtk.MenuItem(_("Open(_O)"))
                launchMenuItem.set_use_underline(True)
                separator1 = Gtk.SeparatorMenuItem()
                desktopMenuItem = Gtk.MenuItem(_("Add to desktop"))
                desktopMenuItem.set_use_underline(True)
                if addedDesktop:
                    desktopMenuItem.set_sensitive(False)
                panelMenuItem = Gtk.MenuItem(_("Lock to panel(_L)"))
                panelMenuItem.set_use_underline(True)
                separator3 = Gtk.SeparatorMenuItem()
                removeFromFavMenuItem = Gtk.MenuItem(_("Unlock from startup menu"))
                if len(self.favorites) < 2:
                    removeFromFavMenuItem.set_sensitive(False)
                separator4 = Gtk.SeparatorMenuItem()
                propsMenuItem = Gtk.MenuItem(_("Property(_P)"))
                propsMenuItem.set_use_underline(True)

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
                widget.set_sensitive(False)
                button.set_sensitive(False)
                mTree.attach_to_widget(widget)
                mTree.popup(None, None, None, None, ev.button, ev.time)

    def FSMthread(self):
        thread.start_new_thread(FSMonitor,(self,))

    def get_realPath(self, path, configDir):
        realPath = GLib.get_home_dir() + path
        try:
            config = ConfigObj(GLib.get_home_dir() + "/.config/user-dirs.dirs")
            chinesePath = config[configDir]
            chinesePath = commands.getoutput("echo "+ chinesePath)
            if os.path.exists(chinesePath):
                realPath = chinesePath
        except Exception, detail:
            print detail
        return realPath

    def on_button_user_clicked ( self, widget, event):
        self.ukuiMenuWin.hide()
        os.system('ukui-control-center -u &')

    def changeimage(self,settings,key,args=None):
        usericonPath = get_user_icon()
        pixbuf = Pixbuf.new_from_file(usericonPath)
        pixbuf = pixbuf.scale_simple(63, 63, 2)  #2 := BILINEAR
        self.image_user.set_from_pixbuf(pixbuf)

    def shutdownmenuPopup(self, widget):
        if True:      #1-左右，2-中，3-右
            mTree = Gtk.Menu()
            mTree.set_name("myGtkLabel")

            shutdown = Gtk.MenuItem(_("Shutdown"))
            reboot = Gtk.MenuItem(_("Reboot"))
            separatorMenuItem = Gtk.SeparatorMenuItem()
            separatorMenuItem.set_visible(True)
            logout = Gtk.MenuItem(_("Logout"))
            changeuser = Gtk.MenuItem(_("Switch User"))
            lock = Gtk.MenuItem(_("Lock Screen"))
            mTree.append(lock)
            arm = False
            machine = platform.machine()
            if machine == "aarch64":
                arm = True
            else:
                arm = False
            if not arm:
                mTree.append(changeuser)
            mTree.append(logout)
            mTree.append(separatorMenuItem)
            mTree.append(reboot)
            mTree.append(shutdown)
            mTree.show_all()
            lock.connect("activate", self.lock, widget)
            changeuser.connect("activate", self.switchuser, widget)
            logout.connect("activate", self.logout, widget)
            reboot.connect("activate", self.reboot, widget)
            shutdown.connect("activate", self.shutdown, widget)
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
        obj = system_bus.get_object('org.freedesktop.DisplayManager',xdg_seat_path)
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

    def lock(self, menu, widget):
        os.system( "ukui-screensaver-command -l" )
        self.ukuiMenuWin.hide()
        Gdk.flush()

    def onShowMenu( self ):
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
            if os.path.exists(heightPath):
                f = open(heightPath, "r")
                lines = f.readlines()
                length = lines[0]
                aa = int(length)
                f.close()
        if self.windowHeight > screenHeight - 60:
            self.addedHeight = screenHeight - 60 - 505
            self.windowHeight = screenHeight - 60
        self.content_holder.set_size_request(340, self.windowHeight)
        self.updateHeight()
        Gdk.flush()

    def updateHeight( self ):
        self.layout_left.move(self.viewport_entry, 103, 465+self.addedHeight)
        self.layout_left.move(self.viewport18, 4, 465+self.addedHeight)
        self.layout_left.move(self.viewport23, 75, 465+self.addedHeight)
        self.viewport1.set_size_request(236, 455+self.addedHeight)
        self.vp_left_container.set_size_request(227, 450+self.addedHeight)
        self.viewport2.set_size_request(236, 455+self.addedHeight)
        self.vp_right_container.set_size_request(227, 450+self.addedHeight)

    def do_plugin( self ):
        self.Todos()

    def Todos( self ):
        self.searchEntry.connect( "changed", self.Filter )
        self.searchEntry.connect( "activate", self.Search )

        self.buildButtonList()

    def buildButtonList( self ):
        if self.buildingButtonList:
            self.stopBuildingButtonList = True
            GLib.timeout_add( 100, self.buildButtonList )
            return
        self.stopBuildingButtonList = False
        self.updateBoxes(False)

    def changeShowCategoryIcons( self, settings, key, args ):
        categoryIconSize = self.iconSize

        for child in self.categoriesBox:
            child.setIconSize( categoryIconSize )

    def menuChanged( self, x, y ):
        # wait some miliseconds because there a multiple events send at the same time and we don't want to rebuild the menu for each
        if self.menuChangedTimer:
            GLib.source_remove( self.menuChangedTimer )

        self.menuChangedTimer = GLib.timeout_add( 100, self.updateBoxes, True )

    def updateBoxes(self, menu_has_changed):
        if self.rebuildLock:
            return

        self.rebuildLock = True
        self.menuChangedTimer = None
        self.loadMenuFiles()

        self.all_application_list = self.build_application_list()
        cat = Category(self.all_application_list)

        new_category_list = cat.category_list()
        added_category_list = []
        removed_category_list = []

        if not self.categoryList:
            added_category_list = new_category_list
        else:
            for item in new_category_list:
                found = False
                for item2 in self.categoryList:
                    if item2['name'] == item['name'] and \
                       item2['icon'] == item['icon'] and \
                       item2['tooltip'] == item['tooltip'] :
                        found = True
                        break
                if not found:
                    added_category_list.append(item)

            for item in self.categoryList:
                found = False
                for item2 in new_category_list:
                    if item2['name'] == item['name'] and \
                       item2['icon'] == item['icon'] and \
                       item2['tooltip'] == item['tooltip'] :
                        found = True
                        break
                if not found:
                    removed_category_list.append(item)

        category_icon_size = self.iconSize

        for item in removed_category_list:
            try:
                button = item['button']
                self.categoryList.remove(item)
                button.destroy()
                del item
            except Exception, e:
                print e

        if added_category_list:
            sorted_category_list = []
            for item in self.categoryList:
                try:
                    self.categoriesBox.remove(item['button'])
                    sorted_category_list.append( ( item['name'].upper(), item['button'] ) )
                except Exception, e:
                    print e

            for item in added_category_list:
                try:
                    if item["name"] == "附件":
                        item["button"] = CategoryButton( item["icon"], category_icon_size, [_("Accessories")], item["filter"] )
                    if item["name"] == "办公":
                        item["filter"] = _("Office")
                        item["button"] = CategoryButton( item["icon"], category_icon_size, [_("Office")], item["filter"] )
                    if item["name"] == "启动":
                        item["button"] = CategoryButton( item["icon"], category_icon_size, [_("StartUp")], item["filter"] )
                    if item["name"] == "游戏":
                        item["filter"] = _("Games")
                        item["button"] = CategoryButton( item["icon"], category_icon_size, [_("Games")], item["filter"] )
                    item["button"].set_name("ButtonApp")
                    self.categorybutton_list.append(item["button"])
                    item["button"].connect( "clicked", self.FilterAndClear, item["filter"] )
                    item["button"].connect( "focus-in-event", self.scrollItemIntoView )                                    #feng
                    item["button"].connect( "enter", self.showGoNext )
                    item["button"].connect( "leave", self.hideGoNext )
                    if server:
                        if item["name"] == "游戏" or item["name"] == "启动":
                            continue;
                    if serverx86:
                        if item["name"] == "游戏" or item["name"] == "启动":
                            continue;
                    if item["name"] == "启动":
                        continue;
                    item["button"].show()

                    self.categoryList.append( item )
                    sorted_category_list.append( ( item["name"].upper(), item["button"] ) )
                except Exception, e:
                    print e

            sorted_category_list.sort()

            for item in sorted_category_list:
                try:
                    self.categoriesBox.pack_start( item[1], False, False, 0 )
                except Exception, e:
                    print e

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
                    if item['desktop_file_path'] == item2['desktop_file_path']:
                        found = True
                        break
                if not found:
                    added_application_list.append(item)

            key = 0
            for item in self.applicationList:
                found = False
                for item2 in new_application_list:
                    if item['desktop_file_path'] == item2['desktop_file_path']:
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
        for item in self.applicationList:
            sorted_application_list.append( ( item["button"].appName.upper(), item["button"] ) )
        for item in added_application_list:
            button = self.build_application_launcher(item, menu_has_changed)
            if button:
                item["button"] = button
                item["button"].connect( "focus-in-event", self.scrollItemIntoView )                                    #feng
                if item["button"].appNoDisplay == True:
                    continue
                sorted_application_list.append( ( item["button"].appName.upper(), item["button"] ) )
                self.applicationList.append( item )

        sorted_application_list.sort()
        launcherNames = [] # Keep track of launcher names so we don't add them twice in the list.
        for item in sorted_application_list:
            launcherName = item[0]
            button = item[1]
            self.applicationsBox.pack_start( button, False, False, 0 )
            button.show()
            if launcherName in launcherNames:
                button.hide()
            else:
                launcherNames.append(launcherName)

        self.uncategorized_list = []
        for item in self.all_application_list:
            found = False
            for item2 in self.applicationList:
                if item['desktop_file_path'] == item2['desktop_file_path'] or os.path.basename(item['desktop_file_path']) == os.path.basename(item2['desktop_file_path']):
                    found = True
                    break
            if not found:
                self.uncategorized_list.append(copy.deepcopy(item))

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
                    if item['desktop_file_path'] == item2['desktop_file_path']:
                        found = True
                        break
                if not found:
                    added_uncategoried_list.append(item)

            key = 0
            for item in self.uncategoriedList:
                found = False
                for item2 in new_uncategoried_list:
                    if item['desktop_file_path'] == item2['desktop_file_path']:
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
            self.categoriesBox.remove( item["button"] )
            sorted_uncategoried_list.append( ( item["button"].appName.upper(), item["button"] ) )
        for item in added_uncategoried_list:
            button = self.build_application_launcher(item, menu_has_changed)
            if button:
                item["button"] = button
                item["button"].connect( "focus-in-event", self.scrollItemIntoView )                                    #feng
                sorted_uncategoried_list.append( ( item["button"].appName.upper(), item["button"] ) )
                self.uncategoriedList.append( item )

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

        sorted_all_application_list = []
        for item in self.all_application_list:
            button = self.build_application_launcher(item, False)
            if button:
                item["button"] = button
                item["button"].connect( "focus-in-event", self.scrollItemIntoView )                                    #feng
                sorted_all_application_list.append( ( item['button'].appName.upper(), item['button'] ) )
        sorted_all_application_list.sort()

        self.all_app_list = sorted_all_application_list

        self.rebuildLock = False

    def build_application_launcher(self, application, menu_has_changed):
        button = MenuApplicationLauncher( application['desktop_file_path'],
                                          self.iconSize, application["category"],
                                          True, 28,
                                          highlight = (True and menu_has_changed) )
        button.set_name("ButtonApp")
        if button.appName == "Vim" or button.appExec == "/usr/bin/policytool" or button.appExec == "ukui-notification-properties" or "ImageMagick" in button.appName or "Fcitx" in button.appName:
            return None
        if button.appExec:
            button.set_tooltip_text( button.getTooltip() )
            button.connect( "clicked", lambda w: self.ukuiMenuWin.hide() )
            button.connect( "button-press-event", self.menuPopup )
            button.desktop_file_path = application['desktop_file_path']
            if button.appName == "Eclipse":
                button.set_tooltip_text( " Eclipse\n 描述：Eclipse集成开发环境" )
            if button.appName == "Qt Creator":
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
                    app_list.append( { 'desktop_file_path': item.get_desktop_file_path(),
                                       'category': entry.name } )
                elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
                    find_applications_recursively(app_list, item, catName)

        for menu in self.menuFiles:
            root = menu.directory
            for entry in root.get_contents():
                if entry.get_type() == ukuimenu.TYPE_DIRECTORY and len(entry.get_contents()):
                    for item in entry.get_contents():
                        if item.get_type() == ukuimenu.TYPE_DIRECTORY:
                            find_applications_recursively(application_list, item, entry.name)
                        elif item.get_type() == ukuimenu.TYPE_ENTRY:
                            application_list.append({ 'desktop_file_path': item.get_desktop_file_path(),
                                                      'category': entry.name })
        return application_list

    def loadMenuFiles( self ):
        self.menuFiles = []
        for mainitems in [ "ukui-applications.menu", "ukui-settings.menu" ]:
            self.menuFiles.append( Menu( mainitems))

    def buildAppHistoryList( self ):
        self.loadMenuFiles()
        newApplicationsList = []

        try:
            def find_applications_recursively(app_list, directory, catName):
                for item in directory.get_contents():
                    if item.get_type() == ukuimenu.TYPE_ENTRY:
                        app_list.append( { "entry": item, "category": catName } )
                    elif item.get_type() == ukuimenu.TYPE_DIRECTORY:
                        find_applications_recursively(app_list, item, catName)

            for menu in self.menuFiles:
                directory = menu.directory
                for entry in directory.get_contents():
                    if entry.get_type() == ukuimenu.TYPE_DIRECTORY and len(entry.get_contents()):
                        for item in entry.get_contents():
                            if item.get_type() == ukuimenu.TYPE_DIRECTORY:
                                find_applications_recursively(newApplicationsList, item, entry.name)
                            elif item.get_type() == ukuimenu.TYPE_ENTRY:
                                newApplicationsList.append( { "entry": item, "category": entry.name } )
            return newApplicationsList
        except Exception, detail:
            print detail
            return None

    def buildCategoryList( self ):
        newCategoryList = [ { "name": _("All"), "icon": "stock_select-all", "tooltip": _("Show all applications"), "filter":"", "index": 0} ]

        num = 1

        for menu in self.menuFiles:
            for child in menu.directory.get_contents():
                if child.get_type() == ukuimenu.TYPE_DIRECTORY:
                    icon = str(child.icon)
                    newCategoryList.append( { "name": child.name, "icon": child.icon, "tooltip": child.name, "filter": child.name, "index": num } )
            num += 1
        return newCategoryList

    def destroy( self ):
        self.applicationsBox.destroy()
        self.categoriesBox.destroy()
        self.searchResultBox.destroy()
        self.searchEntry.destroy()

    def FilterAndClear( self, widget, category = None ):
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
        else:
            self.layout_applications_right.show()
            self.content_holder.set_size_request(575, self.windowHeight)
            widget.aState = True
            for widget1 in self.categorybutton_list:
                if widget1 != widget:
                    widget1.aState = False
                    widget1.rightbox.hide()
        self.Filter( widget, category)

    def Filter( self, widget, category = None ):
        self.filterTimer = None

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
                    i.filterCategory( category )

            for i in self.categoriesBox.get_children():
                i.released()
                i.set_relief( Gtk.ReliefStyle.NONE )
            widget.set_relief( Gtk.ReliefStyle.HALF )

        self.applicationsScrolledWindow.get_vadjustment().set_value( 0 )

    def menuPopup( self, widget, event ):
        button = MenuApplicationLauncher(widget.desktopFile, 32, "", True, highlight=(False))
        addedDesktop = False
        try:
            # Determine where the Desktop folder is (could be localized)
            import commands
            #sys.path.append('/usr/lib/ubuntu-ukui/common')
            from configobj import ConfigObj
            config = ConfigObj(home + "/.config/user-dirs.dirs")
            desktopDir = home + "/Desktop"
            tmpdesktopDir = config['XDG_DESKTOP_DIR']
            tmpdesktopDir = commands.getoutput("echo " + tmpdesktopDir)
            if os.path.exists(tmpdesktopDir):
                desktopDir = tmpdesktopDir
            # Copy the desktop file to the desktop
            if (os.path.exists(desktopDir + "/" + os.path.basename(widget.desktopFile))):
                addedDesktop = True
        except Exception, detail:
            print detail

        text = ""
        if event.button == 3:
            if (os.path.exists(GLib.get_home_dir() + "/.applet")):
                    f = open(GLib.get_home_dir() + "/.applet")
                    text = f.read()

            mTree = Gtk.Menu()
            mTree.set_name("myGtkLabel")
            launchMenuItem = Gtk.MenuItem(_("Open(_O)"))
            launchMenuItem.set_use_underline(True)
            separator1 = Gtk.SeparatorMenuItem()
            desktopMenuItem = Gtk.MenuItem(_("Add to desktop"))
            if addedDesktop:
                desktopMenuItem.set_sensitive(False)
            panelMenuItem = Gtk.MenuItem(_("Lock to panel(_L)"))
            panelMenuItem.set_use_underline(True)
            separator2 = Gtk.SeparatorMenuItem()
            favoriteMenuItem = Gtk.CheckMenuItem(_("Add to startup menu"))
            separator3 = Gtk.SeparatorMenuItem()
            uninstallMenuItem = Gtk.MenuItem(_("Uninstall"))
            deleteMenuItem = Gtk.MenuItem(_("Remove from list(_R)"))
            deleteMenuItem.set_use_underline(True)
            alldelete = Gtk.MenuItem(_("Remove all"))
            separator4 = Gtk.SeparatorMenuItem()
            propsMenuItem = Gtk.MenuItem(_("Property(_P)"))
            propsMenuItem.set_use_underline(True)

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
                if os.path.exists("/usr/bin/synaptic-pkexec"):
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
            propsMenuItem.connect( "activate", self.onPropsApp, widget)     #feng

            if button.appName == "Caja" or button.appName == "文件浏览器"or button.appExec == "ukui-terminal":
                favoriteMenuItem.set_sensitive(False)

            mTree.connect("deactivate", self.set_item_state, widget)

            self.ukuiMenuWin.stopHiding()
            widget.remove(widget.Align1)
            widget.Align1.remove(widget.HBox1)
            widget.viewport.show()
            widget.viewport.add(widget.HBox1)
            widget.add(widget.viewport)
            widget.set_sensitive(False)
            mTree.attach_to_widget(widget)
            mTree.popup(None, None, None, None, event.button, event.time)

    def set_item_state(self, menu, widget):
        button = MenuApplicationLauncher(widget.desktopFile, 32, "", True, highlight=(False))
        button.set_sensitive(True)
        widget.set_sensitive(True)
        widget.emit("leave")
        widget.viewport.remove(widget.HBox1)
        widget.remove(widget.viewport)
        widget.Align1.add(widget.HBox1)
        widget.add(widget.Align1)

    def add_to_desktop(self, widget, desktopEntry):
        try:
            # Determine where the Desktop folder is (could be localized)
            import commands
            #sys.path.append('/usr/lib/ubuntu-ukui/common')
            from configobj import ConfigObj
            config = ConfigObj(home + "/.config/user-dirs.dirs")
            desktopDir = home + "/Desktop"
            tmpdesktopDir = config['XDG_DESKTOP_DIR']
            tmpdesktopDir = commands.getoutput("echo " + tmpdesktopDir)
            if os.path.exists(tmpdesktopDir):
                desktopDir = tmpdesktopDir
            # Copy the desktop file to the desktop
            os.system("cp \"%s\" \"%s/\"" % (desktopEntry.desktopFile, desktopDir))
            os.system("chmod a+rx %s/*.desktop" % (desktopDir))
        except Exception, detail:
            print detail

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

    def onAddToFavorites( self, menu, widget  ):
        self.favoritesAdd( self.favoritesBuildLauncher( widget.desktopFile ) )
        self.updateHistoryBox()

    def onRemoveFromFavorites( self, menu, widget ):
        self.favoritesRemoveLocation( widget.desktopFile )
        self.updateHistoryBox()

    def favoritesRemoveLocation( self, location ):
        for fav in self.favorites:
            if fav.type == "location" and fav.desktopFile == location:
                self.favoritesRemove( fav.position )
        self.updateHistoryBox()

    def favoritesAdd( self, favButton, position = -1 ):
        if favButton:
            favButton.set_name("ButtonApp")
            favButton.position = len( self.favorites )
            self.favorites.append( favButton )
            for fav in self.favorites:
                self.favoritesBox.remove( fav )
                self.favoritesPositionOnGrid( fav )

            favButton.connect( "drag-data-received", self.onFavButtonDragReorder )
            favButton.drag_dest_set( Gtk.DestDefaults.MOTION | Gtk.DestDefaults.HIGHLIGHT | Gtk.DestDefaults.DROP, self.fromFav, Gdk.DragAction.COPY )
            favButton.connect( "drag-data-get", self.onFavButtonDragReorderGet )
            favButton.drag_source_set( Gdk.ModifierType.BUTTON1_MASK, self.toFav, Gdk.DragAction.COPY )

            if position >= 0:
                self.favoritesReorder( favButton.position, position )

            self.favoritesSave()
            self.favoritesBox.resize( self.favoritesGetNumRows(), self.favCols )
        self.updateHistoryBox()

    def favoritesGetNumRows( self ):
        rows = 0
        col = 0
        for fav in self.favorites:
            if  ( fav.type == "separator" or fav.type == "space" ) and col != 0:
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

    def onPropsApp( self, menu, widget ):
        button = MenuApplicationLauncher(widget.desktopFile, 32, "", True, highlight=(False))
        self.builder1 = Gtk.Builder()
        self.builder1.add_from_file( os.path.join( '/', 'usr', 'share', 'ukui-menu',  'plugins', 'property.glade' ) )
        dialog = self.builder1.get_object("dialog1")
        dialog.set_name("myGtkLabel")
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
            self.entry_comment.set_text("Eclipse集成开发环境")
        if button.appName == "Qt Creator":
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
                 for line in lines:
                     word = line.strip().split(':')
                     if word[0] == delappname:
                         continue
                     recentApp[word[0]] = int(word[len(word) - 1])
                 f.close()
                 f = open(GLib.get_home_dir() + "/.recentAppLog", 'w')
             else:
                 f = open(GLib.get_home_dir() + "/.recentAppLog", 'w')

             srApp = sorted(recentApp.iteritems(), key = operator.itemgetter(1), reverse=True)
             for i in range(len(srApp)):
                 f.write(srApp[i][0] + ":" + str(srApp[i][1]) + "\n")
             f.close()
             self.updateHistoryBox()
        except Exception, detail:
            print detail

    def changeTab( self, tabNum, clear = True ):
        notebook = self.builder.get_object( "notebook_left" )
        if tabNum == 0:
            self.favappbutton.grab_focus()
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

    def onAddToStartup( self, menu, widget ):
        widget.addToStartup()

    def showGoNext( self, widget ):
        widget.rightbox.show_all()

    def hideGoNext( self, widget ):
        if not widget.aState:
            widget.rightbox.hide()

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
        except Exception, e:
            print e
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
            for appname in self.appHistoryList:
                if desktopPath:
                    button = MenuApplicationLauncher(desktopPath, 32, "", True, highlight=(False))
                    button.set_name("ButtonApp")
                    if button.appName.lower() == appname["entry"].name.lower():
                        appName = appname["entry"].name.lower()
                        found = True
                        break
                    if desktopPath == appname["entry"].get_desktop_file_path():
                        appName = appname["entry"].name.lower()
                        found = True
                        break
                    if tempName.lower() == appname["entry"].name.lower():
                        appName = appname["entry"].name.lower()
                        found = True
                        break
                if viewName:
                    if viewName.lower() == appname["entry"].name.lower():
                        appName = appname["entry"].name.lower()
                        found = True
                        break
        if not found:
            return
        #read and write recentAppLog file
        if os.path.exists(GLib.get_home_dir() + "/.recentAppLog"):
            f = open(GLib.get_home_dir() + "/.recentAppLog", 'r')
            lines = f.readlines()
            for line in lines:
                word = line.strip().split(':')
                recentApp[word[0]] = int(word[len(word) - 1])
            f.close()
            f = open(GLib.get_home_dir() + "/.recentAppLog", 'w')
        else:
            f = open(GLib.get_home_dir() + "/.recentAppLog", 'w')

        recentApp[appName] = recentApp.get(appName,0) + 1
        srApp = sorted(recentApp.iteritems(), key = operator.itemgetter(1), reverse=True)
        for i in range(len(srApp)):
            f.write(srApp[i][0] + ":" + str(srApp[i][1]) + "\n")
        f.close()
        self.updateHistoryBox()

    def updateHistoryBox( self ):
        for child in self.recentAppBox:
            child.destroy()
        historyDisplayNum = 10
        self.currentHisCount = 1

        try:
            applicationsFile = open(self.favoritesPath, "r")
            applicationsList = applicationsFile.readlines()
            applicationsFile.close()
            self.favAppList = applicationsList
            f = open(GLib.get_home_dir() + "/.recentAppLog", 'r')
            lines = f.readlines()
            for line in lines:
                word = line.strip().split(':')
                found = False
                for appname in self.appHistoryList:
                    findDesktop = False
                    for tempdesktop in self.favAppList:
                        temp = tempdesktop.split(":")[1]
                        if appname["entry"].get_desktop_file_path() in temp or os.path.basename(appname["entry"].get_desktop_file_path()) in temp or os.path.basename(temp) in appname["entry"].get_desktop_file_path():
                            findDesktop = True
                    if word[0].lower() == appname["entry"].name.lower() and findDesktop == False: 
                        button = MenuApplicationLauncher( appname["entry"].get_desktop_file_path(), 32, "", True, highlight=(False) )
                        if button.appName == "Caja" or button.appExec == "ukui-terminal":
                            continue
                        if button.appExec:
                            button.set_tooltip_text( button.getTooltip() )
                            button.connect( "clicked", lambda w: self.ukuiMenuWin.hide() )
                            button.isRecentApp = True        
                            button.connect( "button-press-event", self.menuPopup )

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
            f.close()
        except Exception, e:
            print e

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
