#!/usr/bin/python3

import os.path, sys, subprocess, os, re, tempfile, stat
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

from systeminfo import SystemInfo
from send import Send

sys.path.append("..")

have_display = os.environ.get('DISPLAY') or os.environ.get('WAYLAND_DISPLAY')

class Feedback:
    def w(self, widget):
        return self.widgets.get_object(widget)

    def __init__(self):
        self.widgets = Gtk.Builder()
        # self.widgets.add_from_file('/usr/share/ukui-menu/feedback.ui')
        self.widgets.add_from_file('../data/feedback.ui')
        self.window = self.w('MainWin')

        self.type_combo = self.w('type_combo')
        self.type_combo.connect("changed", self.on_type_combo_changed)

        self.text_view = self.w('text_view')
        self.email_entry = self.w('email_entry')

        self.file_chooser = self.w('file_chooser')
        self.file_chooser.connect('file-set', self.on_file_chooser_changed)
        self.filename = ""

        self.syslog_button = self.w('syslog_button')
        self.kernlog_button = self.w('kernlog_button')
        self.dpkglog_button = self.w('dpkglog_button')
        self.syslog_button.connect("toggled", self.on_syslog_toggled)
        self.kernlog_button.connect("toggled", self.on_kernlog_toggled)
        self.type_combo.set_active(0)

        self.system_info_expander = self.w('system_info_expander')
        self.system_info_expander.connect('notify::expanded', self.on_expander_changed)

        system_info_liststore = Gtk.ListStore(str, str)
        system_info_liststore.append(["DistroRelease", SystemInfo().get_distrorelease()])
        system_info_liststore.append(["DistroInfo", SystemInfo().get_os_info()])
        system_info_liststore.append(["Architecture", SystemInfo().get_system_architecture()])
        system_info_liststore.append(["Locale", SystemInfo().get_default_locale().__str__()])

        tree = Gtk.TreeView(system_info_liststore)
        self.system_info_expander.add(tree)

        column = Gtk.TreeViewColumn("Sytem info")
        title = Gtk.CellRendererText()
        value = Gtk.CellRendererText()
        column.pack_start(title, True)
        column.pack_start(value, True)
        column.add_attribute(title, "text", 0)
        column.add_attribute(value, "text", 1)

        tree.append_column(column)

        self.submit_button = self.widgets.get_object("submit_button")
        self.submit_button.connect('clicked', self.on_submit_clicked)

    def run(self):
        self.window.show_all()

    def quit(self):
        Gtk.main_quit()

    def on_type_combo_changed(self, combo):
        self.fb_type = combo.get_active_text()
        if self.fb_type == "System Bug":
            self.syslog_button.set_active(True)
            self.kernlog_button.set_active(True)
            self.dpkglog_button.set_active(True)
            print("===========")
        else:
            self.syslog_button.set_active(False)
            self.kernlog_button.set_active(False)
            self.dpkglog_button.set_active(False)
            print("!!!!!!!!!!")
        print("fb_type: %s" % self.fb_type)

    def on_file_chooser_changed(self, file_chooser):
        self.filename = file_chooser.get_filename()
        print("Choose %s" % self.filename)

    def on_syslog_toggled(self, button):
        if (button.get_active()):
            print("syslog choosed")
            #os.popen("pkexec python3 /home/feng/ukui/ukui-menu/ukui_menu/getsyslog.py")
        else:
            print("False")

    def on_kernlog_toggled(self, button):
        if (button.get_active()):
            print("kernlog choosed")
        else:
            print("False")

    def on_expander_changed(self, expander, data):
        label = expander.get_children()
        if (expander.get_expanded()):
            label[0].set_visible(True)
        else:

            label[0].set_visible(False)

    def on_submit_clicked(self, button):
        email = self.email_entry.get_text()
        attach_syslog = self.syslog_button.get_active()
        attach_kernlog = self.kernlog_button.get_active()
        attach_dpkglog = self.dpkglog_button.get_active()
        body = '\r\n'.join((
            "%s" % SystemInfo().get_distrorelease(),
            "%s" % SystemInfo().get_os_info(),
            "%s" % SystemInfo().get_system_architecture(),
            "%s" % SystemInfo().get_default_locale().__str__(),
            "%s" % self.get_text_view_str()
        ))
        Send().send(self.fb_type, body, email, self.filename, attach_syslog, attach_kernlog, attach_dpkglog)

    def get_text_view_str(self):
        self.text_buffer = self.text_view.get_buffer()
        butter_iter  = self.text_buffer.get_bounds()
        text_view_str = self.text_buffer.get_text(butter_iter[0], butter_iter[1], False)
        return text_view_str

    def fill_mail_body(self):
        temp = tempfile.NamedTemporaryFile()
        temp.file.write((self.get_text_view_str() + '\n').encode('UTF-8'))
        temp.file.write((SystemInfo().get_distrorelease() + '\n').encode('UTF-8'))
        temp.file.write((SystemInfo().get_os_info() + '\n').encode('UTF-8'))
        temp.file.write((SystemInfo().get_system_architecture() + '\n').encode('UTF-8'))
        temp.file.write((SystemInfo().get_default_locale() + '\n').encode('UTF-8'))
        temp.file.seek(0)
        return temp

if __name__ == '__main__':
    if not have_display:
        print('This program needs a running X session')
    app = Feedback()
    app.window.connect("destroy", Gtk.main_quit)
    app.run()
    Gtk.main()
