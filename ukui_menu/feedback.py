#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright (C) 2018,Tianjin KYLIN Information Technology Co., Ltd.
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

import os.path, sys, os, stat, re
import gettext
import requests
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, GLib

from ukui_menu.systeminfo import SystemInfo
from ukui_menu.post import *

#sys.path.append("..")

# i18n
gettext.install("ukui-menu", "/usr/share/locale")

have_display = os.environ.get('DISPLAY') or os.environ.get('WAYLAND_DISPLAY')

url = 'https://www.ubuntukylin.com'
feedback_url = url + '/feedback'
captcha_url = url + '/member/ajax.php?action=code'
result_url = feedback_url + '/index.php?action=add&lang=cn'

syslog_file = '/var/log/syslog'
apportlog_file = '/var/log/apport.log'
dpkglog_file = '/var/log/dpkg.log'


class Feedback:
    def w(self, widget):
        return self.widgets.get_object(widget)

    def __init__(self):
        self.widgets = Gtk.Builder()
        self.widgets.set_translation_domain('ukui-menu')
        self.widgets.add_from_file('/usr/share/ukui-menu/feedback.ui')
        #self.widgets.add_from_file('../data/feedback.ui')
        self.window = self.w('MainWin')
        self.window.set_title(_("Feedback"))

        self.type_combo = self.w('type_combo')
        self.type_combo.connect("changed", self.on_type_combo_changed)

        self.text_view = self.w('text_view')
        self.email_entry = self.w('email_entry')

        self.file_chooser = self.w('file_chooser')
        filter_images = Gtk.FileFilter()
        filter_images.set_name(_("Images"))
        #filter_images.add_custom(Gtk.FileFilterFlags.FILENAME, self.limit_size, None)
        filter_images.add_pattern("*.png")
        filter_images.add_pattern('*.jpg')
        filter_images.add_pattern('*.jpeg')
        filter_images.add_pattern('*.bmp')
        filter_images.add_pattern('*.svg')
        self.file_chooser.add_filter(filter_images)

        self.syslog_button = self.w('syslog_button')
        self.apportlog_button = self.w('apportlog_button')
        self.dpkglog_button = self.w('dpkglog_button')
        self.syslog_button.set_active(True)

        self.system_info_expander = self.w('system_info_expander')
        self.system_info_expander.connect('notify::expanded', self.on_expander_changed)

        system_info_liststore = Gtk.ListStore(str, str)
        system_info_liststore.append([_("DistroRelease"), SystemInfo().get_distrorelease()])
        system_info_liststore.append([_("DistroInfo"), SystemInfo().get_os_info()])
        system_info_liststore.append([_("Desktop"), SystemInfo().get_desktop_env()])
        system_info_liststore.append([_("Architecture"), SystemInfo().get_system_architecture()])
        system_info_liststore.append([_("Locale"), SystemInfo().get_default_locale().__str__()])

        tree = Gtk.TreeView(system_info_liststore)
        self.system_info_expander.add(tree)

        column = Gtk.TreeViewColumn(_("System Info"))
        title = Gtk.CellRendererText()
        value = Gtk.CellRendererText()
        column.pack_start(title, True)
        column.pack_start(value, True)
        column.add_attribute(title, "text", 0)
        column.add_attribute(value, "text", 1)
        tree.append_column(column)

        self.captcha_label = self.w('captcha_label')
        self.captcha_entry = self.w('captcha_entry')
        self.captcha_image = self.w('captcha_image')

        self.get_feedback_session()

        self.submit_button = self.widgets.get_object("submit_button")
        self.submit_button.connect('clicked', self.on_submit_clicked)

        focus_chain = [self.type_combo, self.text_view, self.email_entry,
                       self.file_chooser, self.syslog_button, self.apportlog_button,
                       self.dpkglog_button, self.system_info_expander, self.captcha_entry,
                       self.submit_button]
        self.window.set_focus_chain(focus_chain)

    def run(self):
        self.window.show_all()

    def limit_size(self, filter_info, data):
        path = filter_info.filename
        file_size = os.path.getsize(path)
        if file_size < 1000000:
            return True
        else:
            return False

    def get_feedback_session(self):
        self.session = requests.session()
        image = get_captcha_image(captcha_url, self.session)
        self.captcha_image.set_from_file(image)

    def on_type_combo_changed(self, combo):
        fb_type = combo.get_active_text()
        if fb_type == _("System Bug"):
            self.syslog_button.set_active(True)
        else:
            self.syslog_button.set_active(False)

    def on_expander_changed(self, expander, data):
        label = expander.get_children()
        if (expander.get_expanded()):
            label[0].set_visible(True)
        else:

            label[0].set_visible(False)

    def on_submit_clicked(self, button):
        text_view_str = self.get_text_view_str()
        if not text_view_str.strip():
           dialog = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING,
               Gtk.ButtonsType.OK, _("Please input the description!"))
           response = dialog.run()
           if response == Gtk.ResponseType.OK:
               print("OK")
           dialog.destroy()
           return

        email = self.email_entry.get_text()
        if email.strip():
            if not self.check_email_addr(email):
                dialog = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING,
                    Gtk.ButtonsType.OK, _("Invalid email address!"))
                response = dialog.run()
                if response == Gtk.ResponseType.OK:
                    print("OK")
                dialog.destroy()
                return
        else:
            dialog = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING,
                    Gtk.ButtonsType.OK, _("Please input the email address!"))
            response = dialog.run()
            if response == Gtk.ResponseType.OK:
                print("OK")
            dialog.destroy()
            return

        filename = self.file_chooser.get_filename()
        if filename:
            if os.path.getsize(filename) > 1048576:
               dialog = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING,
                   Gtk.ButtonsType.OK, _("Maximum upload file size is 1MB!"))
               response = dialog.run()
               if response == Gtk.ResponseType.OK:
                   print("OK")
               dialog.destroy()
               return

        code = self.captcha_entry.get_text()
        if not code.strip():
           dialog = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING,
               Gtk.ButtonsType.OK, _("Please input the verification code!"))
           response = dialog.run()
           if response == Gtk.ResponseType.OK:
               print("OK")
           dialog.destroy()
           return

        body = '\n'.join((
            "%s" % SystemInfo().get_distrorelease(),
            "%s" % SystemInfo().get_os_info(),
            "%s" % SystemInfo().get_desktop_env(),
            "%s" % SystemInfo().get_system_architecture(),
            "%s" % SystemInfo().get_default_locale().__str__(),
            "%s" % text_view_str
        ))
        # TODO: Delete this once the web service support upload text file.
        log = ''
        if self.syslog_button.get_active() and os.access(syslog_file, os.R_OK):
            lines = self.tail(syslog_file, 10)
            log += '\nsyslog:'
            log += lines
        if self.apportlog_button.get_active() and os.access(apportlog_file, os.R_OK):
            lines = self.tail(apportlog_file, 5)
            log += '\napport.log:'
            log += lines
        if self.dpkglog_button.get_active() and os.access(dpkglog_file, os.R_OK):
            lines = self.tail(dpkglog_file, 5)
            log += '\ndpkg.log:'
            log += lines

        filter_chinese = re.compile(u'[\u4E00-\u9FA5]')
        log = filter_chinese.sub(r' ', log)
        body += log
        print (body)

        fb_type = self.type_combo.get_active_text()
        res = post_feedback(self.session, result_url, fb_type, email, body, filename, code)
        print (res)
        if res.find('成功') != -1:
            dialog = Gtk.MessageDialog(None, 0, Gtk.MessageType.INFO,
                Gtk.ButtonsType.OK, _('Successful feedback! Thank you!'))
            response = dialog.run()
            if response == Gtk.ResponseType.OK:
                dialog.destroy()
                self.window.destroy()
        elif res.find('验证码错误') != -1:
            dialog = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING,
                Gtk.ButtonsType.OK, _('Incorrect CAPTCHA, please re-enter!'))
            response = dialog.run()
            if response == Gtk.ResponseType.OK:
                self.get_feedback_session()
                dialog.destroy()
        else:
            dialog = Gtk.MessageDialog(None, 0, Gtk.MessageType.WARNING,
                    Gtk.ButtonsType.OK, _('Unknow error: ') + res)
            response = dialog.run()
            if response == Gtk.ResponseType.OK:
                dialog.destroy()
                self.window.destroy()

    def get_text_view_str(self):
        self.text_buffer = self.text_view.get_buffer()
        butter_iter  = self.text_buffer.get_bounds()
        text_view_str = self.text_buffer.get_text(butter_iter[0], butter_iter[1], False)
        return text_view_str

    def check_email_addr(self, addr):
        """Simple check for email validity"""
        if '@' not in addr:
            return False
        if addr.count('@') != 1:
            return False
        localpart, domainpart = addr.split('@')
        if localpart.startswith('.') or localpart.endswith('.'):
            return False
        if '.' not in domainpart:
            return False
        if domainpart.startswith('.') or domainpart.endswith('.'):
            return False
        return True

    def tail(self, filename, lines):
        bufsize = 8192
        fsize = os.stat(filename).st_size
        iter = 0
        with open(filename) as f:
            if bufsize > fsize:
                bufsize = fsize
            data = []
            while True:
                iter +=1
                f.seek(fsize-bufsize*iter)
                pos = f.tell()
                data.extend(f.readlines())
                if len(data) >= lines or pos == 0:
                    contents = ''.join(data[-lines:])
                    break
        return contents

if __name__ == '__main__':
    if not have_display:
        print('This program needs a running X session')
    app = Feedback()
    app.window.connect("destroy", Gtk.main_quit)
    app.run()
    Gtk.main()

