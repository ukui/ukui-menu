#! /usr/bin/python
# -*- coding: utf-8 -*-

# Copyright (C) 2007-2014 Clement Lefebvre <root@linuxmint.com>
# Copyright (C) 2015-2016 Martin Wimpress <code@ubuntu-ukui.org>
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

__VERSION__='6.0.1'

import gc
import gi
import gettext
import os
import platform
import subprocess
import sys
import traceback
import signal

gi.require_version("Gtk", "3.0")
gi.require_version('UkuiPanelApplet', '4.0')

from ctypes import *
from gi.repository import Gtk, GLib, GdkPixbuf, Gdk, GObject
from gi.repository import UkuiPanelApplet
from gi.repository import Gio

try:
    import xdg.Config
    import ukui_menu.keybinding as keybinding
    import ukui_menu.pointerMonitor as pointerMonitor
except Exception, e:
    print e
    sys.exit( 1 )

signal.signal(signal.SIGINT, signal.SIG_DFL)
GObject.threads_init()

# i18n
gettext.install("ukui-menu", "/usr/share/locale")

NAME = _("Menu")

xdg.Config.setWindowManager('MATE')

from ukui_menu.execute import *

class MainWindow( object ):
    """This is the main class for the application"""

    def __init__( self, toggleButton, keybinder ):
        self.keybinder = keybinder
        self.data_path = os.path.join( '/', 'usr', 'share', 'ukui-menu' )
        self.icon = "/usr/share/ukui-menu/icons/ukui-logo.svg"

        self.de = "mate"

        self.toggle = toggleButton
        # Load UI file and extract widgets
        builder = Gtk.Builder()
        builder.add_from_file(os.path.join( self.data_path, "ukui-menu.glade" ))
        self.window     = builder.get_object( "mainWindow" )
        self.eventbox   = builder.get_object( "eventbox1" )
        self.paneholder = builder.get_object( "paneholder" )
        self.border     = builder.get_object( "border" )

        builder.connect_signals(self)

        self.borderwidth = 1
        self.border.set_padding( self.borderwidth, self.borderwidth, self.borderwidth, self.borderwidth )
        self.eventbox.set_name("EventBox")
        self.window.set_name("Window")

        self.window.connect( "key-press-event", self.onKeyPress )
        self.window.connect( "focus-in-event", self.onFocusIn )
        self.loseFocusId = self.window.connect( "focus-out-event", self.onFocusOut )
        self.loseFocusBlocked = False

        self.offset = 0

        self.window.stick()

        self.PopulatePlugins()

    def on_window1_destroy (self, widget, data=None):
        Gtk.main_quit()
        sys.exit(0)


    def PopulatePlugins( self ):
        PluginPane = Gtk.EventBox()
        PluginPane.show()
        PaneLadder = Gtk.Box( orientation=Gtk.Orientation.VERTICAL )
        PluginPane.add( PaneLadder )
        ImageBox = Gtk.EventBox()
        ImageBox.show()

        self.plugins = {}
        self.pluginlist = {'menu'}

        for plugin in self.pluginlist:
            if plugin in self.plugins:
                print u"Duplicate plugin in list: ", plugin
                continue

        if plugin == "menu":
            try:
                plugin_module = 'ukui_menu.plugins.{plugin}'.format(plugin=plugin)
                exec("from {plugin_module} import pluginclass".format(plugin_module=plugin_module))
                # If no parameter passed to plugin it is autonomous
                if pluginclass.__init__.func_code.co_argcount == 1:
                    MyPlugin = pluginclass()
                else:
                    # pass ukuiMenu and togglebutton instance so that the plugin can use it
                    MyPlugin = pluginclass(self, self.toggle)

                #if ndddot MyPlugin.icon:
                #    MyPlugin.icon = "ukui-logo-icon.png"
            except Exception, e:
                print u"Unable to load " + plugin + " plugin :-("
                print e
            MyPlugin.content_holder.show()

            VBox1 = Gtk.Box( orientation=Gtk.Orientation.VERTICAL )
            VBox1.show()
            #Add plugin to Plugin Box under heading button
            MyPlugin.content_holder.reparent( VBox1 )

            #Add plugin to main window
            PaneLadder.pack_start( VBox1 , True, True, 0)
            PaneLadder.show()

            if MyPlugin.window:
                MyPlugin.window.destroy()

            try:
                if hasattr( MyPlugin, 'do_plugin' ):
                    MyPlugin.do_plugin()

                heightPath = os.path.join(GLib.get_home_dir(), ".windowHeight")
                if os.path.exists(heightPath):
                    f = open(heightPath, "r")
                    lines = f.readlines()
                    length = lines[0]
                    MyPlugin.windowHeight = int(length)
                    f.close()
                MyPlugin.content_holder.set_size_request(345, MyPlugin.windowHeight )
            except Exception, e:
                print e

            self.plugins[plugin] = MyPlugin

        self.paneholder.pack_start( ImageBox, False, False, 0 )
        self.paneholder.pack_start( PluginPane, False, False, 0 )

    def getDefaultStyle( self ):
        widget = Gtk.EventBox()
        widget.show()
        return Gtk.rc_get_style(widget)

    def RegenPlugins( self, *args, **kargs ):
        for item in self.paneholder:
            item.destroy()

        for plugin in self.plugins.values():
            if hasattr( plugin, "destroy" ):
                plugin.destroy()

        try:
            del plugin
        except:
            pass

        try:
            del self.plugins
        except:
            pass

        gc.collect()

        self.PopulatePlugins()

    def onKeyPress( self, widget, event ):
        if event.keyval == Gdk.KEY_Escape:
            self.hide()
            return True
        return False

    def show( self ):
        for plugin in self.plugins.values():
            if hasattr( plugin, "onShowMenu" ):
                plugin.onShowMenu()

        self.window.present()

        self.window.get_window().focus( Gdk.CURRENT_TIME )

        for plugin in self.plugins.values():
            if hasattr( plugin, "changeTab" ):
                plugin.changeTab( 0 )

        Gdk.flush()

    def hide( self ):
        for plugin in self.plugins.values():
            if hasattr( plugin, "onHideMenu" ):
                plugin.onHideMenu()

        self.window.hide()

    def onFocusIn( self, *args ):
        if self.loseFocusBlocked:
            self.window.handler_unblock( self.loseFocusId )
            self.loseFocusBlocked = False

        return False

    def onFocusOut( self, *args):
        if self.window.get_visible():
            self.hide()
        return False

    def stopHiding( self ):
        if not self.loseFocusBlocked:
            self.window.handler_block( self.loseFocusId )
            self.loseFocusBlocked = True

