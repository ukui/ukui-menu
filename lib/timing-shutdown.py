#! /usr/bin/python3
# -*- coding: utf-8 -*-

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

import schedule
import time 
import datetime
import signal
import os
import sys
import gettext
import threading
import fcntl

import gi
gi.require_version("Gtk", "3.0")

from gi.repository import Gtk, Gdk, Gio, GLib, GObject, GdkPixbuf
from gi.repository.GdkPixbuf import Pixbuf

# i18n
gettext.install("ukui-menu", "/usr/share/locale")

statusIcon = None

class JobThread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.__running = threading.Event()
        self.__running.set()
        self.thread_stop = False

    def run(self):
        while not self.thread_stop: 
            self.timingShutdown()

    def stop(self):
        self.thread_stop = True

    def timingShutdown(self):
        time.sleep(1)
        schedule.run_pending() 

class ShutdownDialog(object):
    def __init__(self, app):
        self.settings = Gio.Settings.new("org.ukui.ukui-menu")

        builder = Gtk.Builder()
        builder.set_translation_domain('ukui-menu')
        builder.add_from_file( os.path.join( '/', 'usr', 'share', 'ukui-menu',  'plugins', 'shutdown.ui' ) )
        self.dialog = builder.get_object("dialog1")
        self.dialog.connect("delete-event", self.hide_dialog)
        self.label2 = builder.get_object("label2")
        self.label_time = builder.get_object("label_time")
        self.button_ok = builder.get_object("button_ok")
        self.button_ok.connect("button-press-event", self.on_button_ok_clicked)
        self.button_ok.grab_focus()
        self.button_cancel = builder.get_object("button_cancel")
        self.button_cancel.connect("button-press-event", self.on_button_cancel_clicked)

        self.combobox = builder.get_object("combobox")
        self.combobox.connect("changed", self.column_changed)

        self.spin_hour = builder.get_object("spin_hour")
        self.spin_min = builder.get_object("spin_min")
        self.set_spin_bt_time()

        sequence = self.settings.get_string("shutdown-sequence")
        self.combobox.set_active_id(sequence)

        self.dialog.set_title(_("Timing Shutdown"))
        self.dialog.set_application(app)

        self.jobthread = JobThread()
        self.jobthread.start()
        self.set_shutdown_seq(sequence)
        self.update_widget_state()
        
        self.dialog.show()

    def shutdownChanged(self, settings, key, args = None):
        schedule.clear()
        time.sleep(1)
        self.update_widget_state()
        os.system("ukui-session-save --shutdown-dialog &")

    def shutdown(self, onlythistime = False): 
        if onlythistime:
            settings = Gio.Settings.new("org.ukui.ukui-menu")
            settings.connect("changed::timing-shutdown", self.shutdownChanged)
            settings.set_boolean("timing-shutdown", False)
            settings.set_string("shutdown-sequence", "1")
            self.combobox.set_active_id("1")
        else:
            os.system("ukui-session-save --shutdown-dialog &")

    def compute_time(self, shutdowntime):
        sequence = self.settings.get_string("shutdown-sequence")
        if sequence == "1":#never
            return

        countdown = None
        curtime = datetime.datetime.now()
        day = curtime.day
        hour, minute, second = shutdowntime
        settime = curtime.replace(hour=hour, minute=minute, second=second)
        
        if sequence == "2" or sequence == "3":#only this time or everyday
            if settime < curtime:
                settime = curtime.replace(day=(day+1), hour=hour, minute=minute, second=second)
                countdown = settime - curtime
            else:
                countdown = settime - curtime

        if sequence == "4":#monday to friday
            if curtime.weekday() == 5:#saturday
                settime = curtime.replace(day=(day+2), hour=hour, minute=minute, second=second)
                countdown = settime - curtime 
            elif curtime.weekday() == 6:#sunday
                settime = curtime.replace(day=(day+1), hour=hour, minute=minute, second=second)
                countdown = settime - curtime 
            else:#monday to friday
                if settime < curtime:
                    settime = curtime.replace(day=(day+1), hour=hour, minute=minute, second=second)
                    countdown = settime - curtime
                else:
                    countdown = settime - curtime

        if sequence == "5":#saturday to sunday
            if curtime.weekday() == 5:#saturday
                if settime < curtime:
                    settime = curtime.replace(day=(day+1), hour=hour, minute=minute, second=second)
                    countdown = settime - curtime
                else:
                    countdown = settime - curtime
            if curtime.weekday() == 6:#sunday
                if settime < curtime:
                    settime = curtime.replace(day=(day+6), hour=hour, minute=minute, second=second)
                    countdown = settime - curtime
                else:
                    countdown = settime - curtime

            for weekday in range(5):#monday to friday
                if curtime.weekday() == weekday:
                    settime = curtime.replace(day=(day+5-weekday), hour=hour, minute=minute, second=second)
                    countdown = settime - curtime 

        time_str = str(countdown)
        if "day," in time_str:
            time_str = time_str.replace("day,", _("day"))
        if "days," in time_str:
            time_str = time_str.replace("days,", _("days"))

        return time_str

    def update_label(self, settime):
        time_str = self.compute_time(settime)
        self.label_time.set_text(time_str)
        statusIcon.set_tooltip_text(_("Time to Poweroff:  ") + time_str)

    def set_spin_bt_time(self):
        shutdowntime = self.settings.get_string("shutdown-time")
        if shutdowntime == "":
            curtime = datetime.datetime.now()
            self.spin_hour.set_value(curtime.hour)
            self.spin_min.set_value(curtime.minute)
        else:
            hour = shutdowntime.split(":")[0]
            minute = shutdowntime.split(":")[1]
            self.spin_hour.set_value(int(hour))
            self.spin_min.set_value(int(minute))

    def column_changed(self, widget, user_data = None):
        active_id = self.combobox.get_active_id()

    def hide_dialog(self, widget = None, user_data = None):
        self.dialog.hide()
        return True

    def set_shutdown_seq(self, sequence):
        hour = int(self.spin_hour.get_value())
        minute = int(self.spin_min.get_value())
        settime = str(hour) + ":" + str(minute)

        if sequence == "1":
            self.settings.set_boolean("timing-shutdown", False)
            return
        if sequence == "2":
            schedule.every().day.at(settime).do(self.shutdown, True) 
            self.settings.set_boolean("timing-shutdown", True)
        if sequence == "3":
            schedule.every().day.at(settime).do(self.shutdown) 
            self.settings.set_boolean("timing-shutdown", True)
        if sequence == "4":
            schedule.every().monday.at(settime).do(self.shutdown) 
            schedule.every().tuesday.at(settime).do(self.shutdown) 
            schedule.every().wednesday.at(settime).do(self.shutdown) 
            schedule.every().thursday.at(settime).do(self.shutdown) 
            schedule.every().friday.at(settime).do(self.shutdown) 
            self.settings.set_boolean("timing-shutdown", True)
        if sequence == "5":
            schedule.every().saturday.at(settime).do(self.shutdown) 
            schedule.every().sunday.at(settime).do(self.shutdown) 
            self.settings.set_boolean("timing-shutdown", True)

        self.settings.set_string("shutdown-time", settime)

    def update_widget_state(self):
        timingshutdown = self.settings.get_boolean("timing-shutdown")
        if timingshutdown:
            self.spin_hour.set_sensitive(False)
            self.spin_min.set_sensitive(False)
            self.combobox.set_sensitive(False)
            hour = int(self.spin_hour.get_value())
            minute = int(self.spin_min.get_value())
            settime = str(hour) + ":" + str(minute)
            self.update_label((hour, minute, 0))
            schedule.every(1).seconds.do(self.update_label, (hour, minute, 0)) 
            self.label2.show()
            self.button_ok.set_sensitive(False)
        else:
            self.spin_hour.set_sensitive(True)
            self.spin_min.set_sensitive(True)
            self.combobox.set_sensitive(True)
            self.label2.hide()
            self.label_time.set_text("")
            self.button_ok.set_sensitive(True)

    def on_button_ok_clicked(self, widget, event):
        sequence = self.combobox.get_active_id()
        self.set_shutdown_seq(sequence)
        self.settings.set_string("shutdown-sequence", sequence)
        self.update_widget_state()

    def on_button_cancel_clicked(self, widget, event):
        self.settings.set_boolean("timing-shutdown", False)
        self.settings.set_string("shutdown-sequence", "1")
        self.settings.set_string("shutdown-time", "")
        schedule.clear()
        self.spin_hour.set_sensitive(True)
        self.spin_min.set_sensitive(True)
        self.combobox.set_sensitive(True)
        self.button_ok.set_sensitive(True)
        self.label_time.set_text("")
        self.label2.hide()

