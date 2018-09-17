#!/usr/bin/python3
# -*- coding: UTF-8 -*-

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

import os
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.utils import formataddr
from email import encoders

syslog_file = '/var/log/syslog'
kernlog_file = '/var/log/kern.log'
dpkglog_file = '/var/log/dpkg.log'

class Send:
    _attachments = []
    def __init__(self):
        self.receivers = ['jianfengli@ubuntukylin.com']

    def add_attachment(self, filename):
        att = MIMEBase('application', 'octet-stream')
        att.set_payload(open(filename, 'rb').read())
        att.add_header('Content-Disposition', 'attachment', filename = ('utf-8', '', filename))
        encoders.encode_base64(att)
        self._attachments.append(att)

    def send(self, fb_type, body, sender, attach_file, syslog, kernel_log, dpkg_log):
        print("type: %s\n body: %s\n email %s\n" % (fb_type, body, sender))
        self.sender = sender
        self.message = MIMEMultipart('altermative')
        contents = MIMEText(body, 'plain', 'utf-8')
        user = os.getenv('USER')
        self.message['From'] = formataddr([user, sender])
        self.message['To'] = formataddr(["ubuntukylin", self.receivers[0]])
        self.message['Subject'] = fb_type
        if (os.path.isfile(attach_file)):
            self.add_attachment(attach_file)
        if (syslog and os.path.isfile(syslog_file) and os.access(syslog_file, os.R_OK)):
            self.add_attachment(syslog_file)
        if (kernel_log and os.path.isfile(kernlog_file) and os.access(kernlog_file, os.R_OK)):
            self.add_attachment(kernlog_file)
        if (dpkg_log and os.path.isfile(dpkglog_file) and os.access(dpkglog_file, os.R_OK)):
            self.add_attachment(dpkglog_file)

        for att in self._attachments:
            self.message.attach(att)
        self.message.attach(contents)
        try:
            smtpObj = smtplib.SMTP('localhost')
            smtpObj.sendmail(self.sender, self.receivers, self.message.as_string())
            smtpObj.quit()
            print("发送成功！")
            return True
        except smtplib.SMTPException:
            print("Error: 无法发送邮件")
            return False

if __name__ == '__main__':
    send = Send()
    print("send")