class MenuWin( object ):
    def __init__( self, applet, iid ):
        self.data_path = os.path.join('/','usr','share','ukui-menu')
        self.applet = applet
        #self.loadSettings()

        self.createPanelButton()
        self.applet.connect( "button-press-event", self.showMenu )
        self.applet.connect( "enter-notify-event", self.enter_notify )
        self.applet.connect( "leave-notify-event", self.leave_notify )
        GLib.idle_add( self.InitMenu )

    def InitMenu( self ):
        self.keybinder = keybinding.GlobalKeyBinding()
        self.hotkeyText = "Super_L"
        self.applet.set_flags( UkuiPanelApplet.AppletFlags.EXPAND_MINOR )
        self.mainwin = MainWindow( self.button_box, self.keybinder )
        self.mainwin.window.connect( "map-event", self.onWindowMap )
        self.mainwin.window.connect( "unmap-event", self.onWindowUnmap )
        self.mainwin.window.connect( "realize", self.onRealize )
        self.mainwin.window.connect( "size-allocate", lambda *args: self.positionMenu() )

        self.bind_hot_key()
        self.applet.set_can_focus(False)

        self.pointerMonitor = pointerMonitor.PointerMonitor()
        self.pointerMonitor.connect("activate", self.onPointerOutside)

    def onWindowMap( self, *args ):
        self.applet.get_style_context().set_state( Gtk.StateFlags.SELECTED )
        self.keybinder.set_focus_window( self.mainwin.window.get_window() )
        return False

    def onWindowUnmap( self, *args ):
        self.applet.get_style_context().set_state( Gtk.StateFlags.NORMAL )
        self.keybinder.set_focus_window()
        return False

    def onRealize( self, *args):
        self.pointerMonitor.addWindowToMonitor( self.mainwin.window.get_window() )
        self.pointerMonitor.addWindowToMonitor( self.applet.get_window() )
        self.pointerMonitor.start()
        return False

    def onPointerOutside(self, *args):
        self.mainwin.hide()
        return True

    def onBindingPress(self, binder):
        self.toggleMenu()
        return True

    def enter_notify(self, applet, event):
        self.do_image(True)

    def leave_notify(self, applet, event):
	# Hack for mate-panel-test-applets focus issue (this can be commented)
        if event.state & Gdk.ModifierType.BUTTON1_MASK and applet.get_style_context().get_state() & Gtk.StateFlags.SELECTED:
            if event.x >= 0 and event.y >= 0 and event.x < applet.get_window().get_width() and event.y < applet.get_window().get_height():
                self.mainwin.stopHiding()

        self.do_image(False)

    def do_image(self, saturate):
        if saturate:
            pixbuf = GdkPixbuf.Pixbuf.new_from_file("/usr/share/ukui-menu/icons/start-hover.png")
        else:
            pixbuf = GdkPixbuf.Pixbuf.new_from_file("/usr/share/ukui-menu/icons/start.png")
        self.button_icon.set_from_pixbuf(pixbuf)

    def createPanelButton( self ):
        self.button_icon = Gtk.Image.new_from_file( "/usr/share/ukui-menu/icons/start.png" )
        self.button_icon.set_tooltip_text(_("Start"))
        self.button_box = Gtk.Box()
        self.button_box.set_size_request(50, -1)
        self.button_box.pack_start( self.button_icon , True, True, 0)
        self.button_icon.set_padding( 0, 0 )

        self.button_box.set_homogeneous( False )
        self.button_box.show_all()

        self.applet.add( self.button_box )
        self.applet.set_background_widget( self.applet )

    def bind_hot_key (self):
        try:
            if self.hotkeyText != "":
                self.keybinder.grab( self.hotkeyText )
            self.keybinder.connect("activate", self.onBindingPress)
            self.keybinder.start()
            # Binding menu to hotkey
            print "Binding to Hot Key: " + self.hotkeyText

        except Exception, cause:
            print "** WARNING ** - Menu Hotkey Binding Error"
            print "Error Report :\n", str(cause)
            pass

    def showAboutDialog( self, action, userdata = None ):
        about = Gtk.AboutDialog()
        about.set_name("Ukui Menu")
        about.set_version(__VERSION__)
        about.set_comments( _("Startup Menu") )
        about.set_logo( GdkPixbuf.Pixbuf.new_from_file("/usr/share/ukui-menu/icons/ukui-logo.svg") )
        about.connect( "response", lambda dialog, r: dialog.destroy() )
        about.show()

    def showMenu( self, widget=None, event=None ):
        if event == None or event.button == 1:
            self.toggleMenu()
        # show right click menu
        elif event.button == 3:
            self.create_menu()
        # allow middle click and drag
        elif event.button == 2:
            self.mainwin.hide()

    def toggleMenu( self ):
        if self.applet.get_style_context().get_state() & Gtk.StateFlags.SELECTED:
            self.mainwin.hide()
        else:
            self.positionMenu()
            self.mainwin.show()

    def positionMenu( self ):
        # Get our own dimensions & position
        ourWidth  = self.mainwin.window.get_size()[0]
        ourHeight = self.mainwin.window.get_size()[1] + self.mainwin.offset

        # Get the dimensions/position of the widgetToAlignWith
        entryX = self.applet.get_window().get_origin().x
        entryY = self.applet.get_window().get_origin().y

        entryWidth, entryHeight =  self.applet.get_allocation().width, self.applet.get_allocation().height
        entryHeight = entryHeight + self.mainwin.offset

        # Get the screen dimensions
        screenHeight = Gdk.Screen.height()
        screenWidth = Gdk.Screen.width()
        if self.applet.get_orient() == UkuiPanelApplet.AppletOrient.UP or self.applet.get_orient() == UkuiPanelApplet.AppletOrient.DOWN:
            if entryX + ourWidth < screenWidth or  entryX + entryWidth / 2 < screenWidth / 2:
            # Align to the left of the entry
                newX = entryX
            else:
                # Align to the right of the entry
                newX = entryX + entryWidth - ourWidth

            if entryY + entryHeight / 2 < screenHeight / 2:
                # Align to the bottom of the entry
                newY = entryY + entryHeight
            else:
                newY = entryY - ourHeight
        else:
            if entryX + entryWidth / 2 < screenWidth / 2:
                # Align to the left of the entry
                newX = entryX + entryWidth
            else:
                # Align to the right of the entry
                newX = entryX - ourWidth

            if entryY + ourHeight < screenHeight or entryY + entryHeight / 2 < screenHeight / 2:
                # Align to the bottom of the entry
                newY = entryY
            else:
                newY = entryY - ourHeight + entryHeight
        # -"Move window"
        self.mainwin.window.move( newX, newY )

    # this callback is to create a context menu
    def create_menu(self):
        action_group = Gtk.ActionGroup(name="context-menu")
        action = Gtk.Action(name="UkuiMenuReload", label=_("Reload plugins"), tooltip=None, stock_id="gtk-refresh")
        action.connect("activate", self.mainwin.RegenPlugins)
        action_group.add_action(action)
        action = Gtk.Action(name="UkuiMenuAbout", label=_("About"), tooltip=None, stock_id="gtk-about")
        action.connect("activate", self.showAboutDialog)
        action_group.add_action(action)
        action_group.set_translation_domain ("ukui-menu")

        xml = os.path.join( self.data_path, "popup.xml" )
        self.applet.setup_menu_from_file(xml, action_group)

def applet_factory( applet, iid, data ):
    MenuWin( applet, iid )
    applet.show()
    return True

def quit_all(widget):
    Gtk.main_quit()
    sys.exit(0)

UkuiPanelApplet.Applet.factory_main("UkuiMenuAppletFactory", True,
                                    UkuiPanelApplet.Applet.__gtype__,
                                    applet_factory, None)
