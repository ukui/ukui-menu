#!/usr/bin/env python3
# coding: utf-8

# Copyright (C) 2007-2014 Clement Lefebvre <root@linuxmint.com>
# Copyright (C) 2015-2016 Martin Wimpress <code@ubuntu-mate.org>
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

from gi.repository import Gtk, Gdk, GObject

try:
    import sys
    import string
    import os
    import subprocess
    import threading
    import tempfile
    import gettext
    import dbus

except Exception as detail:
    print (detail)
    sys.exit(1)

from subprocess import Popen

# i18n
gettext.install("ukui-menu", "/usr/share/locale")

class RemoveExecuter(threading.Thread):

    def __init__(self, package):
        threading.Thread.__init__(self)
        self.package = package
        self.iface = self.init_dbus_ifaces()

    def run(self):
        removePackages = string.split(self.package)
        for pkg in removePackages:
            self.iface.remove(pkg)

    def init_dbus_ifaces(self):
        try:
            bus = dbus.SystemBus()
        except:
            print ("could not initiate dbus")
            return False

        try:
            obj = bus.get_object("com.ubuntukylin.softwarecenter","/")
            iface = dbus.Interface(obj, "com.ubuntukylin.softwarecenter")
            return iface
        except:
            print ("Get dbus failed")

class ukuiRemoveWindow:

    def __init__(self, desktopFile):
        self.desktopFile = desktopFile
        (status, output) = subprocess.getstatusoutput("dpkg -S " + self.desktopFile)
        package = output[:output.find(":")]
        if status != 0:
            warnDlg = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING, Gtk.ButtonsType.YES_NO, _("This application has been removed. Are you sure remove the menu form the startup menu?"))
            warnDlg.vbox.set_spacing(10)
            response = warnDlg.run()
            if response == Gtk.ResponseType.YES :
                print (("removing '%s'" % self.desktopFile))
                os.system("rm -f '%s'" % self.desktopFile)
                os.system("rm -f '%s.desktop'" % self.desktopFile)
            warnDlg.destroy()
            sys.exit(0)

        warnDlg = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING, Gtk.ButtonsType.OK_CANCEL, _("The following packages will be removed:"))
        warnDlg.vbox.set_spacing(10)

        treeview = Gtk.TreeView()
        column1 = Gtk.TreeViewColumn(_("Packages will be removed"))
        renderer = Gtk.CellRendererText()
        column1.pack_start(renderer, False)
        column1.add_attribute(renderer, "text", 0)
        treeview.append_column(column1)

        model = Gtk.ListStore(str)
        dependenciesString = subprocess.getoutput("apt-get -s -q remove " + package + " | grep Remv")
        dependencies = string.split(dependenciesString, "\n")
        for dependency in dependencies:
            dependency = dependency.replace("Remv ", "")
            model.append([dependency])
        treeview.set_model(model)
        treeview.show()

        scrolledwindow = Gtk.ScrolledWindow()
        scrolledwindow.set_shadow_type(Gtk.ShadowType.ETCHED_OUT)
        scrolledwindow.set_size_request(300, 150)
        scrolledwindow.set_policy(Gtk.PolicyType.AUTOMATIC, Gtk.PolicyType.AUTOMATIC)
        scrolledwindow.add(treeview)
        scrolledwindow.show()

        warnDlg.get_content_area().add(scrolledwindow)

        response = warnDlg.run()
        if response == Gtk.ResponseType.OK :
            executer = RemoveExecuter(package)
            executer.start()
        elif response == Gtk.ResponseType.CANCEL :
            sys.exit(0)
        warnDlg.destroy()

        Gtk.main()

if __name__ == "__main__":
    mainwin = ukuiRemoveWindow(sys.argv[1])
    Gtk.main()