def main_quit(signum, user_data):
#    jobthread.stop()
#    Gtk.main_quit()
    sys.exit(0)

def quit_dialog(widget, user_data):
    dialog = user_data
    schedule.clear()
    dialog.jobthread.stop()
    sys.exit(0)
        
def showDialog(widget, event, user_data = None):
    dialog = user_data
    if event.button == 1:
        if dialog.dialog.get_style_context().get_state() == Gtk.StateFlags.DIR_LTR:
            dialog.dialog.hide()
        else:
            dialog.dialog.grab_focus()
            dialog.dialog.present()
    if event.button == 3:
        mTree = Gtk.Menu()
        closeItem = Gtk.ImageMenuItem.new_with_mnemonic(_("Quit(_Q)"))
        image = Gtk.Image.new_from_icon_name("gtk-close", Gtk.IconSize.MENU)
        closeItem.set_image(image)
        mTree.append(closeItem)
        closeItem.connect( "activate", quit_dialog, dialog)
        mTree.show_all()
        mTree.popup(None, None, Gtk.StatusIcon.position_menu, widget, event.button, event.time)

def command_line(app, cmdline, user_data = None):
    statusIcon, argv = user_data

    listwins = app.get_windows()
    if listwins:
        for win in listwins:
            win.grab_focus()
            win.present()
    else:
        dialog = ShutdownDialog(app)
        if statusIcon:
            statusIcon.set_visible(True)
            statusIcon.connect("button-press-event", showDialog, dialog)
        if argv == "-s":
            dialog.dialog.hide()

    return 0

class FLOCK(object):
    def __init__(self, name):
        self.fobj = open(name, 'w')
        self.fd = self.fobj.fileno()

    def lock(self):
        try:
            fcntl.lockf(self.fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
            return True
        except:
            return False


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "-s":
        settings = Gio.Settings.new("org.ukui.ukui-menu")
        timingshutdown = settings.get_boolean("timing-shutdown")
        if not timingshutdown:
            sys.exit(0)

    app = Gtk.Application.new("org.gtk.ukui-menu", Gio.ApplicationFlags.HANDLES_COMMAND_LINE)
    app.register(None)

    locker = FLOCK(os.path.expanduser('~') + "/.shutdowndialog.lock")
    status = locker.lock()
    if status:
        statusIcon = Gtk.StatusIcon.new_from_file ("/usr/share/ukui-menu/icons/timingshutdown.svg")
        if len(sys.argv) > 1:
            app.connect("command-line", command_line, (statusIcon, sys.argv[1]))
        else:
            app.connect("command-line", command_line, (statusIcon, None))
    else:
        app.connect("command-line", command_line)

    signal.signal(signal.SIGINT, main_quit)
    app.run()
